#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
public:
    Client(int fd, const std::string& nickname_ );
    ~Client();

    int getFd() const;
    const std::string& getUsername() const;
    void setNickname(const std::string& nickname_);

private:
    int _fd;
    std::string _nickname;
    bool isOpt;
    
};


#endif // !CLIENT_HPP

