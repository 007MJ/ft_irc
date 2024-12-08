#include "../commands/Commands.hpp"


int main(int ac, char **av)
{
    if (ac > 1)
    {
        Commands irc_input(av[1]);
        // irc_input._join();
        // irc_input.keys_and_value();
        // irc_input._mode();
        // irc_input._mode();
        // irc_input._kick();
        irc_input._invinte();
    }else
        std::cout << "no av" << std::endl;
}