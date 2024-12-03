#include "Server/Server.hpp"
#include "../includes/utils.hpp"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 6667
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int main() {
    // Server server(50, PORT, "Password123$");

    int server_fd, client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];
    
    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        errorMsg("Error while trying t create a socket");
        exit(1);
    }
    Server server(server_fd, PORT, "Password123$");
    if(!server.SetUp())
        exit(1);

    // Setup server address
    // memset(&server_addr, 0, sizeof(server_addr));
    // server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = INADDR_ANY;
    // server_addr.sin_port = htons(PORT);

    // // Bind socket to port
    // if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    //     perror("bind");
    //     close(server_fd);
    //     exit(1);
    // }

    // // Listen for incoming connections
    // if (listen(server_fd, 5) < 0) {
    //     perror("listen");
    //     close(server_fd);
    //     exit(1);
    // }

    // std::cout << "Server listening on port " << PORT << "...\n";

    // Prepare poll() structure
    struct pollfd client_fds[MAX_CLIENTS];
    client_fds[0].fd = server_fd;
    client_fds[0].events = POLLRDNORM;
    
    // Initialize client_fds
    for (int i = 1; i < MAX_CLIENTS; ++i) {
        client_fds[i].fd = -1; // No client connected
    }

    while (true) {
        int poll_count = poll(client_fds, MAX_CLIENTS, -1); // Block until something happens
        if (poll_count < 0) {
            perror("poll");
            break;
        }

        // Check for new incoming connections on the server socket
        if (client_fds[0].revents & POLLRDNORM) {
            client_len = sizeof(client_addr);
            client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
            if (client_fd < 0) {
                perror("accept");
                continue;
            }

            // Find a free slot in client_fds
            bool slot_found = false;
            for (int i = 1; i < MAX_CLIENTS; ++i) {
                if (client_fds[i].fd == -1) {
                    client_fds[i].fd = client_fd;
                    client_fds[i].events = POLLRDNORM;
                    std::cout << "Client connected\n";
                    slot_found = true;
                    break;
                }
            }

            if (!slot_found) {
                std::cout << "Server full. Rejecting new connection.\n";
                close(client_fd);
            }
        }

        // Handle data from clients
        for (int i = 1; i < MAX_CLIENTS; ++i) {
            if (client_fds[i].fd != -1 && (client_fds[i].revents & POLLRDNORM)) {
                int n = recv(client_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                if (n < 0) {
                    perror("recv");
                } else if (n == 0) {
                    std::cout << "Client disconnected\n";
                    close(client_fds[i].fd);
                    client_fds[i].fd = -1; // Mark as available
                } else {
                    buffer[n] = '\0'; // Null-terminate the message
                    std::cout << "Received from client: " << buffer << std::endl;
                    // Echo message back to the client
                    send(client_fds[i].fd, "Message received\n", 17, 0);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}



