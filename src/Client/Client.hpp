#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "../Commands/Commands.hpp"
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
        void setJoin(std::map<std::string, std::string> arg);
        void setTopic(std::map<std::string, std::string> arg);
        void setInvinte(std::map<std::string, std::string> arg);
        void setMode(context_mode arg);
        void setKick(context_mode arg);
        void setTypeCmd(std::string type);
        void setIsAuth();

    private:
        int _fd;
        std::string _IPadd;
        std::string _nickname;
        std::string _username;
        bool _isAuth ;
        // cmd of the clients 
        std::string _typeCmd;
        std::map<std::string, std::string> _join;
        std::map<std::string, std::string> _topic;
        std::map<std::string, std::string> _invinte;
        context_mode _mode;
        context_mode _kick;
    
};


#endif // !CLIENT_HPP

