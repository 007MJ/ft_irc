#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "string"
#include <iostream>
#include <array>
#include <vector>
#include <map>
class Commands
{
    private:
        std::string input;
        std::vector<std::string> name_cmds;
        std::vector<std::string> split_cmds;
        std::string type_cmds;
        std::map<std::string, std::string> arg;

    public:
        Commands(std::string message);
        std::vector<std::string> getArr_cmds();
        ~Commands();
         std::map<std::string, std::string> _join();
};


#endif 