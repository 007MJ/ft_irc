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
        std::string _typeCmd; // ex: JOIN, TOPIC, INVITE
        std::map<std::string, std::string> _join; // ex: {"#foobar", ""} : {"#foo", "password"} 
        std::map<std::string, std::string> _topic; // ex: {"#foobar", ":DescriptionTopic"}
        
        std::map<std::string, std::string> _invite ; // ex: {"#foobar", "username"}
        context_mode _mode; //  std::string target = "#channel"
                            //  std::string modestring = "+o"
                            //  std::vector<std::string> arguments = "userName"
        
        // { "#channel", "+i"}  <target> [<modestring> [<mode arguments>...]]
        context_mode _kick; //  std::string target = "#channel"
                            //  std::string modestring = "+o"
                            //  std::vector<std::string> arguments = "userName"
    
};


#endif // !CLIENT_HPP

/*

∗ TOPIC - Change or view the channel topic
∗ MODE - Change the channel’s mode:
    · i: Set/remove Invite-only channel
    · t: Set/remove the restrictions of the TOPIC command to channel
    operators
    · k: Set/remove the channel key (password)
    · o: Give/take channel operator privilege
    l: Set/remove the user limit to channel

*/
