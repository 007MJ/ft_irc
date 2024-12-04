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
bool Server::AddClient(int clientFd_){
    Client newClient(clientFd_)
    _clients.push_back(clientFd_);

    for(int i = 0; i < MAX_CLIENTS ; ++i){
        if(_clientFds[i].fd == -1){ // -1 ==> available slot
            _clientFds[i].fd = clientFd;
            _clientFds[i].events = POLLRDNORM;  // Monitor for read events
            return true;
        }
    }
    return false;
}
bool Server::DeleteClient(int clientFd) {
    // Find and remove client from _clients vector
    auto it = std::find_if(_clients.begin(), _clients.end(),
        [clientFd](const Client& client) { return client.getFd() == clientFd; });
    
    if (it != _clients.end()) {
        _clients.erase(it);
    }

    // Remove the client from the pollfd array
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (_clientFds[i].fd == clientFd) {
            _clientFds[i].fd = -1;  // Mark as unused
            return true;
        }
    }
    return false;
}
