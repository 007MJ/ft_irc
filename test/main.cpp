// #include "../commands/Commands.hpp"
#include "../src/Channel/Channel.hpp"
#include "../src/Client/Client.hpp"
#include <iostream>
#include <string>

int main(int ac, char **av)
{
    if(ac != 2)
        return 1;
    (void)av;
    Client client(42, "127.0.0.1", "albert");
    Channel chan(std::string("albert"), client);
    std::cerr << chan.AddClientAsSuperUser(42) ;
    std::cerr << chan.AddClientAsSuperUser(420) ;

    return 0;
    // if (ac > 1)
    // {
        // Commands irc_input(av[1]);
        // // irc_input._join();
        // // irc_input.keys_and_value();
        // // irc_input._mode();
        // irc_input._topic();
        // irc_input._kick();
        // irc_input._invinte();
    // }else
    //     std::cout << "no av" << std::endl;
}