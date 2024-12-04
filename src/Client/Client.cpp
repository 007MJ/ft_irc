#include "Client.hpp"

Client::Client(int fd, const std::string& nickname_)
    : _fd(fd), _nickname(nickname_) {}

Client::~Client() {}

int Client::getFd() const {
    return _fd;
}

const std::string& Client::getUsername() const {
    return _nickname;
}

void Client::setUsername(const std::string& nickname_) {
    _nickname = nickname_;
}
