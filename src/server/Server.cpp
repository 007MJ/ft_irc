#include "../../includes/header.hpp"

void Server::connection(){
    struct sockaddr_in irc_addr;
    
    irc_addr.sin_family = AF_INET;
    irc_addr.sin_port = htons(this->port);
    irc_addr.sin_addr = 

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("%d\n", fd);

}

void Server::getport(char *number) { this->port = atol(number);}
void Server::getpassword(char *word) { this->password = word;}

Server::Server(){}
Server::~Server(){}