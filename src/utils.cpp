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
            std::cout << "JOIN" << std::endl;
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