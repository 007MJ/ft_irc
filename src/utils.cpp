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



void ClientHandler(std::string msg, Client *nc, Server *irc)
{
    Commands cmd(msg);
    if (nc)
    {
        std::cout << "ClientHandler function() :" << std::endl;
        nc->setTypeCmd(cmd.get_type_cmd());
        // std::cout << "/////////////cmd.get_type_cmd() ://////////////" << cmd.get_type_cmd() << std::endl;

        if(cmd.get_type_cmd() == "PASS")
        {
            std::cout << "Entered command: PASS" << std::endl;
            irc->AuthClient(nc, cmd.get_splitcmds()[1]);
        }
        if (cmd.get_type_cmd() == "NICK")
        {
            std::cout << "NICK" << std::endl;
            nc->setNickname(cmd.get_splitcmds()[1]);
            std::cout << "Nickname after setting: " << nc->getNickname() << std::endl;
        }
        if (cmd.get_type_cmd() == "USER")
        {
            std::cout << "USER" << std::endl;
            nc->setUsername(cmd.get_splitcmds()[1]);
        }
        if (cmd.get_type_cmd() == "JOIN")
        {
            nc->setJoin(cmd._join());
            std::cout << "JOIN" << std::endl;
            RoomCheck(nc, irc);
        }
        // std::cout << "bug 3" << std::endl;
        if (cmd.get_type_cmd() == "MODE")
        {
            nc->setMode(cmd._mode());
            std::cout << "MODE" << std::endl;
            mode(nc, irc);
        }
        if (cmd.get_type_cmd() == "TOPIC")
        {
            std::cout << " TOPIC " << std::endl;
            nc->setTopic(cmd._topic());
            topic(nc, irc);
        }
        if (cmd.get_type_cmd() == "KICK")
        {
            nc->setKick(cmd._kick());
            kick(nc, irc);
            std::cout << "KICK" << std::endl;
        }
        if (cmd.get_type_cmd() == "INVITE")
        {
            nc->setInvinte(cmd._invite());
            std::cout << "INVITE" << std::endl;
            invite(nc, irc);
        }
        if (cmd.get_type_cmd() == "PRIVMSG")
        {
            std::cout << " PRIVMSG " << std::endl;
            nc->setPrivmsg(cmd._privmsg());
            privmsg(nc, irc);
        }
    }
    else
        std::cout << "can't get client :" << std::endl;
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
        //    std::cout << "Received message: (" << input + ")" << "Size: " << input.size() <<  std::endl;
        //    std::cout << "valeur de i: "  << i << std::endl;
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

        // Look for a complete line
        size_t pos;
        if ((pos = buffer.find("\r\n")) != std::string::npos)
        {
            line_ = buffer.substr(0, pos); // Extract the complete line
            buffer.erase(0, pos + 2);      // Remove the processed line
            return true;
        }
        return false; // Data received but no complete line yet
    }
    else if (bytes_read == 0)
    {
        std::cerr << "Client disconnected.\n";
        return false; // Client closed the connection
    }
    else
    {
        std::cerr << "Error receiving data (errno=" << errno << "): " << strerror(errno) << "\n";
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