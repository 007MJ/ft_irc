#include "Server/Server.hpp"
#include "../includes/utils.hpp"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 6668
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int main() {
    int server_fd;
    char buffer[BUFFER_SIZE];
    
    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        errorMsg("Error while trying t create a socket");
        exit(1);
    }
    // TODO check PORT value : [ 0 - 65535 ]
    Server ft_irc(server_fd, PORT, "Password123$");
    if(!ft_irc.SetUp()) // TODO handle signals (must close all fds before exiting)
        exit(1);


    while (true) {
        
        if(!ft_irc.AcceptClient())
            break;

        // Handle data from clients
        for (int i = 1; i < MAX_CLIENTS; ++i) {
            if (ft_irc.getClientFds()[i].fd != -1 && 
                (ft_irc.getClientFds()[i].revents & POLLRDNORM)) {
                
                int n = recv(ft_irc.getClientFds()[i].fd, buffer, sizeof(buffer) - 1, 0);
                if (n < 0) {
                    perror("recv");
                } else if (n == 0) {
                    std::cout << "Client " << ft_irc.GetClientByFd(ft_irc.getClientFds()[i].fd)->getNickname() << " disconnected\n";
                    ft_irc.DeleteClient(ft_irc.getClientFds()[i].fd);
                } else {
                    buffer[n] = '\0';
                    std::cout << "Received from client " << ft_irc.getClientFds()[i].fd 
                            << ": " << buffer << std::endl;
                    send(ft_irc.getClientFds()[i].fd, "Message received\n", 17, 0);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}



