#include "../includes/utils.hpp"
#include <cstring>
#include <cstdlib>




unsigned int invite(Client *nc, Server *irc){
    std::cout << "invite command :" << std::endl;
    std::map<std::string, std::string> arr;
    arr = nc->getInvite();
    std::map<std::string, std::string>::iterator it = arr.begin();
    // std::map<std::string, std::string>::iterator end = arr.end();
    int IsChannel = 0;
    IsChannel = getRoom(it->first, irc);
    if(IsChannel != -1){
        if (irc->getChannel()[IsChannel].getClientChannel().find(nc->getFd()) != irc->getChannel()[IsChannel].getClientChannel().end()){
            if (irc->getChannel()[IsChannel].InviteOnlyModeIsActivated() == true){
                if (irc->getChannel()[IsChannel].IsSuperUser(nc->getFd()))
                {
                    if (isUserChannel(irc->getChannel()[IsChannel].getClientChannel(), it->second) == 0){
                        // irc.getChannel()[IsChannel].AddClient()
                        std::cout << "invite the usr 1" << std::endl;
                    }
                }
            }else{
                if (isUserChannel(irc->getChannel()[IsChannel].getClientChannel(), it->second) == 0){
                    // irc.getChannel()[IsChannel].AddClient()
                    std::cout << "invite the usr 2" << std::endl;
                }
            }
                
        }else
            return std::cout << "the usr is not there" << std::endl,  442;
    }
    return (403);
}

unsigned int topic(Client *nc, Server *irc){
    std::cout << "Topic enter" << std::endl;
    std::vector<std::string> arr;
    arr = nc->getTopic();
    int  IsChannel = getRoom(arr[0], irc);
    if (IsChannel != -1){
        if (irc->getChannel()[IsChannel].getClientChannel().find(nc->getFd()) != irc->getChannel()[IsChannel].getClientChannel().end()){
            if (arr[1] == "")
            {
                std::cout << " the second is empty " << std::endl;
                irc->getChannel()[IsChannel].getTopic();

            }
            else{
                if (irc->getChannel()[IsChannel].TopicModeIsRestricted() == true){
                    if (irc->getChannel()[IsChannel].getSuperUsers().find(nc->getFd()) != irc->getChannel()[IsChannel].getSuperUsers().end()){
                        irc->getChannel()[IsChannel].SetTopic(arr[1]);
                        std::cout << "change the topic 1" << std::endl;
                    }
                }else{
                    irc->getChannel()[IsChannel].SetTopic(arr[1]);
                        std::cout << "change the topic 2 " << std::endl;
                }
            }
        }else
            return 442;
    }else{
        std::cout << "no room for see the Topic" << std::endl;
        std::cout << "channel name"<< arr[0] << std::endl;
    }
    return 403;
}

unsigned int isUserChannel(std::map<int, Client *> clientsChannel, std::string nickname){
    std::map<int, Client*>::iterator it = clientsChannel.begin();
    std::map<int, Client*>::iterator end = clientsChannel.end();
    while (it != end){
        if(it->second->getNickname() == nickname || it->second->getUsername() == nickname)
            return (1);
        it++;
    }
    return (0);
}


void sendMsg(std::string msg, Client *nc, int codeError){
    const void *pmsg = static_cast<const void *>(&msg);
    send(nc->getFd(), pmsg, msg.size(), 0);
    (void)codeError;
}


void sendToChannel(Client *nc, Channel room, std::string msg){
    std::map<int, Client*> arrClient = room.getClientChannel();
    std::map<int, Client*>::iterator it = arrClient.begin();
    std::map<int, Client*>::iterator ite = arrClient.end();
    if (room.IsMember(nc->getFd()))
    {
        while (it != ite){
            sendMsg(msg, it->second, 0);
            it++;
        }
    }
}


void sendToUser(std::string nameClient, Server *irc, std::string msg){
    std::string usrName;
    unsigned int index = 0;
    unsigned int indexUsr = 0;
    while (index < nameClient.size())
    {
        if (nameClient[index] == ':')
            index++;
        else{
            usrName[indexUsr] = nameClient[index];
            index++;
            indexUsr++;
        }
    }
    index = getUser(usrName, irc);
    if (index > 0)
        sendMsg(msg, &irc->getClients()[index], 0);
}

unsigned int privmsg(Client *nc, Server *irc){
    context_mode prmsg = nc->getPrivmsg();
    unsigned index = 0;
    int roomIndex = getRoom(prmsg.target, irc);
    if (roomIndex > -1)
        sendToChannel(nc, irc->getChannel()[roomIndex], prmsg.modestring);
    sendToUser(prmsg.target, irc, prmsg.modestring);
    while (index < prmsg.arguments.size())
    {
        roomIndex = getRoom(prmsg.arguments[index], irc);
        if (roomIndex > -1)
            sendToChannel(nc, irc->getChannel()[roomIndex], prmsg.modestring);
        sendToUser(prmsg.arguments[index], irc, prmsg.modestring);
        index++;
    }
    return 402;
}



int getRoom(std::string nameRoom, Server *irc){
    unsigned int index = 0;
    while (index < irc->getChannel().size()){
        if (nameRoom == irc->getChannel()[index].GetName())
            return index;
        index++;
    }
    return (-1);
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


void RoomCheck(Client *nc, Server *irc){

    std::map<std::string, std::string> arr;
    int indexRoom;
    arr = nc->getJoin();
    std::map<std::string, std::string>::iterator it = arr.begin();
    std::map<std::string, std::string>::iterator end = arr.end();
    while (it != end)
    {
        indexRoom = getRoom(it->first, irc);
        std::cout << " is index room :" << indexRoom << std::endl;
        if (indexRoom != -1){
            if (irc->getChannel()[indexRoom].GetPassword() == it->second){
                if (irc->getChannel()[indexRoom].InviteOnlyModeIsActivated() == false){
                    if (isUserChannel(irc->getChannel()[indexRoom].getClientChannel(), nc->getNickname()) == 0)
                        irc->getChannel()[indexRoom].AddClient(nc);
                    displayRoom(irc->getChannel()[indexRoom]);
                    std::cout << "Display usr" << std::endl;
                }
            }
        }else {
            std::cout << "add the room->name : " << it->first  << " the len " << it->first.size() << std::endl;
            irc->addChannel(it->first, it->second, *nc);
        }
        it++;
    }
    std::cout << "end function " << std::endl;
    // return 0;
}


void displayRoom(Channel room){
    std::map<int, Client *> arr = room.getClientChannel();
    std::map<int, Client *>::iterator it = arr.begin();
    std::map<int, Client *>::iterator end = arr.end();

    while (it != end)
    {
        sendMsg(it->second->getNickname(), it->second, 0);
        sendMsg(room.getTopic(), it->second, 0);
    }
}