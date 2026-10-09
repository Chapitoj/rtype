/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Arguments
*/

#ifndef ARGUMENTS_HPP_
    #define ARGUMENTS_HPP_

    #include <iostream>
    #include <vector>
    #include <optional>
    #include <map>
    #include <functional>
    #include <string>
    #include <stdexcept>
    #include <tuple>
    #include "Error.hpp"

namespace rType {
    /*
    * @brief Parse a TCP/UDP port number.
    * @param arg text to parse
    * @throws std::invalid_argument if arg is not an integer in [1, 65535]
    */
    inline std::size_t parsePort(const std::string &arg)
    {
        std::size_t parsed = 0;
        int value = 0;

        try {
            value = std::stoi(arg, &parsed);
        } catch (const std::exception &) {
            throw std::invalid_argument("invalid port: " + arg);
        }
        if (parsed != arg.size() || value < 1 || value > 65535) {
            throw std::invalid_argument("invalid port: " + arg);
        }
        return static_cast<std::size_t>(value);
    }

    class Arguments {
        public:
            /*
            * @brief Constructor of Arguments class.
            * @param ac number of arguments
            * @param av content of arguments
            */
            Arguments(int ac, const char *av[],
                std::map<std::string, std::function<void ()>> flags,
                std::map<std::string, std::pair<bool, std::function<void(std::string)>>> flagsBools) : _ac(ac), _flags(flags), _flagBools(flagsBools)
            {
                for (int i = 1; i < ac; i++)
                    this->_av.emplace_back(std::string(av[i]));
            };

        protected:
            /*
            * @brief Retrieve the arguments passed to the program.
            */
            virtual void retrieveArguments()
            {
                for (const auto& arg : this->_av) {
                    if (this->_flags.find(arg) != this->_flags.end()) {
                        this->_flags[arg]();
                        continue;
                    }
                    for (const auto& [flag, pair] : this->_flagBools) {
                        if (pair.first) {
                            pair.second(arg);
                            break;
                        }
                    }
                }
            }

            std::size_t _ac;
            std::vector<std::string> _av;
            std::map<std::string, std::function<void()>> _flags;
            std::map<std::string, std::pair<bool, std::function<void(std::string)>>> _flagBools;
    };
};

#endif /* !ARGUMENTS_HPP_ */
