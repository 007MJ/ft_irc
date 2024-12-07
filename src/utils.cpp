#include "../includes/utils.hpp"
#include <cstring>
#include <cstdlib>

void errorMsg(const std::string &msg){
    std::string formattedMsg = msg + "\n";
    write(2, formattedMsg.c_str(), formattedMsg.length());
}

bool ValidateAndStoreArgs(char *argv[], int* port_, std::string& pass_){
    {
        for(size_t i = 0; i < std::strlen(argv[1]); i++){
            if(!isdigit(argv[1][i])){
                errorMsg("Error. A non-digit character is detected in port number");
                return false;
            }
        }
        *port_ = std::atoi(argv[1]);
        
    }
    {
        for(size_t i = 0; i < std::strlen(argv[2]); i++){
            if(iswspace(argv[2][i])){
                errorMsg("Error. A white space character is detected in the entered password");
                return false;
            }
        }
        pass_ = argv[2];
    }
    return true;
}