#include "Server.hpp"
#include "../../includes/utils.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>

Server::Server(int sockFd_, uint16_t port_, std::string password_) :
_sockFd(sockFd_),
_name("FT_IRC"),
_port(port_),
_password(password_)
{
    // Initialize client_fds
    _clientFds[0].fd = _sockFd;
    _clientFds[0].events = POLLRDNORM;

    // Initialize client_fds
    for (int i = 1; i < MAX_CLIENTS; ++i) {
        _clientFds[i].fd = -1; // No client connected
    }

}

Server::~Server(){
    for (int i = 1; i < MAX_CLIENTS; ++i) {
        if( _clientFds[i].fd != -1)
            close(_clientFds[i].fd);
    }
}

const struct pollfd* Server::getClientFds() const {
    return _clientFds;
}


bool Server::SetUp(){
    // Setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(_port);

    // TODO setsockopt ==> sets socket option (socket levels purpose)
    // TODO fcntl ==­> sets the server to non-blocking mode

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
    Client newClient(clientFd_, "127.0.0.1", "Bob");
    _clients.push_back(newClient);

    for(int i = 0; i < MAX_CLIENTS ; ++i){
        if(_clientFds[i].fd == -1){ // -1 ==> available slot
            _clientFds[i].fd = clientFd_;
            _clientFds[i].events = POLLRDNORM;  // Monitor for read events
            std::cout << "Client connected\n";
            return true;
        }
    }
    return false; // No slot found to add client
}
bool Server::DeleteClient(int clientFd_) {
    // Find and remove client from _clients vector
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getFd() == clientFd_) {
            close(clientFd_);
            _clients.erase(it);
            return true;  // Exit the loop after erasing
        }
    }

    // Remove the client from the pollfd array
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (_clientFds[i].fd == clientFd_) {
            _clientFds[i].fd = -1;  // Mark as unused
            return true;
        }
    }
    return false;
}

Client *Server::GetClientByFd(int fd_)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getFd() == fd_) {
            return &(*it);  // Return pointer to the Client object
        }
    }
    return NULL;
}

bool Server::AcceptClient(){
    socklen_t client_len;
    int client_fd;

    int poll_count = poll(_clientFds, MAX_CLIENTS, -1);
    if (poll_count < 0) {
            errorMsg("Error while trying to poll.");
            return false;
    }

    // Check for new incoming connections on the server socket
        if (_clientFds[0].revents & POLLRDNORM) {
            client_len = sizeof(client_addr);
            client_fd = accept(_sockFd, (struct sockaddr*)&client_addr, &client_len);
            if (client_fd < 0) {
                errorMsg("Error while trying to accept new connection.");
                return false;
            }
            if(! AddClient(client_fd)){
                std::cout << "Server full. Rejecting new connection.\n";
                DeleteClient(client_fd);
                return false;
            }
        }

    return true;
}

bool Server::ReceiveDataFromClient(int fd) {
    char buffer[BUFFER_SIZE];
    int n = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (n < 0) {
        errorMsg("Error while tryong to receive data.");
        return false;
    } else if (n == 0) {
        std::cout << "Client " << fd << " disconnected\n";
        DeleteClient(fd);  // Handle client disconnection
        return false;
    } else {
        buffer[n] = '\0';
        std::cout << "Received from client " << fd << ": " << buffer << std::endl;
        send(fd, "Message received\n", 17, 0);  // Respond to client
        return true;
    }
}
