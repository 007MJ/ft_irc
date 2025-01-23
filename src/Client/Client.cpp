#include "Client.hpp"
#include "../../includes/utils.hpp"

Client::Client(int fd, const std::string &IPadd_, const std::string &nickname_)
    : _fd(fd), _IPadd(IPadd_), _nickname(nickname_) {
        _username = "";
        _isAuth = false;
        _isIdentified = false;
    }

Client::~Client()
{
    // delete this;
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
    // if(_isAuth){ std::cout << "Client with fd: " << _fd << " is auth" << std::endl;}
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
void Client::setIsIdentified(){
    if(!_isIdentified){
        _isIdentified = true;
        std::string response = ":FT_IRC NOTICE * :Welcome to the server !\r\n";
        clean_send(_fd, response.c_str());
    }
}

void Client::setInfos(bool done){this->_infos = done;}
bool Client::getInfos() {return this->_infos;}

context_mode Client::getPrivmsg() const {return this->_privmsg;}
std::string Client::getTypeCmd() {return this->_typeCmd;}
context_mode Client::getMode() {return this->_mode;}
context_mode Client::getKick() {return this->_kick;}
std::map<std::string, std::string> Client::getJoin() {return this->_join;}
std::vector<std::string> Client::getTopic() {return this->_topic;}
std::map<std::string, std::string> Client::getInvite() { return this->_invite;}

void Client::setJoin(std::map<std::string, std::string> arg) {this->_join = arg;}
void Client::setTopic(std::vector<std::string> arg) {this->_topic = arg;}
void Client::setInvinte(std::map<std::string, std::string> arg) { this->_invite  = arg;}
void Client::setMode(context_mode arg) {this->_mode = arg;}
void Client::setKick(context_mode arg) {this->_kick = arg;}
void Client::setPrivmsg(context_mode arg)  {this->_privmsg = arg;}
void Client::setTypeCmd(std::string type) {this->_typeCmd = type;}