/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** CliArguments
*/

#include "CliArguments.hpp"
#include <string>

namespace {
    const std::string USAGE =
        "USAGE:\n\t./r-type_client -p <port> [-h <ip>] [-u <username>]\n\n"
        "DESCRIPTION:\n\tLaunch the R-Type client and connect to a server.\n\n"
        "OPTIONS:\n\t-p <port>\tSpecify the port number to connect to (required, 1-65535).\n"
        "\t-h <ip>\t\tSpecify the server's IP address (optional). Defaults to 127.0.0.1 if not provided.\n"
        "\t-u <username>\tSpecify the username (optional, 1-12 characters). Defaults to RTyper.\n\n"
        "EXAMPLES:\n\t./r-type_client -p 8080\n\t./r-type_client -p 8080 -h 192.168.1.10 -u Alice\n\n"
        "NOTES:\n\t- Ensure that the server is running and accessible on the provided IP and port.\n"
        "\t- If the IP address is omitted, the client connects to localhost (127.0.0.1) by default.";
}

rType::CliArguments::CliArguments(int ac, const char *av[]) : rType::Arguments(ac, av,
    {
        {"-p", [this](){this->_flagBools["-p"].first = true;}},
        {"-u", [this](){this->_flagBools["-u"].first = true;}},
        {"-h", [this](){this->_flagBools["-h"].first = true;}},
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
        },
        {"-h", {false, [this](std::string arg){
            this->_ip = arg;
            this->_flagBools["-h"].first = false;}}
        },
        {"-u", {false, [this](std::string arg){
            if (arg.empty() || arg.size() > 12) {
                throw rType::ParameterError(USAGE);
            }
            this->_username = arg;
            this->_flagBools["-u"].first = false;}}
        }
    })
{
    this->retrieveArguments();
}

void rType::CliArguments::retrieveArguments()
{
    rType::Arguments::retrieveArguments();
    if (!this->_op_port.has_value()) {
        throw rType::ParameterError(USAGE);
    }
    this->_port = this->_op_port.value();
}
