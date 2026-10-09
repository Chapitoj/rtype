/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Server
*/

#include "Server.hpp"
#include "StringUtils.hpp"

rType::Server::Server(std::size_t port) : _server(port)
{
    _running = std::make_shared<std::atomic<bool>>(true);
    command_map_t commands = {
        {0x65, std::bind(&Server::addNewClient, this, std::placeholders::_1)},
        {0x66, std::bind(&Server::disconnectClient, this, std::placeholders::_1)},
        {0x67, std::bind(&Server::createRoom, this, std::placeholders::_1)},
        {0x68, std::bind(&Server::listOfRoom, this, std::placeholders::_1)},
        {0x69, std::bind(&Server::joinRoom, this, std::placeholders::_1)},
    };
    _server.start(std::make_unique<command_map_t>(commands));
}

rType::Server::~Server()
{
    Datatype data;
    data.id = 0x02;
    data.length = 0;

    _server.sendToEveryone(data);
}

void rType::Server::run()
{
    try {
        while (*_running) {
            std::string input;
            std::getline(std::cin, input);
            if (utils::StringUtils::toLower(input) == "exit" || std::cin.eof()) {
                *_running = false;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in run: " << e.what() << std::endl;
    }
}

void rType::Server::addNewClient(const Datatype &data)
{
    if (data.length == 0 || data.length > 12) {
        return;
    }
    std::string username(data.payload.begin(), data.payload.end());

    _server.acceptClient(username);
}

void rType::Server::disconnectClient(const Datatype &)
{
    _server.removeClient();
}

void rType::Server::createRoom(const Datatype &)
{
    try {
        std::size_t roomId = _roomsCount++;
        _rooms.emplace_back(std::make_unique<GameRoom>(roomId, _server.getIoContext(), _server.getMutex(), _running));
        std::size_t port = _rooms.back()->getPort();
        Datatype data;
        data.id = 0x08;
        data.length = 2;
        data.payload.emplace_back((port >> 8) & 0xFF);
        data.payload.emplace_back(port & 0xFF);
        _server.send(data);
        _server.removeClient();
    } catch (const std::exception& e) {
        std::cerr << "Error creating room: " << e.what() << std::endl;
    }
}

void rType::Server::listOfRoom(const Datatype &)
{
    Datatype data;
    data.id = 0x03;
    data.length = 0;
    for (const auto& room : _rooms) {
        if (room->getNbPlayers() < 4 && !room->getGameStatus()) {
            data.payload.emplace_back(room->getRoomId());
            data.length += 1;
        }
    }
    _server.send(data);
}

void rType::Server::joinRoom(const Datatype &data)
{
    if (data.payload.empty()) {
        return;
    }
    try {
        std::size_t roomId = data.payload[0];
        auto it = std::find_if(_rooms.begin(), _rooms.end(), [roomId](const std::unique_ptr<GameRoom> &room) {
            return room->getRoomId() == roomId;
        });
        if (it == _rooms.end() || (*it)->getGameStatus()) {
            listOfRoom({});
            return;
        }
        std::size_t port = (*it)->getPort();
        Datatype data;
        data.id = 0x08;
        data.length = 2;
        data.payload.emplace_back((port >> 8) & 0xFF);
        data.payload.emplace_back(port & 0xFF);
        _server.send(data);
        _server.removeClient();
    } catch (const std::exception& e) {
        std::cerr << "Error joining room: " << e.what() << std::endl;
    }
}