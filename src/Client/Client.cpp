#include "Client.hpp"

Client::Client(int fd, const std::string &IPadd_, const std::string &nickname_)
    : _fd(fd), _IPadd(IPadd_), _nickname(nickname_) {}

Client::~Client()
{
    // TODO close(_fd); ?
}

int Client::getFd() const
{
    return _fd;
}

const std::string &Client::getNickname() const
{
    return _nickname;
}

const std::string &Client::getIPadd() const
{
    return _IPadd;
}

void Client::setIPadd(const std::string &IPadd_)
{
    _IPadd = IPadd_;
}
void Client::setNickname(const std::string &nickname_)
{
    _nickname = nickname_;
}
