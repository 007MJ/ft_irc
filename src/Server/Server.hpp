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
#define USERLEN 20

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
    bool UserNickNameExists(Client *client_, const std::string& nickname_);
    bool SetClientNickName(Client *client_, const std::string &nickname_);
    bool SetClientUsername(Client *client_, const std::string &username_);
    bool SetClientInfos(Commands *cmd_, Client *client_, Server *irc_);
    bool ClientIsIdentified(Client *client_);

    bool AcceptClient();
    const struct pollfd* getClientFds() const;
    bool ReceiveDataFromClient(int fd);
    bool IsClientAuth(int fd_);
    bool AuthClient(Client *client_, std::string password_);
    std::vector<Channel> getChannel();
    std::vector<Client> getClients();
    void addChannel(std::string name, std::string pwd, Client &nc);
    void deleteChannel(size_t index_);
    int getClientIndex(int fd_);
    Channel* getChannelByName(const std::string &name_);
    size_t getChannelIndex(std::string &name_);
    // void remove(std::string room);

    const std::string& getName() const { return _name; }


private:
    int _sockFd;
    const std::string _name;
    uint16_t _port;
    std::string _password;
    struct sockaddr_in server_addr;
    std::vector<Client> _clients;
    std::vector<Channel> _channel;
    struct pollfd _clientFds[MAX_CLIENTS];
    struct sockaddr_in client_addr;

};





#endif