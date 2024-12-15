#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "../Client/Client.hpp"
#include <map>
#include <set>

class Channel
{
public:
    Channel(std::string name_, Client& owner_);
    ~Channel();

    void AddClient(Client* client_);
    void RemoveClient(int fd_);
    std::string AddClientAsSuperUser(int fd_);
    void RemoveClientAsSuperUser(int fd_);
    

private:
    std::string _name;
    std::map<int, Client*> _clients;  // Key: fd, Value: pointer to Client instance
    std::set<int> _superUsers; 
    
};




#endif // !CHANNEL_HPP



