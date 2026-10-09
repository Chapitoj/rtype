/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ServArguments
*/

#include "ServArguments.hpp"
#include <string>

namespace {
    const std::string USAGE =
        "USAGE:\n\t./r-type_server -p <port>\n\n"
        "DESCRIPTION:\n\tLaunch the R-Type server and listen for incoming client connections.\n\n"
        "OPTIONS:\n\t-p <port>\tSpecify the port number to listen on (1-65535).\n\n"
        "EXAMPLES:\n\t./r-type_server -p 8080\n\n"
        "NOTES:\n\t- Ensure the specified port is available and not in use by another process.";
}

rType::ServArguments::ServArguments(int ac, const char *av[]) : rType::Arguments(ac, av,
    {
        {"-p", [this](){this->_flagBools["-p"].first = true;}},
        {"-help", [](){throw rType::ParameterError(USAGE);}}
    },
    {
        {"-p", {false, [this](std::string arg){
            try {
                this->_op_port = rType::parsePort(arg);
            } catch (std::exception &) {
                throw rType::ParameterError(USAGE);
            }
            this->_flagBools["-p"].first = false;}}
        }
    })
{
    this->retrieveArguments();
}

void rType::ServArguments::retrieveArguments()
{
    rType::Arguments::retrieveArguments();
    if (!this->_op_port.has_value()) {
        throw rType::ParameterError(USAGE);
    }
    this->_port = this->_op_port.value();
}
