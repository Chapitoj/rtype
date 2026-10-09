/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Lobby
*/

#include "Lobby.hpp"

rType::Lobby::Lobby(asio::io_context &ioContext, std::shared_ptr<std::mutex> mutex, std::size_t id)
    : _id(id), _socket(ioContext, udp::endpoint(udp::v4(), 0)), _mutex(mutex)
{
    _port = _socket.local_endpoint().port();
}

rType::Lobby::~Lobby()
{
    _socket.close();
}

void rType::Lobby::start(std::unique_ptr<command_map_t> commands)
{
    _commands = std::move(commands);
    serverLoop();
}

std::size_t rType::Lobby::getPort() const
{
    return _port;
}

void rType::Lobby::receive(const asio::error_code &error, std::size_t bytes)
{
    if (error) {
        return;
    }
    try {
        Datatype data = Datatype::deserialize(_buffer, bytes);
        auto cmd = _commands->find(data.id);

        if (cmd != _commands->end()) {
           cmd->second(data);
        }
    } catch (const std::exception &e) {
        // std::cerr << "Error in receive: " << e.what() << std::endl;
    }
}

void rType::Lobby::send(const Datatype &data)
{
    std::shared_ptr<std::vector<uint8_t>> serializedData = std::make_shared<std::vector<uint8_t>>(data.serialize());
    std::lock_guard<std::mutex> lock(*_mutex);
    _socket.async_send_to(
        asio::buffer(*serializedData),
        _endpoint,
        [this, serializedData](const asio::error_code &error, std::size_t) {
            if (error) {
                std::cerr << "Error: " << error.message() << std::endl;
            }
        }
    );
}

void rType::Lobby::send(const udp::endpoint &endpoint, const Datatype &data)
{
    std::shared_ptr<std::vector<uint8_t>> serializedData = std::make_shared<std::vector<uint8_t>>(data.serialize());
    std::lock_guard<std::mutex> lock(*_mutex);
    _socket.async_send_to(
        asio::buffer(*serializedData),
        endpoint,
        [this, serializedData](const asio::error_code &error, std::size_t) {
            if (error) {
                std::cerr << "Error: " << error.message() << std::endl;
            }
        }
    );
}

void rType::Lobby::sendToEveryone(const Datatype &data)
{
    std::vector<udp::endpoint> endpoints;

    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        for (const auto &client : _clients) {
            endpoints.push_back(client.endpoint);
        }
    }
    for (const auto &endpoint : endpoints) {
        send(endpoint, data);
    }
}

void rType::Lobby::serverLoop()
{
    _socket.async_receive_from(asio::buffer(_buffer), _endpoint, [this](const asio::error_code &error, std::size_t bytes) {
        receive(error, bytes);
        _buffer.fill(0);
        serverLoop();
    });
}

void rType::Lobby::acceptClient(const std::string &username)
{
    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        auto known = std::find_if(_clients.begin(), _clients.end(), [this](const ClientStruct &client) {
            return client.endpoint == _endpoint;
        });

        if (known == _clients.end()) {
            if (_clients.size() >= 4) {
                return;
            }
            _clients.emplace_back(ClientStruct{_clientId++, username, _endpoint});
        }
    }
    Datatype data;
    data.id = 0x01;
    data.length = 0;

    send(data);
}

void rType::Lobby::removeClient()
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    auto it = std::find_if(_clients.begin(), _clients.end(), [this](const ClientStruct &client) {
        return client.endpoint == _endpoint;
    });

    if (it != _clients.end()) {
        _clients.erase(it);
    }
}

std::vector<rType::ClientStruct> rType::Lobby::getClients() const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    return _clients;
}

std::size_t rType::Lobby::getNbClients() const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    return _clients.size();
}

int rType::Lobby::getCurrentClientId() const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    auto it = std::find_if(_clients.begin(), _clients.end(), [this](const ClientStruct &client) {
        return client.endpoint == _endpoint;
    });
    if (it != _clients.end()) {
        return it->id;
    }
    return -1;
}
