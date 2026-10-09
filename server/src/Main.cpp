/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Main
*/

#include "ServArguments.hpp"
#include "Server.hpp"

int main(int ac, const char *av[])
{
    try {
        rType::ServArguments args(ac, av);
        rType::Server server(args.getPort());

        server.run();
    } catch (const rType::Error &error) {
        std::cout << error.what() << std::endl;
        return 84;
    }
    return 0;
}
