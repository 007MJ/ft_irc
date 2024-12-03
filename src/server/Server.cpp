#include "Server.hpp"

Server::Server(int sockFd_) :  _sockFd(sockFd_), _name("FT_IRC"){
    std::cout << _name + " Server initialized with socket: " << _sockFd << std::endl;
}
Server::~Server(){}
