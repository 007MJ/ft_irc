#include "Commands.hpp"

int removeCommanSpace(const std::string& word) {
    for (size_t i = 0; i < word.size(); ++i) {
        if (word[i] == ' ' || word[i] == ',')
            return 1;
    }
    return 0;
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

bool too_much_cmds(std::vector<std::string> split_cmd, std::vector<std::string> arr)
{
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

std::string noNewLIne(const std::string& word) {
    unsigned long i = 0;
    while (i < word.length() && word[i] != '\n')
        i++;
    return word.substr(0, i);
}


int isCommaSpace(unsigned int start, const std::string& msg) {
    while (start < msg.size()) {
        if (msg[start] == ',' || msg[start] == ' ') {
            // Ignore les espaces ou virgules consécutifs
            while (start < msg.size() && (msg[start] == ',' || msg[start] == ' '))
                start++;
            return (int)start;
        }
        start++;
    }
    return (int)start;
}

int whereIsDifferent(std::string s1, std::string s2)
{
    unsigned index = 0;
    std::cout << "size s1 :" << s1.size() << "size s2 " << s2.size() << std::endl;
    while (index < s1.size() && index < s2.size())
    {
        if (s1[index] == s2[index])
        {
            std::cout << "same" <<  std::endl;
            std::cout << "s1 :" << s1[index] << std::endl;
            std::cout << "s2 :" << s2[index] << std::endl;
        }else
        {
            std::cout << "dif -> "<<  s1[index]<< std::endl;
            std::cout << "dif -> "<<  s2[index]<< std::endl;
        }
        index++;
    }
    return 0;
}

Commands::Commands(std::string message) {
    // Liste des commandes reconnues
    std::string arr[] = {"PONG", "QUIT", "JOIN", "MODE", "TOPIC", "KICK", "INVITE", "PRIVMSG", "PASS", "NICK", "USER"};
    // std::cout << "size of arrrrrrrrrrrrr" << sizeof(arr) << std::endl;
    for (unsigned int  i = 0; i <= 10 ; i++) {
        name_cmds.push_back(arr[i]);
    }
    // std::cout << "sizeof "<< name_cmds.size() << std::endl;
    this->input = message;
    std::string word = "";

    // Parsing du message en mots séparés par espaces ou virgules
    for (size_t i = 0; i < this->input.size(); ++i) 
    {
        char c = this->input[i];

        if (c != ' ' && c != ',' && c != '\n') 
        {
            word += c; // Ajoute le caractère au mot en cours
        } 
        else 
        {
            if (!word.empty()) 
            {
                this->split_cmds.push_back(word); // Ajoute le mot à la liste
                word.clear(); // Réinitialise le mot
            }
        }
    }

    // Ajoute le dernier mot si la chaîne ne se termine pas par un séparateur
    if (!word.empty()) {
        this->split_cmds.push_back(word);
    }

    // Recherche de la commande principale
    for (size_t i = 0; i < this->split_cmds.size(); ++i) 
    {
        // std::cout << "tour " << i << std::endl;
        // std::cout <<"main loop " << this->split_cmds[i] << std::endl;
        // std::cout <<"size of arr " << name_cmds.size() << std::endl;
        for (size_t j = 0; j < name_cmds.size(); ++j) 
        {
            if (this->split_cmds[i] == name_cmds[j]) 
            {
                // std::cout << "same fun" << this->split_cmds[i] << std::endl;
                this->type_cmds = name_cmds[j];
                break;
            }
        }
        if (!this->type_cmds.empty()) 
            break;
    }

    // for (unsigned int i = 0; i < this->split_cmds.size(); i++)
    // {
    //     // std::cout<< "in index :" << this->split_cmds[i] << std::endl;
    // }
    // Vérifie s'il y a trop de commandes reconnues
    if (too_much_cmds(this->split_cmds, name_cmds)) {
        this->type_cmds = "";
    }
    // std::cout << "type of cmd :" << this->type_cmds << std::endl;
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
            while (in < this->name_cmds.size() && is_enter == false)
            {
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

std::map<std::string, std::string>Commands::keys_and_value() 
{
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
    while (index < this->split_cmds.size())
    {
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