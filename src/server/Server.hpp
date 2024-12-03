#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <map>

#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp"

class Server
{
public:
    Server(int sockFd_);
    ~Server();

private:
    int _sockFd;
    const std::string _name;
    std::map<bool, Client> _clients;
    
};





#endif