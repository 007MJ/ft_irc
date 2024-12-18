#include "Channel.hpp"
#include <sstream> 

Channel::Channel(std::string name_, std::string password_, Client& owner_)
: _name(name_),
_password(password_)
{
    AddClient(&owner_);
    _superUsers.insert(owner_.getFd()); // Adds the client to the list of super users

    _inviteOnly = false;
    _topic = "";
    _isRestrictedTopic = false;
    _limit = 8;

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

    if (IsMember(fd_)) {
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

bool Channel::IsMember(int fd_)
{
    return( _clients.find(fd_) != _clients.end() );
}

bool Channel::IsSuperUser(int fd_)
{
    return( _superUsers.find(fd_) != _superUsers.end() );
}

const std::string &Channel::GetPassword() const
{
    return _password;
}

void Channel::SetPassword(const std::string &password_)
{
    _password = password_;
}

const std::string &Channel::getTopic() const
{
    return _topic;
}

void Channel::SetTopic(const std::string &topic_)
{
    _topic = topic_;
}

bool Channel::InviteOnlyModeIsActivated() const
{
    return _inviteOnly;
}

bool Channel::TopicModeIsRestricted() const
{
    return _isRestrictedTopic;
}

void Channel::SetRestrictedTopicModeTo(bool restriction_)
{
    _isRestrictedTopic = restriction_;
}

void Channel::SetInviteOnlyModeTo(bool mode_)
{
    _inviteOnly = mode_;
}

int Channel::getChannelLimit() const
{
    return _limit;
}

void Channel::SetChannelLimit(int limit_)
{
    _limit = limit_;
}
