#include "../includes/utils.hpp"
#include "Server/Server.hpp"
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





void ClientHandler(std::string msg, Client *nc, Server *irc){
    Commands cmd(msg);
    if (nc)
    {
        std::cout << "ClientHandler function() :" << std::endl;
        nc->setTypeCmd(cmd.get_type_cmd());
            std::cout << "bug 2" << std::endl;
        if (cmd.get_type_cmd() == "JOIN"){
            nc->setJoin(cmd._join());
            std::cout << "JOIN" << std::endl;
            RoomCheck(nc, irc);
        }
            // std::cout << "bug 3" << std::endl;
        if (cmd.get_type_cmd() == "MODE"){
            nc->setMode(cmd._mode());
            std::cout << "MODE" << std::endl;
            mode(nc, irc);
        }
        if (cmd.get_type_cmd() == "TOPIC"){
            std::cout << " TOPIC " << std::endl;
            nc->setTopic(cmd._topic());
            topic(nc, irc);
        }
        if (cmd.get_type_cmd() == "KICK"){
            nc->setKick(cmd._kick());
            kick(nc, irc);
            std::cout << "KICK" << std::endl;
        }
        if (cmd.get_type_cmd() == "INVITE"){
            nc->setInvinte(cmd._invite ());
            std::cout << "INVITE" << std::endl;
            invite(nc, irc);
        }
        if (cmd.get_type_cmd() == "PRIVMSG"){
             std::cout << " PRIVMSG " << std::endl;
             nc->setPrivmsg(cmd._privmsg());
            privmsg(nc, irc);
        }

    }else 
        std::cout<< "can't get client :" << std::endl;
}




bool clean_recv(int fd_, char *buffer){
    int n = recv(fd_, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0)
    {
        if (n == 0)
            std::cout << "Client " << fd_ << " disconnected\n";
        else
            std::cout << "Error receiving data from client " << fd_ << "\n";
        return false; // Disconnect the client
    }
    buffer[n] = '\0'; // Null-terminate input
    return true;
}
bool clean_send(int fd_, const char *buff){
    if(send(fd_, buff, strlen(buff), 0 ) < 1){
        std::cerr << "Error while trying to send message to client!" << std::endl;
        return false;
    }
    return true;
}

bool promptForUsername(int fd_, char *buff){
    // Step 1: Prompt the client for their username
    if (!clean_send(fd_, "Please enter your username: \n")) {
        return false; // Return false if sending the prompt failed
    }
    return clean_recv(fd_, buff);
}