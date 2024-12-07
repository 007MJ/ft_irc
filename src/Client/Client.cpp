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
