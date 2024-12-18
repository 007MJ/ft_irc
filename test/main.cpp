// #include "../commands/Commands.hpp"
#include "../src/Channel/Channel.hpp"
#include "../src/Client/Client.hpp"
#include <iostream>
#include <sstream>
#include <string>

int main(int ac, char **av)
{
    if(ac != 3)
        return 1;

    Client client(42, "127.0.0.1", "Bob");
    Client client1(56, "127.0.0.1", "Lwalya");
    Client client2(533, "127.0.0.1", "John");
    Channel chan(std::string("albert"), std::string(av[1]), client);
    
    std::cout << "######################### DEBUG/TEST #########################\n";

    chan.SetTopic(std::string(av[2]));
    std::cout << chan.GetName() << "'s topic set to: " << chan.getTopic() + "\n";

    chan.AddClient(&client1);
    chan.AddClient(&client2);

    std::cout << "Password set to: " << chan.GetPassword() + "\n";

    chan.SetPassword("pass");
    std::cout << "Password reset to: " << chan.GetPassword() + "\n";

    chan.SetInviteOnlyModeTo(true);
    std::cout << chan.GetName() << "'s restriction mode is set to : " << chan.InviteOnlyModeIsActivated() << "\n";


    std::cerr << chan.AddClientAsSuperUser(56) ;
    std::cerr << chan.AddClientAsSuperUser(420) ;

    std::ostringstream outt;
    outt << chan.GetName() << "'s restriction is set to: ";
    chan.TopicModeIsRestricted() == true ? outt << "On\n" : outt << "Off\n";
    std::cout << outt.str();

    std::cout << chan.GetName() << "'s limit : " << chan.getChannelLimit() << "\n";
    chan.SetChannelLimit(3);
    std::cout << chan.GetName() << "'s limit : " << chan.getChannelLimit() << "\n";

    std::cout << "############################# END #############################\n";
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
