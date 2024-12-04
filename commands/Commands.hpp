#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "string"
#include <iostream>
#include <array>
#include <vector>

class Commands
{
    private:
        std::string input;
        std::vector<std::string> arr_cmds;
    public:
        Commands(std::string message);
        ~Commands();
};


#endif 