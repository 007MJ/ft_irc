#include "../includes/utils.hpp"
#include <cstring>
#include <cstdlib>

void errorMsg(const std::string &msg){
    std::string formattedMsg = msg + "\n";
    write(2, formattedMsg.c_str(), formattedMsg.length());
}


bool ValidateAndStoreArgs(char *argv[], int* port_, std::string& pass_){
    {
        for(size_t i = 0; i < std::strlen(argv[1]); i++){
            if(!isdigit(argv[1][i])){
                errorMsg("Error. A non-digit character is detected in port number");
                return false;
            }
        }
        *port_ = std::atoi(argv[1]);
        
    }
    {
        for(size_t i = 0; i < std::strlen(argv[2]); i++){
            if(iswspace(argv[2][i])){
                errorMsg("Error. A white space character is detected in the entered password");
                return false;
            }
        }
        pass_ = argv[2];
    }
    return true;
}



// join chanal 
    // 1.  the user must be invited if the channel is invite-only;
    // 2. the user's nick/username/hostname must not match any active bans;
    // 3. the correct key (password) must be given if it is set.

    // JOIN succesful 
        // sent the channel's topic (using RPL_TOPIC)
        // the list of users who are on the channel (using RPL_NAMREPLY), which must include the user joining


// Chanal mode 
    // 

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
    while (index < irc->getChannel().size()){
        if (usrName == irc->getClients()[index].getNickname() || usrName == irc->getClients()[index].getUsername())
            return index;
        index++;
    }
    return (-1);
}

void EnterRoom(Channel room, std::string topic, Client *nc){
    // std::map<int, Client *> arr = room.getClientChannel();
    // std::map<int, Client *>::iterator it = arr.begin();
    // std::map<int, Client *>::iterator end = arr.end();
    // const void *msg  = room.getTopic();
    // const void *msg = static_cast<void *>(room.getTopic());
    // send(nc->getFd(), room.getTopic(), room.getTopic().size(), 0);
    (void) room;
    (void) topic;                     
    (void) nc;                     
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
                    // display topic 
                    // display usr 
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
        if(it->second->getNickname() == nickname)
            return (1);
        it++;
    }
    return (0);
}


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

unsigned int privmsg(Client *nc, Server *irc){
    context_mode prmsg = nc->getPrivmsg();
    // regarde si c'est channel 
    int roomIndex = getRoom(prmsg.target, irc);
    int roomIndex = getUser(prmsg.target, irc);

    // getRoom(std::string nameRoom, Server *irc)
    // getUser(std::string name, Server *irc)
    return 402;
}


void ActionClient(Client *nc, Server *irc){
    std::cout << "c'est empty" << std::endl;
    if (nc->getTypeCmd() == "JOIN"){
        std::cout << "Enter in JOIN Cmd" << std::endl;
        RoomCheck(nc, irc);
    }
    if (nc->getTypeCmd() == "TOPIC"){
        topic(nc, irc);
    }
    if (nc->getTypeCmd() == "INVITE")
        invite(nc, irc);
    if (nc->getTypeCmd() == "PRIVMSG")
        privmsg(nc, irc);
}

void ClientHandler(std::string msg, Client *nc){
    Commands cmd(msg);
    if (nc)
    {
        std::cout << "ClientHandler function() :" << std::endl;
        nc->setTypeCmd(cmd.get_type_cmd());
            std::cout << "bug 2" << std::endl;
        if (cmd.get_type_cmd() == "JOIN"){
            nc->setJoin(cmd._join());
            // std::cout << "JOIN" << std::endl;
        }
            // std::cout << "bug 3" << std::endl;
        if (cmd.get_type_cmd() == "MODE"){
            nc->setMode(cmd._mode());
            std::cout << "MODE" << std::endl;
        }
        if (cmd.get_type_cmd() == "TOPIC"){
            nc->setTopic(cmd._topic());
            std::cout << "TOPIC" << std::endl;
        }
        if (cmd.get_type_cmd() == "KICK"){
            nc->setKick(cmd._kick());
            std::cout << "KICK" << std::endl;
        }
        if (cmd.get_type_cmd() == "INVITE"){
            nc->setInvinte(cmd._invite ());
            std::cout << "INVITE" << std::endl;
        }
        if (cmd.get_type_cmd() == "PRIVMSG"){
             std::cout << "utils  privmsg " << std::endl;
            nc->setPrivmsg(cmd._privmsg());
        }

    }else 
        std::cout<< "can't get client :" << std::endl;
}