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

    }else 
        std::cout<< "can't get client :" << std::endl;
}