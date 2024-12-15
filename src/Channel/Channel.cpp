#include "Channel.hpp"
#include <sstream> 

Channel::Channel(std::string name_, Client& owner_) : _name(name_)
{
    AddClient(&owner_);
    _superUsers.insert(owner_.getFd()); // Adds the client to the list of super users
}

Channel::~Channel()
{
    //TODO clear the map and the set
}

void Channel::AddClient(Client *client_)
{
    if (client_)
    {
        _clients[client_->getFd()] = client_;
    }
}

void Channel::RemoveClient(int fd_)
{
    _clients.erase(fd_);
}

std::string Channel::AddClientAsSuperUser(int fd_)
{
    std::ostringstream output;

    if (_clients.find(fd_) != _clients.end()) {
        _superUsers.insert(fd_);
        output << _clients[fd_]->getNickname() << " added as moderator to #" << _name << " channel.\n";
    } else {
        output << "Error: Client with fd " << fd_ << " is not a member of #" << _name << " channel.\n";
    }

    return output.str();
}

void Channel::RemoveClientAsSuperUser(int fd_)
{
    _superUsers.erase(fd_);
}
