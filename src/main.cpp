#include "../includes/header.hpp"

int main(int ac, char **av){

    if (ac == 3)
    {
        Server irc;
        std::cout << "port : " << av[1] << std::endl;
        std::cout << "password : " << av[2] << std::endl;
        irc.getport((av[1]));
        irc.getpassword(av[2]);
        irc.connection();
    }else
        std::cout << "no av "<< std::endl;

    // socket
    // bind
    // listen
    // accept
    // other 
    // close 
}