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
    const std::string& getUsername() const;
    const std::string& getIPadd() const;
    bool getIsAuth() const;

    void setIPadd(const std::string& IPadd_);
    void setNickname(const std::string& nickname_);
    void setUsername(const std::string& username_);
    void setIsAuth();

private:
    int _fd;
    std::string _IPadd;
    std::string _nickname;
    std::string _username;
    bool _isAuth ;
    
};


#endif // !CLIENT_HPP

