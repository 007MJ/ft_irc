#include "Server.hpp"
#include "../../includes/utils.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <fcntl.h>
#include <cstring>
// #include <stdio.h>

Server::Server(int sockFd_, uint16_t port_, std::string password_) : _sockFd(sockFd_),
                                                                     _name("FT_IRC"),
                                                                     _port(port_),
                                                                     _password(password_)
{
    // Initialize client_fds
    _clientFds[0].fd = _sockFd;
    _clientFds[0].events = POLLRDNORM;

    // Initialize client_fds
    for (int i = 1; i < MAX_CLIENTS; ++i)
    {
        _clientFds[i].fd = -1; // No client connected
        _clientFds[i].events = 0;
    }
}

Server::~Server()
{
    for (int i = 1; i < MAX_CLIENTS; ++i)
    {
        if (_clientFds[i].fd != -1)
            close(_clientFds[i].fd);
    }
}

const struct pollfd *Server::getClientFds() const
{
    return _clientFds;
}

bool Server::SetUp()
{
    // Setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(_port);

    // TODO setsockopt ==> sets socket option (socket levels purpose)
    // TODO fcntl ==­> sets the server to non-blocking mode

    // Bind socket to port
    if (bind(_sockFd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        errorMsg("Error while trying to bind the server's socket to the given port.");
        close(_sockFd); // TODO should I close _sockFd once only in the destructor?
        return false;
    }

    // Listen for incoming connections
    if (listen(_sockFd, 5) < 0)
    { // TODO change number 5
        errorMsg("Error while trying to make the server listen for incoming connections.");
        close(_sockFd);
        return false;
    }

    std::cout << "Server listening on port " << _port << "...\n";

    return true;
}
bool Server::AddClient(int clientFd_)
{
    Client newClient(clientFd_, "127.0.0.1", "Bob");
    _clients.push_back(newClient);

    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (_clientFds[i].fd == -1)
        { // -1 ==> available slot
            _clientFds[i].fd = clientFd_;
            _clientFds[i].events = POLLRDNORM; // Monitor for read events
            std::cout << "Client connected\n";
            return true;
        }
    }
    return false; // No slot found to add client
}
bool Server::DeleteClient(int clientFd_)
{
    // Find and remove client from _clients vector
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getFd() == clientFd_)
        {
            _clients.erase(it);
            break; // Exit the loop after erasing
        }
    }

    // Remove the client from the pollfd array
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (_clientFds[i].fd == clientFd_)
        {
            close(clientFd_);
            _clientFds[i].fd = -1;    // Mark as unused
            _clientFds[i].events = 0; // Clear any associated events
            _clientFds[i].revents = 0;
            return true;
        }
    }
    return false;
}

Client *Server::GetClientByFd(int fd_)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getFd() == fd_)
        {
            return &(*it); // Return pointer to the Client object
        }
    }
    return NULL;
}

bool Server::AcceptClient()
{
    socklen_t client_len;
    int client_fd;

    int poll_count = poll(_clientFds, MAX_CLIENTS, -1);
    if (poll_count < 0)
    {
        errorMsg("Error while trying to poll.");
        return false;
    }

    // Check for new incoming connections on the server socket
    if (_clientFds[0].revents & POLLRDNORM)
    {
        client_len = sizeof(client_addr);
        client_fd = accept(_sockFd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0)
        {
            errorMsg("Error while trying to accept new connection.");
            return false;
        }
        if(fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1){
            errorMsg("fcntl() failed\n");
            return false;
        }
        else if (!AddClient(client_fd))
        {
            std::cerr << "Server full. Rejecting new connection.\n";
            DeleteClient(client_fd);
            return false;
        }
        else
        {
            if (send(client_fd, "Please enter the password to access the server\n", 48, 0) <= 0)
            {
                std::cerr << "Failed to send prompt to client " << client_fd << "\n";
                DeleteClient(client_fd);
            }
        }
    }
    return true;
}

// bool Server::ReceiveDataFromClient(int fd)
// {
//     char buffer[BUFFER_SIZE];
//     int n = recv(fd, buffer, sizeof(buffer) - 1, 0);
//     if (n < 0)
//     {
//         errorMsg("Error while trying to receive data.");
//         return false;
//     }
//     else if (n == 0)
//     {
//         std::cout << "Client " << fd << " disconnected\n";
//         DeleteClient(fd); // Handle client disconnection
//         return false;
//     }
//     else
//     {
//         buffer[n] = '\0';
//         std::cout << "Received from client " << fd << ": " << buffer << std::endl;
//         send(fd, "Message received\n", 17, 0); // Respond to client
//         return true;
//     }
// }

bool Server::IsClientAuth(int fd_)
{
    return _clients[fd_].getIsAuth();
}

// bool Server::AuthClient(int fd_)
// {

//     if (send(fd_, "Please enter the password to access the server\n", 48, 0) < 1)
//     {
//         return false;
//     }
//     char buffer[BUFFER_SIZE];
//     int n = recv(fd_, buffer, sizeof(buffer) - 1, 0);
//     if (n <= 0) {
//         if (n == 0) {
//             std::cout << "Client " << fd_ << " disconnected\n";
//         } else {
//             errorMsg("Error while trying to receive data.");
//         }
//         DeleteClient(fd_);
//         return false;
//     }
//     // Respond to client

//         buffer[n - 1] = '\0';
//         std::cout << "-" << buffer << "-    -" << _password.c_str() << "-" << "\n";

//         if (!strcmp(buffer, _password.c_str()))
//         {
//             Client* client = GetClientByFd(fd_);
//             if (client) {
//                 client->setIsAuth(); // Mark client as authenticated
//             }
//             send(fd_, "You have been successfully authenticated\n", 43, 0);
//             return true;
//         }

//     send(fd_, "Wrong password, authentification failed. Try again.\n", 51, 0);
//     return false;

// }

bool Server::AuthClient(int fd_)
{

    // Receive input
    char buffer[BUFFER_SIZE];
    int n = recv(fd_, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0)
    {
        if (n == 0)
        {
            std::cout << "Client " << fd_ << " disconnected\n";
        }
        else
        {
            std::cout << "Error receiving data from client " << fd_ << "\n";
        }
        DeleteClient(fd_);
        return false; // Disconnect the client
    }

    buffer[n] = '\0'; // Null-terminate input
    std::string input(buffer);
    input.erase(input.find_last_not_of("\r\n") + 1);

    // Verify the password
    if (input == _password)
    {
        _clients[fd_].setIsAuth();
        if (send(fd_, "You have been successfully authenticated!\n", 43, 0) < 1)
        {
            std::cout << "Error sending authentication success message to client " << fd_ << "\n";
            return false;
        }
        return true;
    }
    else
    {
        if (send(fd_, "Wrong password, authentication failed. Try again\n", 50, 0) < 1)
        {
            std::cout << "Error sending authentication failure message to client " << fd_ << "\n";
            return false;
        }
        return false; // Wait for the client to try again
    }
}