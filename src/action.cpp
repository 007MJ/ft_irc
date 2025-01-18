#include "../includes/utils.hpp"
#include <cstring>
#include <cstdlib>


int getClient(std::string nameClient, Server *irc){
    std::vector<Client> arrClient = irc->getClients();
    unsigned int index = 0;
    while (index < arrClient.size())
    {
        if (nameClient == arrClient[index].getNickname() || nameClient == arrClient[index].getUsername())
            return (index);
        index++;
    }
    return (-1);
}


unsigned int invite(Client *nc, Server *irc)
{
    std::cout << "invite command :" << std::endl;
    std::map<std::string, std::string> arr;
    arr = nc->getInvite();
    std::map<std::string, std::string>::iterator it = arr.begin();
    // std::map<std::string, std::string>::iterator end = arr.end();
    int isChannel = 0;
    isChannel = getRoomindex(it->first, irc);
    Channel room  = irc->getChannel()[isChannel];
    if(isChannel != -1)
    {
        if (room.InviteOnlyModeIsActivated() == true)
        {
            if (room.IsSuperUser(nc->getFd()))
            {
                    int indexClient = getClient(it->second, irc);
                    if (indexClient > 0)
                    {
                        room.AddClient(&irc->getClients()[indexClient]);
                        std::cout << "invite the usr 1" << std::endl;
                    }
            }
            else
            {
                // if (isUserChannel(room.getClientChannel(), it->second) == 0){
                    // irc.getChannel()[IsChannel].AddClient()
                    std::cout << "invite the usr 2" << std::endl;
            }
        }
    }
    return (403);
}

int pasrinTopic(std::string  topic)
{
    bool dot = false;
    unsigned int index = 0;
    while (index < topic.size())
    {
        if (topic[index] == ':')
            dot = true;
        index++;
    }
    if (dot == true && topic.size() == 1)
        return 2;
    if (dot == true && topic.size() > 1)
        return 1;
    return index;
}

unsigned int topic(Client *nc, Server *irc)
{
    std::vector<std::string> arr;
    arr = nc->getTopic();

    int indexRoom = -1;
    int setTop;
    if (arr.size() > 0)
         indexRoom =  getRoomindex(arr[0], irc);
    if (indexRoom > -1)
    {
        setTop = pasrinTopic(arr[1]);
        Channel room = irc->getChannel()[indexRoom];
        if (room.IsMember(nc->getFd()) && setTop != 0)
        {
            if (room.IsSuperUser(nc->getFd()))
            {
                if (setTop == 2 )
                    room.SetTopic("");
                if (setTop == 1)
                    room.SetTopic(arr[1]);
            }
        }
        else if (setTop == 0)
        {
            void *str = new char [room.getTopic().size() + 1];
            strcpy((char *)str, room.getTopic().c_str());
            send(nc->getFd(), str, room.getTopic().size(), 0);
            free(str);
            std::cout << " see topic " << std::endl;
        }
    }
    return 0;
}


void sendMsg(Client *author, std::string msg, Client *nc, int codeError){

    if (author != NULL)
    {
        if (author->getFd() != nc->getFd())
        {
            void *str = new char [msg.size() + 1];
            strcpy((char *)str, msg.c_str());
            send(nc->getFd(), str, msg.size(), 0);
            free(str);
            (void)codeError;
        }
    }
}


void sendToChannel(Client *nc, Channel room, std::string msg){
    std::map<int, Client*> arrClient = room.getClientChannel();
    std::map<int, Client*>::iterator it = arrClient.begin();
    std::map<int, Client*>::iterator ite = arrClient.end();
    if (room.IsMember(nc->getFd()))
    {
        while (it != ite){
            sendMsg(nc, msg, it->second, 0);
            it++;
        }
    }
}


void sendToUser(Client *author, std::string nameClient, Server *irc, std::string msg)
{
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
        sendMsg(author, msg, &irc->getClients()[index], 0);
}



unsigned int privmsg(Client *nc, Server *irc)
{
    std::cout << " in privmsg fun " << std::endl;
    context_mode prmsg = nc->getPrivmsg();
    unsigned index = 0;
    int roomIndex ;
    sendToUser(nc, prmsg.target, irc, prmsg.modestring);
    while (index < prmsg.arguments.size())
    {
        std::cout << " in privmsg fun " << std::endl;
        roomIndex = getRoomindex(prmsg.arguments[index], irc);
        if (roomIndex > -1)
            sendToChannel(nc, irc->getChannel()[roomIndex], prmsg.modestring);
        sendToUser(nc, prmsg.arguments[index], irc, prmsg.modestring);
        index++;
    }
    return 402;
}



int getRoomindex(std::string nameRoom, Server *irc){
    unsigned int index = 0;
    while (index < irc->getChannel().size())
    {
        if (nameRoom == irc->getChannel()[index].GetName()){
            std::cout <<"arg name : " << nameRoom << " Get->name " << irc->getChannel()[index].GetName() << std::endl;
            return index;
        }
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
        indexRoom = getRoomindex(it->first, irc);
        std::cout << " is index room :" << indexRoom << std::endl;
        if (indexRoom > -1){
            Channel room = irc->getChannel()[indexRoom];
            if (room.GetPassword() == it->second)
            {
                if (room.IsMember(nc->getFd()) == false && room.InviteOnlyModeIsActivated() == false)
                {
                    room.AddClient(nc);
                    displayRoom(nc, room);
                    std::cout << "Display usr" << std::endl;
                }
            }
        }
        else 
        {
            std::cout << "add the room->name : " << it->first  << " the len " << it->first.size() << std::endl;
            irc->addChannel(it->first, it->second, *nc);
        }
        it++;
    }
    std::cout << "end function " << std::endl;
}


void displayRoom(Client *nc , Channel room){
    std::map<int, Client *> arr = room.getClientChannel();
    std::map<int, Client *>::iterator it = arr.begin();
    std::map<int, Client *>::iterator end = arr.end();

    char buffer[BUFFER_SIZE];
    while (it != end)
    {
        strcpy(buffer, (it->second->getNickname() + '\n').c_str());
        send(nc->getFd(), (void *)buffer, it->second->getNickname().size() + 1, 0);
        strcpy(buffer, (room.getTopic() + '\n').c_str());
        send(nc->getFd(), (void *)buffer, room.getTopic().size() + 1, 0);
        it++;
    }
}


void kick(Client *nc, Server *irc)
{
    context_mode obj;
    obj = nc->getKick();
    std::cout<<"Target :" << obj.target << std::endl;
    std::cout<<"Modestring :" << obj.modestring << std::endl;
    int index = 0;
    index = getRoomindex(obj.target, irc);
    int usrIndex = getUser(obj.modestring, irc);
    if (index > -1)
    {
        Channel chann = irc->getChannel()[index];
        if(chann.IsSuperUser(nc->getFd()) && chann.IsMember(usrIndex)){
            chann.RemoveClient(usrIndex);
            std::cout << "Client with index " << usrIndex << " removed!\n";
            return;
        }
        std::cout << "Client with index " << usrIndex << " removed!\n";

    }
}
void mode(Client *nc, Server *irc)
{
    std::cout << "Mode fun" << std::endl;
    context_mode var = nc->getMode();

    std::cout<< "target :" << var.target << std::endl;
    std::cout<< "arg :" << var.modestring << std::endl;
    (void)irc;
}
