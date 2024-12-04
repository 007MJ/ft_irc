#include "Commands.hpp"

bool _onlyspace(std::string string){
        for (unsigned long i = 0 ; i < string.length(); i++)
            if (string[i] != ' ')
                return false;
        return true;
}

bool is_command(std::string s1, std::string s2){
    unsigned long index = 0;
    if (index == 0 && s1[index] == ':' && s2[index] == ':')
        return (true);
    else if (s1 == s2)
        return (true);
    return (false);
}

Commands::Commands(std::string message)
{
    std::array<std::string, 7> name_cmds = {"#JOIN", "&JOIN", ":", "MODE", "TOPIC", "KICK", "INVITE"};
    for (unsigned long i = 0; i < name_cmds.size(); i ++)
    {
        std::cout << name_cmds[i] << std::endl;
    }
    this->input = message;
    std::string delims;
    std::string word;
    int end = 0;

    std::cout << "the string ....message = "<< this->input << "is empty "<< this->input.empty() << std::endl;

    while (this->input.empty() == false)
    {
         end = this->input.find(' ');
         if (end == 0 || end  == -1)
            end = (int)this->input.length();       
         word = this->input.substr(0, end);
         if (_onlyspace(word) == false)
            this->arr_cmds.push_back(word);
         this->input.erase(0, end  + 1);
    }
    unsigned long i = 0;
    bool find_cmd = false;
    while(i < this->arr_cmds.size() && find_cmd == false)
    {
        unsigned long index = 0;
        while (index < name_cmds.size() && find_cmd == false)
        {
            if (is_command(this->arr_cmds[i] ,name_cmds[index]))
            {
                this->type_cmds = name_cmds[index];
                find_cmd = true;
            }
            index++;
        }
        i++;
    }
    std::cout << this->type_cmds << std::endl;
    // std::cout << this->arr_cmds[i] << std::endl;
    std::cout << "....complite" << std::endl;

}
Commands::~Commands(){}