#include "Commands.hpp"

bool _onlyspace(std::string string){
        for (unsigned long i = 0 ; i < string.length(); i++)
            if (string[i] != ' ')
                return false;
        return true;
}

Commands::Commands(std::string message)
{
    std::array<std::string, 5> name_cmds = {"JOIN", "MODE", "TOPIC", "KICK", "INVITE"};
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

    for (unsigned long i = 0 ; i < this->arr_cmds.size(); i++)
    {
        unsigned long index = 0;
        while (index < name_cmds.size())
        {
            // if (this->arr_cmds[i] == name_cmds[index])
            // {


                // break;
            // }
            index++;
        }
    }
    // std::cout << this->arr_cmds[i] << std::endl;
    std::cout << "....complite" << std::endl;

}
Commands::~Commands(){}