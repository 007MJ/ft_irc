#include "../commands/Commands.hpp"


int main(int ac, char **av)
{
    if (ac > 1)
    {
        Commands irc_input(av[1]);
        irc_input._join();
    }else
        std::cout << "no av" << std::endl;
}