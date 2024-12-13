#include "Server/Server.hpp"
#include "Commands/Commands.hpp"
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

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        errorMsg("Wrong args number.\nTo run the program, the format must be as follow:\n./ircserv <port> <password>");
        return 1;
    }
    int port = 0;
    std::string password = "";

    if (!ValidateAndStoreArgs(argv, &port, password))
    {
        return 1;
    }
    std::cout << "PORT: " << port << std::endl;
    if (port < 1 || port > 65535)
    {
        errorMsg("Invalid port number. Please use a port in the range [1-65535].");
        return 1;
    }

    // Create server socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        errorMsg("Error while trying to create a socket");
        exit(1);
    }

    Server ft_irc(server_fd, port, password);
    if (!ft_irc.SetUp())
    {
        errorMsg("Failed to set up the server.");
        close(server_fd);
        return 1;
    } // TODO handle signals (must close all fds before exiting)

    while (true)
    {
        if (!ft_irc.AcceptClient())
            break;

        // Handle data from clients
        for (int i = 1; i < MAX_CLIENTS; ++i)
        {
            if (ft_irc.getClientFds()[i].fd != -1 &&
                (ft_irc.getClientFds()[i].revents & POLLRDNORM))
            {

                if (!ft_irc.IsClientAuth(ft_irc.getClientFds()[i].fd))
                {
                    if (!ft_irc.AuthClient(ft_irc.getClientFds()[i].fd))
                    {
                        continue; // Skip to the next client if awaiting authentication
                    }
                }
                // Le client doit s'identifier (Nickname)
                else
                {
                    char buffer[BUFFER_SIZE];
                int n = recv(ft_irc.getClientFds()[i].fd, buffer, sizeof(buffer) - 1, 0);
                if (n <= 0) {
                    if (n == 0) {
                        std::cout << "Client "
                                  << ft_irc.GetClientByFd(ft_irc.getClientFds()[i].fd)->getNickname()
                                  << " disconnected\n";
                    }
                    else {
                        std::cout << "Error receiving data from client " << ft_irc.getClientFds()[i].fd << "\n";
                    }
                    ft_irc.DeleteClient(ft_irc.getClientFds()[i].fd);
                } else {
                    // Tout commence ici !
                    buffer[n] = '\0';
                    std::cout << "Received from client " 
                              << ft_irc.getClientFds()[i].fd << ": " << buffer << "\n";
                    ClientHandler(buffer, ft_irc.GetClientByFd(ft_irc.getClientFds()[i].fd));
                
                }
                }
            }
        }
    }
    close(server_fd);
    return 0;
}
