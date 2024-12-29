#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "../Client/Client.hpp"
#include <map>
#include <set>

class Channel
{
public:
    Channel(std::string name_, std::string password_, Client& owner_);
    ~Channel();

    const std::string& GetName() const {return _name;}
    void AddClient(Client* client_);
    void RemoveClient(int fd_);
    std::string AddClientAsSuperUser(int fd_);
    void RemoveClientAsSuperUser(int fd_);

    bool IsMember(int fd_);
    bool IsSuperUser(int fd_);

    const std::string& GetPassword() const;
    void SetPassword(const std::string& password_);
    
    const std::string& getTopic() const;
    void SetTopic(const std::string& topic_);

    bool InviteOnlyModeIsActivated() const;
    void SetInviteOnlyModeTo(bool mode_);

    bool TopicModeIsRestricted() const;
    void SetRestrictedTopicModeTo(bool restriction_);

    int getChannelLimit() const;
    void SetChannelLimit(int limit_);

        

private:
    std::string _name;
    std::string _topic;
    std::map<int, Client*> _clients;  // Key: fd, Value: pointer to Client instance
    std::set<int> _superUsers; 
    std::string _password;
    bool _inviteOnly;
    bool _isRestrictedTopic;
    int _limit;
    
};




#endif // !CHANNEL_HPP
