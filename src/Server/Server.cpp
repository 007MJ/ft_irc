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
    Client newClient(clientFd_, "127.0.0.1", "");
    // std::cout << "###### Client AddClient() #########, fd: " << clientFd_ << std::endl;
    // newClient.setInfos(false);
    // std::cout << "Client nickname:: " + newClient.getNickname() ;
    // if(newClient.getNickname().empty())
    // {
    //     std::cout << " it's is empty\n";
    // }
    // else
    // {
    //     std::cout << " it's not empty\n";
    // }
    _clients.push_back(newClient);

    for (int i = 1; i < MAX_CLIENTS; ++i)
    {
        if (_clientFds[i].fd == -1)
        { // -1 ==> available slot
            _clientFds[i].fd = clientFd_;
            _clientFds[i].events = POLLRDNORM; // Monitor for read events
            std::cout << newClient.getNickname() + " connected\n";
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

bool Server::UserNickNameExists(Client *client_, const std::string& nickname_)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if(client_->getFd() != it->getFd() && it->getNickname() == nickname_)
        {
            return true;
        }
    }
    return false;
}

bool Server::SetClientNickName(Client *client_, const std::string &nickname_)
{
    std::string response = "";
    if(nickname_.empty())
    {
        response = ":server.name NOTICE * :Nickname cannot be empty. Please choose another one.\r\n";
        clean_send(client_->getFd(), response.c_str());
        std::cout << "Nickname cannot be empty. Please choose another one.\r\n";
        return false;
    }
    if(UserNickNameExists(client_, nickname_))
    {
        response = ":server.name NOTICE * :Nickname already taken. Please choose another one.\r\n";
        clean_send(client_->getFd(), response.c_str());
        std::cout << "Nickname already taken. Please choose another one.\r\n";
        return false;
    }
    response = ":FT_IRC NOTICE * :Nickname set to " + nickname_ + "\r\n";
    clean_send(client_->getFd(), response.c_str());
    client_->setNickname(nickname_);
    std::cout << "Nickname after setting: " << client_->getNickname() << std::endl;
    return true;
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
        // std::cout << "###### Client accepted, fd: " << client_fd << std::endl;
        if (client_fd < 0)
        {
            errorMsg("Error while trying to accept new connection.");
            return false;
        }
        if(fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1){
            errorMsg("fcntl() failed\n");
            return false;
        }
        // std::cout << "###### Client fcntl() #########, fd: " << client_fd << std::endl;
        if (!AddClient(client_fd))
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
    int cliIndex = getClientIndex(fd_);
    if(cliIndex == -1){
        std::cout << "Client with fd: " << fd_ << " not found" << std::endl;
        return false;
    }    
    return _clients[cliIndex].getIsAuth();
}
std::vector<Client> Server::getClients() {return this->_clients;}

bool Server::ClientIsIdentified(Client *client_){
    // bool isID = !client_->getNickname().empty() && !client_->getUsername().empty() && client_->getIsAuth();
    // if(isID) {std::cout << "Client with fd: " << client_->getFd() << " is identified " << std::endl;}
    // else{std::cout << "Client with fd: " << client_->getFd() << " is not identified " << std::endl;}
    // std::cout << "--------------------------------infos----------------------------------\n";
    // std::cout << "Nickname: " << client_->getNickname() << std::endl;
    // std::cout << "Username: " << client_->getUsername() << std::endl;
    // std::cout << "IsAuth: " << client_->getIsAuth() << std::endl;

    return ( !client_->getNickname().empty() && !client_->getUsername().empty() && client_->getIsAuth());
}

bool Server::AuthClient(Client *client_, std::string password_)
{
    std::string response = "";
    if (password_ == _password)
    {
        client_->setIsAuth();
        std::cout << "---------------Client successfully authenticated----------\n";
        response = ":" + _name + " NOTICE * :You have been successfully authenticated!\r\n";
        return clean_send(client_->getFd(), response.c_str()), true;
    }
    std::cout << "Wrong password\n";
    response = ":server.name NOTICE * :Wrong password. Try again.\r\n";
    return clean_send(client_->getFd(), response.c_str()), false;
}

bool Server::SetClientInfos(Commands *cmd_, Client *client_, Server *irc_)
{
    // std::cout << "----------------- SetClientInfos ----------------- command type: " + cmd_->get_type_cmd() << std::endl;
    std::string response = "";
    if(cmd_->get_type_cmd() == "PASS")
    {
        if(client_->getIsAuth())
        {
            response = ":" + irc_->getName() + " NOTICE * :Error: Already authenticated\r\n";
            clean_send(client_->getFd(), response.c_str());
            return false;
        }
        if (cmd_->get_splitcmds().size() < 2)
        {
            response = ":" + irc_->getName() + " NOTICE * :Error: Missing password\r\n";
            clean_send(client_->getFd(), response.c_str());
            return false;
        }
        else if (cmd_->get_splitcmds().size() > 2)
        {
            response = ":" + irc_->getName() + " NOTICE * :Error: Too many arguments\r\n";
            clean_send(client_->getFd(), response.c_str());
            return false;
        }
        irc_->AuthClient(client_, cmd_->get_splitcmds()[1]);
    }
    else if (cmd_->get_type_cmd() == "NICK")
    {
        if(cmd_->get_splitcmds().size() < 2)
        {
            response = ":" + irc_->getName() + " NOTICE * :Error: Missing nickname\r\n";
            clean_send(client_->getFd(), response.c_str());
            return false;
        }
        else if(cmd_->get_splitcmds().size() > 2)
        {
            response = ":" + irc_->getName() + " NOTICE * :Error: Too many arguments\r\n";
            clean_send(client_->getFd(), response.c_str());
            return false;
        }
        // std::cout << "NICK" << std::endl;
        irc_->SetClientNickName(client_, cmd_->get_splitcmds()[1]);
    }
    else if (cmd_->get_type_cmd() == "USER") // USER <username> 0 * <realname>
    {
        if(cmd_->get_splitcmds().size() >= 5 && cmd_->get_splitcmds()[2] == "0" && cmd_->get_splitcmds()[3] == "*")
        {
            std::string username = getFullUsername(cmd_->get_splitcmds());
            // std::cout << "Received username: ================ " << username << std::endl;
            if(username.empty())
            {
                response = ":" + irc_->getName() + " NOTICE * :Error: Missing username\r\n";
                clean_send(client_->getFd(), response.c_str());
                return false;
            }
            else if (username.size() == 1)
            {
                response = ":" + irc_->getName() + " NOTICE * :Error: Username too short\r\n";
                clean_send(client_->getFd(), response.c_str());
                return false;
            }
            else if (username.size() > USERLEN)
            {
                response = ":" + irc_->getName() + " NOTICE * :Error: Username too long\r\n";
                clean_send(client_->getFd(), response.c_str());
                return false;                
            }
            else if (username.find_first_of("\a\b\f\n\r\t\v") != std::string::npos)
            {
                response = ":" + irc_->getName() + " NOTICE * :Error: Invalid characters in username\r\n";
                clean_send(client_->getFd(), response.c_str());
                return false;
            }
            // std::cout << "USER" << std::endl;
            response = ":" + irc_->getName() + " NOTICE * :Username set to " + username + "\r\n";
            clean_send(client_->getFd(), response.c_str());
            client_->setUsername(username);
        }
        else
        {
            response = ":" + irc_->getName() + " NOTICE * :Error: Invalid USER command\r\n";
            clean_send(client_->getFd(), response.c_str());
            return false;
        }

        // return irc_->SetClientNickName(client_, cmd_->get_splitcmds()[1]);
    }
    if(ClientIsIdentified(client_))
        client_->setIsIdentified();
    return true;
}



int Server::getClientIndex(int fd_){
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        if (_clients[i].getFd() == fd_)
            return static_cast<int>(i); // Return the index as an integer
    }
    return -1; // Return -1 if the client is not found
}

Channel *Server::getChannelByName(const std::string &name_)
{
    for (size_t i = 0; i < _channel.size(); ++i)
    {
        if (_channel[i].GetName() == name_)
            return &_channel[i];
    }
    return nullptr;
}

std::vector<Channel> Server::getChannel(){return this->_channel;}

void Server::addChannel(std::string name, std::string pwd, Client &nc) {
    std::cout << "******************New channel added****************" << std::endl;
    Channel newRoom(name, pwd, nc);
    _channel.push_back(newRoom);
}
