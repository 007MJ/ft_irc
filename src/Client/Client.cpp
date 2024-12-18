#include "Client.hpp"

Client::Client(int fd, const std::string &IPadd_, const std::string &nickname_)
    : _fd(fd), _IPadd(IPadd_), _nickname(nickname_) {
        _isAuth = false;
    }

Client::~Client()
{
    // TODO close(_fd); ?
}

int Client::getFd() const
{
    return _fd;
}

const std::string& Client::getNickname() const
{
    return _nickname;
}

const std::string& Client::getUsername() const
{
    return _username;
}

const std::string& Client::getIPadd() const
{
    return _IPadd;
}

bool Client::getIsAuth() const
{
    return _isAuth;
}



void Client::setIPadd(const std::string &IPadd_)
{
    _IPadd = IPadd_;
}
void Client::setNickname(const std::string &nickname_)
{
    _nickname = nickname_;
}

void Client::setUsername(const std::string &username_)
{
    _username = username_;
}

void Client::setIsAuth()
{
    _isAuth = true;
}
void Client::setJoin(std::map<std::string, std::string> arg) {this->_join = arg;}
void Client::setTopic(std::map<std::string, std::string> arg) {this->_topic = arg;}
void Client::setInvinte(std::map<std::string, std::string> arg) { this->_invite  = arg;}
void Client::setMode(context_mode arg) {this->_mode = arg;}
void Client::setKick(context_mode arg) {this->_kick = arg;}
void Client::setTypeCmd(std::string type) {this->_typeCmd = type;}

std::string Client::getTypeCmd() {return this->_typeCmd;}
std::map<std::string, std::string> Client::getJoin() {return this->_join ;}
std::map<std::string, std::string> Client::getTopic() {return this->_topic;}
std::map<std::string, std::string> Client::getInvite() { return this->_invite;}
context_mode Client::getMode() {return this->_mode;}
context_mode Client::getKick() {return this->_kick;}