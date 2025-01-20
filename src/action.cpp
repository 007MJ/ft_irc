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


void channelSend(std::string msg, Client *nc, int codeError){

        void *str = new char [msg.size() + 1];
        strcpy((char *)str, msg.c_str());
        send(nc->getFd(), str, msg.size(), 0);
        free(str);
        (void)codeError;

}

void sendMsg(std::string msg, Client *nc, int codeError)
{
    void *str = new char [msg.size() + 1];
    strcpy((char *)str, msg.c_str());
    send(nc->getFd(), str, msg.size(), 0);
    free(str);
    (void)codeError;
}


void sendToChannel(Channel room, std::string msg){
    std::map<int, Client*> arrClient = room.getClientChannel();
    std::map<int, Client*>::iterator it = arrClient.begin();
    std::map<int, Client*>::iterator ite = arrClient.end();
    // if (room.IsMember(nc->getFd()))
    // {
        std::cout << "Send message to the room :" << std::endl;
        while (it != ite)
        {
            std::cout << "CLient:"<< it->second->getFd() << std::endl;
            channelSend(msg, it->second, 0);
            it++;
        }
    // }
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
    {
        std::cout << " send User message " << std::endl;
        if (author->getFd() != irc->getClients()[index].getFd())
            sendMsg(msg, &irc->getClients()[index], 0);
    }
}



unsigned int privmsg(Client *nc, Server *irc)
{
    std::cout << " in privmsg fun " << std::endl;
    context_mode prmsg = nc->getPrivmsg();
    unsigned index = 0;
    int roomIndex ;
    while (index < prmsg.arguments.size())
    {
        std::cout << " in privmsg fun arg"<< prmsg.arguments[index] << std::endl;
        roomIndex = getRoomindex(prmsg.arguments[index], irc);
        if (roomIndex > -1)
        {
            std::cout<< "Send mesg channl "<< std::endl;
            sendToChannel(irc->getChannel()[roomIndex], prmsg.modestring);
        }
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
        if (indexRoom > -1)
        {
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
            // add super as superUser();
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
        if(chann.IsSuperUser(nc->getFd()) && chann.IsMember(usrIndex))
        {
            chann.RemoveClient(usrIndex);
            std::cout << "Client with index " << usrIndex << " removed!\n";
            return;
        }
        std::cout << "Client with index " << usrIndex << " removed!\n";

    }
}
void modeOption(Channel room, std::string opt, std::string data)
{
    if (opt == "+i" || opt == "-i")
    {
        std::cout << " mode Option i+ or -i" << std::endl;
        if (opt[0] == '-')
            room.SetInviteOnlyModeTo(false);
        else
            room.SetInviteOnlyModeTo(true);
    }
    if (opt == "+t" || opt == "-t")
    {
        std::cout << " mode Option +t or -t" << std::endl;
        if (opt[0] == '-')
            room.SetRestrictedTopicModeTo(false);
        else
            room.SetRestrictedTopicModeTo(true);
    }
    if (opt == "+k" || opt == "-k")
    {
        std::cout << " mode Option +k or -k" << std::endl;
        if (opt[0] == '-')
            room.SetPassword("");
        else
            room.SetPassword(data);
    }
    if (opt == "+o" || opt == "-o")
    {
        std::cout << " mode Option +o or -o" << std::endl;
        std::map<int, Client *> arrClients = room.getClientChannel();
        std::map<int, Client *>::iterator it = arrClients.begin();
        std::map<int, Client *>::iterator end =  arrClients.end();
        while (it != end)
        {
            if (it->second->getNickname() == data || it->second->getUsername() == data)
                break;
            it++;
        }
        if (it != end)
        {
            if (opt[0] == '-')
                room.AddClientAsSuperUser(it->second->getFd());
            else
                room.RemoveClientAsSuperUser(it->second->getFd());
        }
    }
    if (opt == "+l" || opt == "-l")
    {
        std::cout << " mode Option +l or -l" << std::endl;
        if (opt[0] == '-')
            room.SetChannelLimit(1);
        else
            room.SetChannelLimit(atoi(data.c_str()));
    }
}
void mode(Client *nc, Server *irc)
{
    std::cout << "Mode fun" << std::endl;
    context_mode var = nc->getMode();

    int indexRoom = getRoomindex(var.target, irc);
    if (indexRoom > -1)
    {
        Channel room = irc->getChannel()[indexRoom];
        // if (room.IsSuperUser(nc->getFd()))
        // {
            std::cout << "Room :"<< var.target << std::endl;
            std::cout << "Mode :"<< var.modestring << std::endl;
            if (var.modestring.size() == 2)
            {
                if (var.arguments.size() == 0)
                    var.arguments[0] = "";
                modeOption(room, var.modestring, var.arguments[0]);
            }
        // }        
    }
}