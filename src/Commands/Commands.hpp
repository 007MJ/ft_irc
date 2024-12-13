#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "string"
#include <iostream>
#include <array>
#include <vector>
#include <map>

struct   context_mode{
    std::string target;
    std::string modestring;
    std::vector<std::string> arguments;
};

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
        Commands();
        ~Commands();
        std::vector<std::string> get_splitcmds();
        std::vector<std::string> values();
        std::string get_message(std:: string message);
        // std::vector<std::string> splittochar(char x);
        std::string get_type_cmd();
        std::map<std::string, std::string> keys_and_value();
        std::map<std::string, std::string> _join();
        std::map<std::string, std::string> _topic();
        std::map<std::string, std::string> _invinte();
        context_mode _mode();
        context_mode _kick();

};


#endif 