#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <map>
#include <vector>
#include <netinet/in.h>
#include <sys/poll.h>

#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

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

    bool AcceptClient();
    const struct pollfd* getClientFds() const;
    bool ReceiveDataFromClient(int fd);
    bool IsClientAuth(int fd_);
    bool AuthClient(int fd_);


private:
    int _sockFd;
    const std::string _name;
    uint16_t _port;
    std::string _password;
    struct sockaddr_in server_addr;
    std::vector<Client> _clients;
    struct pollfd _clientFds[MAX_CLIENTS];
    struct sockaddr_in client_addr;

};





#endif