#include "../includes/utils.hpp"
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>
#include <iostream>
#include "Client/Client.hpp"
#include "Server/Server.hpp"
#include "Channel/Channel.hpp"
#include "Replis.hpp"

int getClient(const std::string& nameClient,  Server* irc) {
    const std::vector<Client> arrClient = irc->getClients();
    for (size_t index = 0; index < arrClient.size(); ++index) {
        if (nameClient == arrClient[index].getNickname()) {
            return static_cast<int>(index);
        }
    }
    return -1;
}

int getUser(std::string usrName, Server *irc){
    unsigned int index = 0;
    while (index < irc->getClients().size()){
        if (usrName == irc->getClients()[index].getNickname() || usrName == irc->getClients()[index].getUsername())
            return index;
        index++;
    }
    return (-1);
}

unsigned int invite(Client* nc, Server* irc) {
    std::cout << "invite command:" << std::endl;
    const std::map<std::string, std::string>& arr = nc->getInvite();
    std::map<std::string, std::string>::const_iterator it = arr.begin();
    Replis msgReplis;
    Channel* room = nullptr;
    while (it != arr.end()) 
    {
        const std::string namechannel(it->first);
        room = irc->getChannelByName(namechannel);
        if (room == NULL) {
            std::cout << "Channel not found: " << it->first << std::endl;
            return 404; // Not Found
        }

        if (room->InviteOnlyModeIsActivated()) 
        {
            if (room->IsSuperUser(nc->getFd())) 
            {
                int indexClient = getClient(it->second, irc);
                if (indexClient >= 0) 
                {
                    if (room->getClientChannel().size() <  (size_t)room->getChannelLimit())
                        room->AddClient(&irc->getClients()[indexClient]);
                    std::cout << "Invited user: " << it->second << std::endl;
                    std::cout << "Invited name client " << irc->getClients()[indexClient].getNickname() << std::endl;
                } else 
                {
                    std::cout << "User not found: " << it->second << std::endl;
                }
            } else 
            {
                std::cout << "Permission denied. User is not a superuser." << std::endl;
                std::string replisInvite = msgReplis.RPL_NOPRIVILEGES(nc->getNickname());
                send(nc->getFd(),replisInvite.c_str(), replisInvite.size(), 0);
            }
        } else {
            std::cout << "Channel is not invite-only." << std::endl;
            std::string replisInvite = msgReplis.RPL_INVITEONLY(nc->getNickname(), room->GetName());
            send(nc->getFd(), replisInvite.c_str(), replisInvite.size(), 0);
        }
        ++it;
    }
    return 200; // Success
}


int pasrinTopic(const std::string& topic) {
    if (topic.empty()) return 0;

    bool hasColon = false;
    for (size_t index = 0; index < topic.size(); ++index) {
        if (topic[index] == ':') {
            hasColon = true;
            break;
        }
    }

    if (hasColon) {
        return (topic.size() == 1) ? 2 : 1;
    }
    return 0;
}

unsigned int topic(Client* nc, Server* irc) {
    const std::vector<std::string>& arr = nc->getTopic();
    if (arr.empty()) {
        std::cout << "Error: No channel specified for TOPIC." << std::endl;
        return 400; // Bad Request
    }
    Replis msgReplis;

    Channel* room = irc->getChannelByName(arr[0]);
    int setTop = pasrinTopic(arr[1]);
    
    if (setTop == 0) {
        std::string replisTopic = msgReplis.RPL_TOPIC(nc->getNickname(), room->GetName(), room->getTopic());
        send(nc->getFd(), replisTopic.c_str(), replisTopic.size(), 0);
        std::cout << "Viewing topic: " << room->getTopic() << std::endl;
    } else if (room->TopicModeIsRestricted() && room->IsSuperUser(nc->getFd())) {
        room->SetTopic(setTop == 2 ? "" : arr[1]);
        std::cout << "Topic updated to: " << room->getTopic() << std::endl;
    } else if (!room->TopicModeIsRestricted()) {
        room->SetTopic(setTop == 2 ? "" : arr[1]);
        std::cout << "Topic updated to: " << room->getTopic() << std::endl;
    } else {
        std::cout << "Permission denied. User is not a superuser." << std::endl;
    }
    return 200;
}

void sendMsg(std::string& msg, Client* nc, int codeError) {
    send(nc->getFd(), msg.c_str(), msg.size(), 0);
    (void)codeError;
}

void sendToChannel(Channel& room, std::string msg, int author) {
    const std::map<int, Client*>& clients = room.getClientChannel();
    std::map<int, Client*>::const_iterator it = clients.begin();

    while (it != clients.end()) {
        if (it->first != author)
            sendMsg(msg, it->second, 0);
        ++it;
    }
}

void sendToUser(Client* author, std::string& nameClient, Server* irc,  std::string& msg) {
    int userIndex = getUser(nameClient, irc);
    if (userIndex >= 0 && irc->getClients()[userIndex].getFd() != author->getFd()) {
        sendMsg(msg, &irc->getClients()[userIndex], 0);
    } else {
        std::cout << "Error: Cannot send message to user: " << nameClient << std::endl;
    }
}

void kick(Client* nc, Server* irc) {
    context_mode obj = nc->getKick();
    // int index = getRoomindex(obj.target, irc);
    int usrIndex = getUser(obj.modestring, irc);

    if ( usrIndex == -1) {
        std::cout << "Error: Channel or user not found." << std::endl;
        return;
    }

    std::string const nameTagrget(obj.target);
    Channel* room = irc->getChannelByName(nameTagrget);
    if (room->IsSuperUser(nc->getFd())) {
        // irc->getClients()[usrIndex].getFd()
        room->RemoveClient(irc->getClients()[usrIndex].getFd());
        if (room->getClientChannel().size() == 0)
            irc->getChannel().erase(irc->getChannel().begin() + usrIndex);
        std::cout << "User " << usrIndex << " kicked from channel " << room->GetName() << "." << std::endl;
    } else {
        std::cout << "Permission denied. User is not a superuser." << std::endl;
    }
}

void privmsg(Client* nc, Server* irc) {
    std::cout << "in privmsg function" << std::endl;
    context_mode prmsg = nc->getPrivmsg();
    unsigned int index = 0;

    while (index < prmsg.arguments.size()) 
    {
        std::cout << "Sending message to channel" << std::endl;
        Channel *room = irc->getChannelByName(prmsg.arguments[index]);
        if (room) 
            sendToChannel(*irc->getChannelByName(prmsg.arguments[index]), prmsg.modestring, nc->getFd());
        else
            sendToUser(nc, prmsg.arguments[index], irc, prmsg.modestring);
        ++index;
    }

}

void RoomCheck(Client *nc, Server *irc) {
    // Récupère les canaux que le client veut rejoindre
    std::map<std::string, std::string> arr = nc->getJoin();
    if (arr.empty()) {
        return;
    }

    Replis msgRplis;

    // Itération explicite sur la map avec std::map::iterator
    std::map<std::string, std::string>::iterator it = arr.begin();
    for (; it != arr.end(); ++it) {
        Channel *room = irc->getChannelByName(it->first); // Pointeur brut utilisé à la place de shared_ptr
        if (!room) {
            // Crée un nouveau channel si inexistant
            irc->addChannel(it->first, it->second, *nc);
            continue;
        }

        const std::map<int, Client *> &clients = room->getClientChannel();
        if (room->GetPassword() == it->second && !room->IsMember(nc->getFd())) {
            if (room->InviteOnlyModeIsActivated() && clients.size() < static_cast<size_t>(room->getChannelLimit())) {
                room->AddClient(nc);

                std::map<int, Client *>::const_iterator clientIt = clients.begin();
                for (; clientIt != clients.end(); ++clientIt) {
                    std::string userList = clientIt->second->getNickname();
                    std::string endOfNames = msgRplis.RPL_ENDOFNAMES(userList, room->GetName());
                    std::cout << " userList "<< endOfNames << std::endl;
                    send(nc->getFd(), endOfNames.c_str(), endOfNames.size(), 0);
                }
                // std::cout << "userList of channel "<<  msgRplis.RPL_ENDOFNAMES(userList, room->GetName())<< std::endl;
                sendToChannel(*room, msgRplis.NOTIFCHANNEL(nc->getNickname(), nc->getUsername(), room->GetName()), nc->getFd());
            }
        }
    }
}

void modeOption(Channel& room, const std::string& opt, const std::string& data) {
    std::cout << " data "<<  data << std::endl;
    std::cout << "opt "<<  opt << std::endl;
    if (opt == "+i" ||  opt == "-i") {
        room.SetInviteOnlyModeTo(opt[0] != '-');
    } else if (opt == "+t" || opt == "-t") {
        room.SetRestrictedTopicModeTo(opt[0] != '-');
    } else if (opt == "+k" || opt == "-k") {
        room.SetPassword(opt[0] == '-' ? "" : data);
    } else if (opt == "+o" || opt == "-o") {
        const std::map<int, Client *> clients = room.getClientChannel();
        std::map<int, Client *>::const_iterator it = clients.begin();
        while (it != clients.end()) {
            if (it->second->getNickname() == data) {
                if (opt[0] == '-') {
                    room.RemoveClientAsSuperUser(it->second->getFd());
                } else {
                    room.AddClientAsSuperUser(it->second->getFd());
                }
                break;
            }
            ++it;
        }
    } else if (opt == "+l" ||  opt == "-l") {
        room.SetChannelLimit(opt[0] == '-' ? 0 : std::atoi(data.c_str()));
    }
}

void mode(Client* nc, Server* irc) {
    std::cout << "Mode command:" << std::endl;
    context_mode var = nc->getMode();
    // int indexRoom = getRoomindex(var.target, irc);
    Replis replisMsg;
    // if (indexRoom == -1) {
        // std::cout << "Error: Channel not found." << std::endl;
        // return;
    // }

    Channel* room = irc->getChannelByName(var.target);
    if (var.modestring.size() == 2) {
        std::string data = var.arguments.empty() ? "" : var.arguments[0];
        if (room && room->IsSuperUser(nc->getFd()))
                modeOption(*room, var.modestring, data);
        else{
            std::string replispriv =  replisMsg.RPL_NOPRIVILEGES(nc->getNickname());
            send(nc->getFd(), replispriv.c_str(), replispriv.size(), 0);
        }

    } else {
        std::cout << "Error: Invalid mode string." << std::endl;
    }
}