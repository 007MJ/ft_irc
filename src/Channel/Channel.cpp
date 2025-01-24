#include "Channel.hpp"
#include <sstream> 
#include "../../includes/utils.hpp"

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
        // std::cout << "###############Before adding " << client_->getNickname() << std::endl;
        // ListClients();
        // std::cout << "---------------------" << client_->getNickname() + " with fd: " << client_->getFd() << " added to the channel----------" << std::endl;
        // std::cout << "Client memory address: " << client_ << std::endl;
        // _clients.insert(std::pair<int, Client*>(client_->getFd(), client_));
        // std::cout << "###############After adding " << client_->getNickname() << std::endl;
        _clients[client_->getFd()] = client_;
                
        std::string msg = ":" + client_->getNickname() + " PRIVMSG " + GetName() + " :" + client_->getNickname() + " has joined the channel.\r\n";
        SendToChannel(client_, msg);
        displayChannelUsers(client_->getFd());
        ListClients();
        // std::cout << "*****************************List of users**************************** ";
        // std::cout << _clients.size() << " users" << std::endl;
        // for(std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        // {
        //     std::cout << "- " << it->second->getNickname() << std::endl;
        // }
    }
    std::cout << "the size of the channel :"<< _clients.size() << std::endl;
}

void Channel::RemoveClient(int fd_)
{
    for (std::map<int, Client *> ::iterator it = _clients.begin() ; it != _clients.end(); it++){
        if (it->first == fd_)
        {
            std::cout << " remove CLient  : "<< it->second->getNickname() << std::endl;

        }
        else
            std::cout << " false remove CLient  : "<< it->second->getNickname() << std::endl;
        std::cout << "==> fd1  : "<< fd_ << std::endl;
        std::cout << "==> it->first  : "<< it->first << std::endl;
    }
    if(IsSuperUser(fd_))
        RemoveClientAsSuperUser(fd_);
    _clients.erase(fd_);
    std::cout << "the size of the channel :"<< _clients.size() << std::endl;
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
    if (_topic[0] == ':') {
        _topic.erase(0, 1);  // Remove the colon at the beginning
    }

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
    if (_limit < 1)
        _limit = 1;
}
std::map<int, Client*> Channel::getClientChannel() {return this-> _clients;}
std::set<int> Channel::getSuperUsers() {return this->_superUsers;}

void Channel::ListClients(){
    std::cout << "List of users in the channel: " << _name << " (" << _clients.size() << ")" << std::endl;
    std::map<int, Client*>::iterator it = _clients.begin();
    for(; it != _clients.end(); ++it){
        std::cout << "- " + it->second->getNickname() + " fd: " << it->first << " memory address: " << it->second << std::endl;
    }
}

void Channel::SendToChannel(Client *author_, std::string& msg_){
    std::map<int, Client*>::iterator it = _clients.begin();
    for(; it != _clients.end(); ++it){
        if(it->first != author_->getFd()){
            // SendToUser(author_, it->second, msg_);
            clean_send(it->first, msg_.c_str());
        }
    }
}

void Channel::displayChannelUsers(int author) {
    Client *client = _clients[author];
    if (!client) return; // Ensure the author is a valid client

    std::string nickname = client->getNickname();

    // RPL_TOPIC (332): Show the channel topic
    std::string response = ":FT_IRC 332 " + nickname + " " + _name + " :" + _topic + "\r\n";
    clean_send(author, response.c_str());

    // RPL_NAMREPLY (353): List users in the channel
    response = ":FT_IRC 353 " + nickname + " = " + _name + " :";
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        response += it->second->getNickname() + " ";
    }
    response += "\r\n";
    clean_send(author, response.c_str());

    // RPL_ENDOFNAMES (366): End of the NAMES list
    response = ":FT_IRC 366 " + nickname + " " + _name + " :End of /NAMES list.\r\n";
    clean_send(author, response.c_str());
}

/*

PASS abc
NICK izem
USER amdan 0 * amdan


*/