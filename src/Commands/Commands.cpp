#include "Commands.hpp"

int removeCommanSpace(std::string word){
    unsigned int index = 0;
    std::string newString = "";
    while (index < word.size())
    {  
        if (word[index] == ' ' || word[index] == ',')
            return (1);
        index++;
    }
    return (0);
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
    while (i < txt.length() && (txt[i] == ' '))
            i++;
    return (i);
}

bool too_much_cmds(std::vector<std::string> split_cmd, std::array<std::string , 6> arr){
    int count = 0;
    unsigned long index = 0;
    unsigned long index_arr = 0;

    while (index < split_cmd.size())
    {
        index_arr = 0;
        while (index_arr < arr.size())
        {
            if (split_cmd[index] == arr[index_arr])
            {
                count++;
                break;
            }
            index_arr++;
        }
        index++;
    }
    if (count > 1)
        return (true);
    return false;
}
Commands::Commands() {}

std::string Commands::get_message(std::string msg){return msg;}
std::string Commands::get_type_cmd() {return this->type_cmds;}

bool isNewLine(std::string word){
    unsigned long i = 0 ;
    while ( i < word.length())
    {
        if (word[i] == '\n')
            return (true);
        i++;
    }
    return (false);
}

std::string noNewLIne(std::string word){
    std::string newWord = "";
    unsigned long i = 0 ;
    while ( i < word.length() && word[i] != '\n')
    {
        // std::cout << "word : " << "index " << i << word[i] << std::endl;
        newWord[i] += word[i];
        i++;
    }
    newWord = word.substr(0, i);
    std::cout << "le mot sans newLine : "<<newWord << std::endl;
    return newWord;
}

int isCommaSpace(unsigned int  start, std::string msg)
{
    bool isChar = false;
    while (start <  msg.size())
    {
        std::cout << "index word " << msg[start] << std::endl;
        if (msg[start] == ' ')
        {
            isChar = true;
            while (start <  msg.size() && msg[start] == ' ')
                start++;
        }
        if (msg[start] == ',')
        {
            isChar = true;
            while (start <  msg.size() && msg[start] == ' ')
                start++;
            if (start <  msg.size())
                start++;
        }
        if (isChar == true)
            return (start);
        start++;
    }
    return (start++);
}

Commands::Commands(std::string message)
{
    std::array<std::string , 6> arr = {"JOIN", "MODE", "TOPIC", "KICK", "INVITE", "PRIVMSG"};
    for (unsigned long i = 0; i < arr.size(); i ++)
    {
        name_cmds.push_back(arr[i]);
        // std::cout<< "Commands constructeur " << name_cmds[i] << std::endl;
    }
    this->input = message;
    std::string delims;
    std::string word;
    int end = 0;
    int start = 0;
    while (this->input.empty() == false)
    {
        start = index_lastspace(this->input);
        end = isCommaSpace(start , this->input);
        std::cout << "start index " << start << " end index " << end << std::endl;
         if (end == 0 || end  == -1) 
            end = (int)this->input.length();       
         word = this->input.substr(start, end);
         if (removeCommanSpace(word))
            word = this->input.substr(0, word.size() - 1);

         if (isNewLine(word) == true)
            word = noNewLIne(word);
         std::cout << "Le mot : " << word << " size : " << word.size() << std::endl;
            this->split_cmds.push_back(word);
         this->input.erase(start, end);
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
    if (too_much_cmds(this->split_cmds, arr))
        this->type_cmds = "";
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
        }
        index++;
    }
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
    return (this->arg);
}



/// @brief EX : It parses the message for the channel command channel name (value) and for password (key_value) if there is : usrname(value) and empty(key_value)
/// @return container map<value, key_value> 
std::map<std::string, std::string> Commands::_join(){
    // parsing quand la commande elle ne pas bonnes
    return (keys_and_value());
}


std::vector<std::string> Commands::_topic(){
    std::vector<std::string>  arr;
    unsigned long it = 0;
    std:: string channelName;
    std:: string msg;
    while (it < this->split_cmds.size())
    {
        if (it == 1){
            channelName = this->split_cmds[it];
            arr.push_back(channelName);
        }else if (it > 0){
            std::cout << this->split_cmds[it] << std::endl;
            msg += this->split_cmds[it] + " ";
        }
        it++;
    }
    if ( it == 1)
        msg = "";
    arr.push_back(msg);
    return (arr);
}

std::map<std::string, std::string> Commands::_invite (){
    // parsing quand la commande elle ne pas bonnes
    return (keys_and_value());
}

std::vector<std::string> Commands::get_splitcmds(){
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
        {
            if (index < this->split_cmds.size() && index > 2)
                var.arguments.push_back(this->split_cmds[index]);
        }
        index++;
    }
    return var;
}

context_mode Commands::_kick(){return (this->_mode());}

context_mode Commands::_privmsg()
{
   bool isTarget = false;
   unsigned long index = 0;
   context_mode var;
    while (index < this->split_cmds.size()){
         if (this->split_cmds[index][0] != ':' && isTarget == false && (this->split_cmds[index] != "PRIVMSG"))
         {
            std::cout << "this tagert : " << this->split_cmds[index] << std::endl;
            var.arguments.push_back(this->split_cmds[index]);
            index++;
        }else{
            std::cout << "this missage : " << this->split_cmds[index] << std::endl;
            if (this->split_cmds[index][0] == ':')
                isTarget = true;
            if (this->split_cmds[index] != "PRIVMSG" && this->split_cmds[index][0] != '#' && isTarget == true)
                var.modestring += split_cmds[index] + " ";
            index++;
        }
    }

    std::cout<< " modestring " << var.modestring << std::endl;
    std::cout<< " target " << var.arguments[0] << std::endl;
    return var;
}



