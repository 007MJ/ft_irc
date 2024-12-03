#include "Server.hpp"
#include "../../includes/utils.hpp"
#include <arpa/inet.h>

Server::Server(int sockFd_, uint16_t port_, std::string password_) :
_sockFd(sockFd_),
_name("FT_IRC"),
_port(port_),
_password(password_)
{
    // std::cout << _name + " Server initialized with socket: " << _sockFd << std::endl;
    // std::cout << "Listenning on port: " << _port << std::endl    << "Password: " << _password << std::endl;
}

Server::~Server(){}

bool Server::SetUp(){
    // Setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(_port);

    // Bind socket to port
    if (bind(_sockFd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        errorMsg("Error while trying to bind the server's socket to the given port.");
        close(_sockFd); // TODO should I close _sockFd once only in the destructor?
        return false;
    }

    // Listen for incoming connections
    if (listen(_sockFd, 5) < 0) { // TODO change number 5
        errorMsg("Error while trying to make the server listen for incoming connections.");
        close(_sockFd);
        return false;
    }

    std::cout << "Server listening on port " << _port << "...\n";
    return true;

}

