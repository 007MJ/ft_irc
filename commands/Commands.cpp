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

int index_lastspace(std::string txt){
    unsigned long i = 0;
    while (i < txt.length() && txt[i] == ' ')
            i++;
    return (i);
}

Commands::Commands(std::string message)
{
    std::array<std::string , 5> arr = {"JOIN", "MODE", "TOPIC", "KICK", "INVITE"};
    for (unsigned long i = 0; i < arr.size(); i ++)
    {
        name_cmds.push_back(arr[i]);
        // std::cout<< "Commands constructeur " << name_cmds[i] << std::endl;
    }
    this->input = message;
    std::string delims;
    std::string word;
    int end = 0;

    std::cout << "the string ....message = "<< this->input << "is empty "<< this->input.empty() << std::endl;

    while (this->input.empty() == false)
    {
         end = this->input.find(' ');
         end += index_lastspace(this->input);
        //  std::cout << "is the space "<< end  << std::endl;
         if (end == 0 || end  == -1)
            end = (int)this->input.length();       
         word = this->input.substr(0, end);
         if (_onlyspace(word) == false)
            this->split_cmds.push_back(word);
         this->input.erase(0, end);
    }

    unsigned long i = 0;
    bool find_cmd = false;
    while(i < this->split_cmds.size() && find_cmd == false)
    {
        unsigned long index = 0;
        while (index < name_cmds.size() && find_cmd == false)
        {
            if (is_command(this->split_cmds[i] ,name_cmds[index]))
            {
                this->type_cmds = name_cmds[index];
                find_cmd = true;
            }
            index++;
        }
        i++;
    }
    i = 0 ;
    std::cout << "type de command :"<< this->type_cmds << std::endl;
    while(i < this->split_cmds.size() )
    {
    
        std::cout << "what is in split_cmds ->" << this->split_cmds[i] << std::endl;
        i++;
    }

    std::cout << "....complite" << std::endl;

}
Commands::~Commands(){}

std::vector<std::string>Commands::values(){
    unsigned long index = 0;
    bool is_enter = false;
    std::vector<std::string> tmp;
    while (index < this->split_cmds.size())
    {
        unsigned long in = 0;
        is_enter = false;
        if (this->split_cmds[index][0] != '#' && this->split_cmds[index][0] != '&' && this->split_cmds[index][0] != ':')
        {
            while (in < this->name_cmds.size() && is_enter == false){
                if (split_cmds[index] == name_cmds[in])
                    is_enter = true;
                in++;
            }
            if (!is_enter)
                tmp.push_back(this->split_cmds[index]);
            std::cout << "Enter split -> : "<< this->split_cmds[index] << "name_cmds-> : " <<name_cmds[index]<< std::endl;
            // std::cout << "is what we want "<< tmp[in] << std::endl;
        }
        index++;
    }
    unsigned long in = 0;
        std::cout<<" start"<< std::endl;
    while (in < tmp.size()){
        std::cout<<" tmp --- " << tmp[in] << std::endl;
        in++;
    }
    std::cout<<" end"<< std::endl;
    return (tmp);
}

std::map<std::string, std::string>Commands::keys_and_value() {
    unsigned long index = 0;
    std::vector<std::string> tmp = this->values();
    while (index < this->split_cmds.size()){
        if (is_command(this->split_cmds[index], ":"))
            this->arg[this->split_cmds[index]] = "";
        else{
            if (this->split_cmds[index][0] == '#' || this->split_cmds[index][0] == '&')
                this->arg[this->split_cmds[index]] = "";
        }
        index++;
    }

    index = 0;
    unsigned long i = 0; 
    while (index < this->split_cmds.size() && i < this->arg.size() ){
        if (this->split_cmds[index][0] == '#' || this->split_cmds[index][0] == '&')
        {
            if (tmp.size() > 0)
                this->arg[this->split_cmds[index]] = tmp[i];
            i++;
        }
        index++;
    }
    std::map<std::string, std::string>::iterator it=this->arg.begin();
    std::map<std::string, std::string>::iterator end=this->arg.end();
    while (it != end )
    {
        std::cout<< "firs-> : " << it->first << " | second-> : " << it->second << std::endl;
        it++;
    }
    return (this->arg);
}



// std::vector<std::string> Commands::splittochar(char x){
// }



/// @brief It parses the message for the channel command channel name (value) and for password (key_value) if there is : usrname(value) and empty(key_value)
/// @return container map<value, key_value> 
std::map<std::string, std::string> Commands::_join(){
    return (keys_and_value());
}

std::vector<std::string> Commands::getsplitcmds()  {
    return (this->split_cmds);
}


/// @brief It analyzes the message as follows <target> [<modestring> [<mode arguments>...]]
/// @return struct with which has three variables the string target and string modestring and argument vector
context_mode Commands::_mode(){
    unsigned long index = 0;
    context_mode var;
    if (this->split_cmds.size() > 2)
        var.target = this->split_cmds[1];
    while (index < this->split_cmds.size()){
        if (index == 2)
            var.modestring = this->split_cmds[index];
        else
            var.arguments.push_back(this->split_cmds[index]);
        index++;
    }
    std::cout << "strut" << std::endl;
    std::cout << "target    " << var.target << std::endl;
    std::cout << "modstring " << var.modestring << std::endl;
    std::cout << "argument  " << var.arguments[0] << std::endl;

    return var;
}