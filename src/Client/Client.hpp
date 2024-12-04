#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
public:
    Client(int fd, const std::string& IPadd_, const std::string& nickname_ );
    ~Client();

    int getFd() const;
    const std::string& getNickname() const;
    const std::string& getIPadd() const;

    void setIPadd(const std::string& IPadd_);
    void setNickname(const std::string& nickname_);

private:
    int _fd;
    std::string _IPadd;
    std::string _nickname;
    
};


#endif // !CLIENT_HPP

