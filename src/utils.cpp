#include "../includes/utils.hpp"
#include "Server/Server.hpp"
#include <cstring>
#include <cstdlib>

void errorMsg(const std::string &msg)
{
    std::string formattedMsg = msg + "\n";
    write(2, formattedMsg.c_str(), formattedMsg.length());
}

bool ValidateAndStoreArgs(char *argv[], int *port_, std::string &pass_)
{
    {
        for (size_t i = 0; i < std::strlen(argv[1]); i++)
        {
            if (!isdigit(argv[1][i]))
            {
                errorMsg("Error. A non-digit character is detected in port number");
                return false;
            }
        }
        *port_ = std::atoi(argv[1]);
    }
    {
        for (size_t i = 0; i < std::strlen(argv[2]); i++)
        {
            if (iswspace(argv[2][i]))
            {
                errorMsg("Error. A white space character is detected in the entered password");
                return false;
            }
        }
        pass_ = argv[2];
    }
    return true;
}

bool HandleConnection(std::string msg_, Client *client_, Server *irc_)
{

    Commands cmd(msg_);
    if (!client_)
        return std::cerr << "Error: Client not found" << std::endl, false;

    client_->setTypeCmd(cmd.get_type_cmd());
    
    if (cmd.get_type_cmd() == "PASS" || cmd.get_type_cmd() == "NICK" || cmd.get_type_cmd() == "USER")
        return irc_->SetClientInfos(&cmd, client_, irc_);

    std::string response = ":" + irc_->getName() + " NOTICE * :Error: Please complete your authentication!\r\n";
    return clean_send(client_->getFd(), response.c_str()), false;
}

bool ClientHandler(std::string msg, Client *nc, Server *irc)
{
    if(!irc->ClientIsIdentified(nc))
    {
        std::cout << "----------------OLÉÉÉ ---------- THIS SHOULD NEVER HAPPEN-------------" << std::endl;
        std::cout << "Client not identified" << std::endl;
        return false;
    }
    Commands cmd(msg);
    if (nc)
    {
        // std::cout << "ClientHandler function() :" << std::endl;
        nc->setTypeCmd(cmd.get_type_cmd());

        // std::cout << "Entered command: PASS" << std::endl;
        if (cmd.get_type_cmd() == "NICK")
        {
            // std::cout << "NICK" << std::endl;
            return irc->SetClientNickName(nc, cmd.get_splitcmds()[1]);
        }
        else if (cmd.get_type_cmd() == "USER")
        {
            std::cout << "USER" << std::endl;
            nc->setUsername(cmd.get_splitcmds()[1]);
        }
        else if (cmd.get_type_cmd() == "JOIN")
        {
            nc->setJoin(cmd._join());
            std::cout << "JOIN" << std::endl;
            RoomCheck(nc, irc);
        }
        else if (cmd.get_type_cmd() == "MODE")
        {
            nc->setMode(cmd._mode());
            std::cout << "MODE" << std::endl;
            mode(nc, irc);
        }
        else if (cmd.get_type_cmd() == "TOPIC")
        {
            std::cout << " TOPIC " << std::endl;
            nc->setTopic(cmd._topic());
            topic(nc, irc);
        }
        else if (cmd.get_type_cmd() == "KICK")
        {
            nc->setKick(cmd._kick());
            kick(nc, irc);
            std::cout << "KICK" << std::endl;
        }
        else if (cmd.get_type_cmd() == "INVITE")
        {
            nc->setInvinte(cmd._invite());
            std::cout << "INVITE" << std::endl;
            invite(nc, irc);
        }
        else if (cmd.get_type_cmd() == "PRIVMSG")
        {
            std::cout << " PRIVMSG " << std::endl;
            nc->setPrivmsg(cmd._privmsg());
            privmsg(nc, irc);
        }
        else
        {
            std::string response = ":server.name NOTICE * :Error: Command not recognized\r\n";
            clean_send(nc->getFd(), response.c_str());
            return false;
        }
    }
    else
    {
        std::string response = ":server.name NOTICE * :Error: Client not found\r\n";
        clean_send(nc->getFd(), response.c_str());
        return false;
    }
    return true;
}

bool clean_recv(int fd_, char *buffer)
{
    memset(buffer, 0, BUFFER_SIZE);
    std::string input;
    int n = recv(fd_, buffer, sizeof(buffer), 0);
    int i = 0;
    i++;
    if (n > 0)
    {
        buffer[n] = '\0'; // Null-terminate the buffer
        input += buffer;  // Append received data to the partial message
    }
    else
    {
        if (n == 0)
            std::cout << "Client " << fd_ << " disconnected\n";
        else
            std::cout << "Error receiving data from client " << fd_ << "\n";
        return false;
    }

    return true;
}

bool clean_recv1(int fd_, std::string &line_)
{
    static std::string buffer; // Retain partial messages
    char temp[BUFFER_SIZE];
    int bytes_read;

    // Attempt to read data from the client
    bytes_read = recv(fd_, temp, sizeof(temp) - 1, 0);
    if (bytes_read > 0)
    {
        temp[bytes_read] = '\0'; // Null-terminate the temporary buffer
        buffer += temp;          // Append new data to the persistent buffer

        // Look for a complete line (support both \r\n and \n)
        size_t pos;
        if ((pos = buffer.find("\r\n")) != std::string::npos)
        {
            line_ = buffer.substr(0, pos); // Extract the complete line
            buffer.erase(0, pos + 2);      // Remove the processed line
            return true;
        }
        else if ((pos = buffer.find("\n")) != std::string::npos)
        {
            line_ = buffer.substr(0, pos); // Extract the complete line
            buffer.erase(0, pos + 1);      // Remove the processed line
            return true;
        }
        return false; // Data received but no complete line yet
    }
    else if (bytes_read == 0)
    {
        std::cerr <<  + " disconnected.\n";
        return false; // Client closed the connection
    }
    else
    {
        std::cerr << "Error receiving data\n" ;
        return false; // Error occurred
    }
}

bool clean_send(int fd_, const char *buff)
{
    if (send(fd_, buff, strlen(buff), 0) < 1)
    {
        std::cerr << "Error while trying to send message to client!" << std::endl;
        return false;
    }
    return true;
}

bool promptForUsername(int fd_, char *buff)
{
    // Step 1: Prompt the client for their username
    if (!clean_send(fd_, "Please enter your username: \n"))
    {
        return false; // Return false if sending the prompt failed
    }
    return clean_recv(fd_, buff);
}

std::string getFullUsername(std::vector<std::string> splittedName_)
{
    std::string fullName = "";

    for (size_t i = 4 /* starts from the 5th arg in the command */; i < splittedName_.size(); i++)
    {
        if (splittedName_[i][0] == ':')
            splittedName_[i].erase(0, 1);
        fullName += splittedName_[i];
        if (i != splittedName_.size() - 1)
            fullName += " ";
    }
    return fullName;
}

bool SendToUser(Client* sender_, Client* receiver_, std::string& msg_){
    std::string response = ":" + sender_->getNickname() + " PRIVMSG " + receiver_->getNickname() + " :" + msg_ + "\r\n";
    return clean_send(receiver_->getFd(), response.c_str());
}
