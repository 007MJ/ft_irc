#include "../includes/utils.hpp"
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>
#include <iostream>
#include "Client/Client.hpp"
#include "Server/Server.hpp"
#include "Channel/Channel.hpp"

int getClient(const std::string &nameClient, Server *irc)
{
    const std::vector<Client> arrClient = irc->getClients();
    for (size_t index = 0; index < arrClient.size(); ++index)
    {
        if (nameClient == arrClient[index].getNickname())
        {
            return static_cast<int>(index);
        }
    }
    return -1;
}

int getUser(std::string usrName, Server *irc)
{
    unsigned int index = 0;
    while (index < irc->getClients().size())
    {
        if (usrName == irc->getClients()[index].getNickname() || usrName == irc->getClients()[index].getUsername())
            return index;
        index++;
    }
    return (-1);
}

unsigned int invite(Client *nc, Server *irc)
{
    std::cout << "invite command:" << std::endl;
    const std::map<std::string, std::string> &arr = nc->getInvite();
    std::map<std::string, std::string>::const_iterator it = arr.begin();

    Channel *room = nullptr;
    while (it != arr.end())
    {
        const std::string namechannel(it->first);
        room = irc->getChannelByName(namechannel);
        if (room == NULL)
        {
            std::cout << "Channel not found: " << it->first << std::endl;
            return 404; 
        }

        if (room->InviteOnlyModeIsActivated())
        {
            if (room->IsSuperUser(nc->getFd()))
            {
                int indexClient = getClient(it->second, irc);
                if (indexClient >= 0)
                {
                    if (room->getClientChannel().size() < (size_t)room->getChannelLimit())
                        room->AddClient(&irc->getClients()[indexClient]);
                    std::cout << "Invited user: " << it->second << std::endl;
                    std::cout << "Invited name client " << irc->getClients()[indexClient].getNickname() << std::endl;
                    std::string response = ":" + nc->getNickname() + " INVITE " + irc->getClients()[indexClient].getNickname()+ " :" + room->GetName() + "\r\n";
                    clean_send(nc->getFd(), response.c_str());
                }
                else
                {
                    std::string response = ":FT_IRC 401 " + nc->getNickname() + " " + irc->getClients()[indexClient].getNickname() + " :No such nick/channel\r\n";
                    clean_send(nc->getFd(), response.c_str());
                }
            }
            else
            {
                std::string response = ":FT_IRC 482 " + nc->getNickname() + " " + room->GetName() + " :You're not channel operator\r\n";
                clean_send(nc->getFd(), response.c_str());

            }
        }
        else
        {
            std::string response = ":FT_IRC 482 " + nc->getNickname() + " " + room->GetName() + " :Channel is not in invite-only mode\r\n";
            clean_send(nc->getFd(), response.c_str());
        }
        ++it;
    }
    return 200; // Success
}

int pasrinTopic(const std::string &topic)
{
    if (topic.empty())
        return 0;

    bool hasColon = false;
    for (size_t index = 0; index < topic.size(); ++index)
    {
        if (topic[index] == ':')
        {
            hasColon = true;
            break;
        }
    }

    if (hasColon)
    {
        return (topic.size() == 1) ? 2 : 1;
    }
    return 0;
}

unsigned int topic(Client *nc, Server *irc)
{
    const std::vector<std::string> &arr = nc->getTopic();
    if (arr.empty())
    {
        std::cout << "Error: No channel specified for TOPIC." << std::endl;
        return 400;
    }

    // int indexRoom = getRoomindex(arr[0], irc);
    // if (indexRoom == -1) {
    // std::cout << "Channel not found: " << arr[0] << std::endl;
    // return 404; // Not Found
    // }

    Channel *room = irc->getChannelByName(arr[0]);
    if (!room)
    {
        std::string response = ":FT_IRC 403 " + nc->getNickname() + " " + arr[0] + " :No such channel\r\n";
        clean_send(nc->getFd(), response.c_str());
        return 1;
    }
    int setTop = pasrinTopic(arr[1]);
    //
    if (setTop == 0)
    {
        std::string response = ""; 
        if(room->getTopic().empty()){
            response = ":FT_IRC 331 " + nc->getNickname() + " " + room->GetName() + " :No topic is set\r\n";
            clean_send(nc->getFd(), response.c_str());
            return 1;
        }
        // sendMsg("Topic: " + room->getTopic(), nc, 0);
        std::cout << "Viewing topic: " << room->getTopic() << std::endl;
        response = ":FT_IRC 332 " + nc->getNickname() + " " + room->GetName() + " :" + room->getTopic() + "\r\n";
        clean_send(nc->getFd(), response.c_str());

    }
    else if (room->TopicModeIsRestricted() && room->IsSuperUser(nc->getFd()))
    {
        room->SetTopic(setTop == 2 ? "" : arr[1]);
        std::cout << "Topic updated to: " << room->getTopic() << std::endl;
        std::string response = ":FT_IRC 332 " + nc->getNickname() + " " + room->GetName() + " :" + room->getTopic() + "\r\n";
        clean_send(nc->getFd(), response.c_str());
    }
    else if (!room->TopicModeIsRestricted())
    {
        room->SetTopic(setTop == 2 ? "" : arr[1]);
        std::cout << "Topic updated to: " << room->getTopic() << std::endl;
         std::string response = ":FT_IRC 332 " + nc->getNickname() + " " + room->GetName() + " :" + room->getTopic() + "\r\n";
        clean_send(nc->getFd(), response.c_str());
    }
    else
    {
        std::cout << "Permission denied. User is not a superuser." << std::endl;
        std::string response = ":FT_IRC 482 " + nc->getNickname() + " " + room->GetName() + " :You're not channel operator\r\n";
        clean_send(nc->getFd(), response.c_str());

    }
    return 200; // Success
}

void sendMsg(const std::string &msg, Client *nc, int codeError)
{
    send(nc->getFd(), msg.c_str(), msg.size(), 0);
    (void)codeError; // Suppress unused variable warning
}

void sendToChannel(Channel &room, const std::string &msg)
{
    const std::map<int, Client *> &clients = room.getClientChannel();
    std::map<int, Client *>::const_iterator it = clients.begin();

    while (it != clients.end())
    {
        sendMsg(msg, it->second, 0);
        // std::string response = ":" + nc->getNickname() + " KICK " + roomName + " " + irc->getClients()[usrIndex].getNickname() + "\r\n";
        // clean_send(nc->getFd(), response.c_str());
        ++it;
    }
}

void sendToUser(Client *author, std::string &nameClient, Server *irc, std::string &msg)
{
    int userIndex = getUser(nameClient, irc);
    if (userIndex >= 0 && irc->getClients()[userIndex].getFd() != author->getFd())
    {
        std::string response = ":" + author->getNickname() + " PRIVMSG " + irc->getClients()[userIndex].getNickname() + " :" + msg + "\r\n";
        clean_send(author->getFd(), response.c_str());
    }
    else
    {
        std::cout << "index user  " << userIndex << "\n";
        std::cout << "name Client " << nameClient << "\n";
        std::string response = ":FT_IRC 401 " + author->getNickname() + " " + nameClient + " :No such nick/channel\r\n";
        clean_send(author->getFd(), response.c_str());
    }
}

void kick(Client *nc, Server *irc)
{
    context_mode obj = nc->getKick();
    // int index = getRoomindex(obj.target, irc);
    int usrIndex = getUser(obj.modestring, irc);

    if (usrIndex == -1)
    {
        std::cout << "Error: Channel or user not found." << std::endl;
        return;
    }

    std::string const nameTagrget(obj.target);
    Channel *room = irc->getChannelByName(nameTagrget);
    if(!room){
        std::cerr << "Channel not found\n";
        return;
    }
    std::string roomName = room->GetName();
    if (room->IsSuperUser(nc->getFd()))
    {
        room->RemoveClient(irc->getClients()[usrIndex].getFd());
        if (room->getClientChannel().empty()){
            irc->deleteChannel(irc->getChannelIndex(roomName));
            return ;
        }
        std::string response = ":" + nc->getNickname() + " KICK " + roomName + " " + irc->getClients()[usrIndex].getNickname() + "\r\n";
        clean_send(nc->getFd(), response.c_str());
        room->SendToChannel(nc, response);
        std::string reskick = ":" + nc->getNickname() + "!" + nc->getUsername() + "@FT_IRC KICK "+ roomName + " "+ irc->getClients()[usrIndex].getNickname() + "\r\n";
        clean_send(irc->getClients()[usrIndex].getFd(), reskick.c_str());
    }
    else
    {
        std::string response = ":FT_IRC 482 " + nc->getNickname() + " " + roomName + " :You're not channel operator\r\n";
        clean_send(nc->getFd(), response.c_str());

    }
}

void privmsg(Client *nc, Server *irc)
{
    std::cout << "in privmsg function" << std::endl;
    context_mode prmsg = nc->getPrivmsg();
    unsigned int index = 0;

    std::string response = "";
    while (index < prmsg.arguments.size())
    {
        std::cout << "Sending message to channel" << std::endl;
        if (prmsg.arguments[index][0] && prmsg.arguments[index][0] == '#'){
            Channel *room = irc->getChannelByName(prmsg.target);
            if(!room){
                 std::string response = ":FT_IRC 401 " + nc->getNickname() + " " + prmsg.target + " :No such nick/channel\r\n";
                clean_send(nc->getFd(), response.c_str());  
                return ;
            }
            std::string response = ":" + nc->getNickname() + " PRIVMSG " + room->GetName() + " :" + prmsg.arguments[index] + "\r\n";
            room->SendToChannel(nc, response);

        }
        else
        {
            sendToUser(nc, prmsg.target, irc, prmsg.arguments[index]);
        }
        std::cout << "prmsg.arguments[index]" << prmsg.arguments[index] + "\n";
        std::cout << "prmsg.modestring" << prmsg.modestring + "\n";
        std::cout << "prmsg.tartget" << prmsg.target + "\n";
        std::cout << "prmsg.modestring" << prmsg.modestring + "\n";
        ++index; 
    }
}

void RoomCheck(Client *nc, Server *irc)
{

    std::map<std::string, std::string> arr = nc->getJoin();
    std::map<std::string, std::string>::iterator it = arr.begin();

    Channel *room = nullptr;
    while (it != arr.end())
    {
        room = irc->getChannelByName(it->first);
        if (room != nullptr)
        {
            if (!room->IsMember(nc->getFd()))
            {
                std::cout << "is invite true: " << (room->InviteOnlyModeIsActivated()) << std::endl;
                std::cout << "-channel size: " << (room->getClientChannel().size()) << std::endl;
                std::cout << "-limite channel " << (size_t)room->getChannelLimit() << std::endl;
                if(room->GetPassword() != it->second)
                {
                    std::string response = ":FT_IRC 475 " + nc->getNickname() + " " + room->GetName() + " :Cannot join channel (+k)\r\n";
                    clean_send(nc->getFd(), response.c_str());
                    break;
                }
                if (room->InviteOnlyModeIsActivated())
                {
                    std::cout << "InviteOnlyModeIsActivated for " + room->GetName() << std::endl;
                    std::string response = ":FT_IRC 473 " + nc->getNickname() + " " + room->GetName() + " :Cannot join channel (+i)\r\n";
                    clean_send(nc->getFd(), response.c_str());
                    break;
                }
                if (room->getClientChannel().size() >= (size_t)room->getChannelLimit())
                {
                    std::string response = ":FT_IRC 471 " + nc->getNickname() + " " + room->GetName() + " :Cannot join channel (+l)\r\n";
                    clean_send(nc->getFd(), response.c_str());
                    break;
                }
                room->AddClient(nc);
            }
        }
        else
            irc->addChannel(it->first, it->second, *nc);
        it++;
    }
}

void modeOption(Channel &room, const std::string &opt, const std::string &data, Client *nc)
{
    // std::cout << " data " << data << std::endl;err
    if(!nc){
        std::cerr << "Client not found!\n";
        return;
    }
    if (opt == "+i" || opt == "-i")
    {
        room.SetInviteOnlyModeTo(opt[0] != '-');
        if (opt[0] == '-')
        {
            std::string response = ":FT_IRC MODE " + room.GetName() + " -i\r\n";
            clean_send(nc->getFd(), response.c_str());
        }else{
            std::string response = ":FT_IRC MODE " + room.GetName() + " +i\r\n";
            clean_send(nc->getFd(), response.c_str());
        }
    }
    else if (opt == "+t" || opt == "-t")
    {
        room.SetRestrictedTopicModeTo(opt[0] != '-');
        if (opt[0] == '-')
        {
            std::string response = ":FT_IRC MODE " + room.GetName() + " -t\r\n";
            clean_send(nc->getFd(), response.c_str());
        }else{
            std::string response = ":FT_IRC MODE " + room.GetName() + " +t\r\n";
            clean_send(nc->getFd(), response.c_str());
        }
    }
    else if (opt == "+k" || opt == "-k")
    {
        room.SetPassword(opt[0] == '-' ? "" : data);
         if (opt[0] == '-')
        {
            std::string response = ":FT_IRC MODE " + room.GetName() + " -k\r\n";
            clean_send(nc->getFd(), response.c_str());
        }else{
            std::string response = ":FT_IRC MODE " + room.GetName() + " +k\r\n";
            clean_send(nc->getFd(), response.c_str());
        }
    }
    else if (opt == "+o" || opt == "-o")
    {
        const std::map<int, Client *> clients = room.getClientChannel();
        std::map<int, Client *>::const_iterator it = clients.begin();
        while (it != clients.end())
        {
            if (it->second->getNickname() == data)
            {
                if (opt[0] == '-')
                {
                    room.RemoveClientAsSuperUser(it->second->getFd());
                }
                else
                {
                    room.AddClientAsSuperUser(it->second->getFd());
                }
                break;
            }
            ++it;
        }
        if (opt[0] == '-')
        {
            std::string response = ":FT_IRC MODE " + room.GetName() + " -o\r\n";
            clean_send(nc->getFd(), response.c_str());
        }else{
            std::string response = ":FT_IRC MODE " + room.GetName() + " +o\r\n";
            clean_send(nc->getFd(), response.c_str());
        }
    }
    else if (opt == "+l" || opt == "-l")
    {
        room.SetChannelLimit(opt[0] == '-' ? 0 : std::atoi(data.c_str()));
        if (opt[0] == '-')
        {
            std::string response = ":FT_IRC MODE " + room.GetName() + " -l\r\n";
            clean_send(nc->getFd(), response.c_str());
        }else{
            std::string response = ":FT_IRC MODE " + room.GetName() + " +l\r\n";
            clean_send(nc->getFd(), response.c_str());
        }
    }
}

void mode(Client *nc, Server *irc)
{
    std::cout << "Mode command:" << std::endl;
    context_mode var = nc->getMode();
  
    Channel *room = irc->getChannelByName(var.target);
    if(!room){
        std::cerr << "Room not found\n";
        std::string response = ":FT_IRC 403 " + nc->getNickname() + " " + var.target + " :No such channel\r\n";
        clean_send(nc->getFd(), response.c_str());
        return;
    }
    if (var.modestring.size() == 2)
    {
        std::string data = var.arguments.empty() ? "" : var.arguments[0];
        if (room && room->IsSuperUser(nc->getFd()))
            modeOption(*room, var.modestring, data, nc);
        else
        {
            std::string response = ":FT_IRC 482 " + nc->getNickname() + " " + room->GetName() + " :You're not channel operator\r\n";
            clean_send(nc->getFd(), response.c_str());
            
        }
    }
    else
    {
        std::cout << "Error: Invalid mode string." << std::endl;
    }
}