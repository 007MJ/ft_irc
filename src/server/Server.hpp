#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <map>
#include <vector>
#include <netinet/in.h>
#include <sys/poll.h>

#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp"

class Server
{
public:
    Server(int sockFd_, uint16_t port_, std::string password_);
    ~Server();

    // Sets up the server by binding and listening for incoming connections
    bool SetUp();
    bool AddClient(int clientFd_);
    bool DeleteClient(int clientFd_);
    Client* GetClientByFd(int fd_);
    bool ClientExists(int fd_);

private:
    int _sockFd;
    const std::string _name;
    uint16_t _port;
    std::string _password;
    struct sockaddr_in server_addr;
    std::vector<Client> _clients;
    struct pollfd _clientFds[MAX_CLIENTS];


    
};





#endif