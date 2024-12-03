#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <map>
#include <netinet/in.h>

#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp"

class Server
{
public:
    Server(int sockFd_, uint16_t port_, std::string password_);
    ~Server();

    // Sets up the server by binding and listening for incoming connections
    bool SetUp();

private:
    int _sockFd;
    const std::string _name;
    std::map<bool, Client> _clients;
    uint16_t _port;
    std::string _password;
    struct sockaddr_in server_addr;


    
};





#endif