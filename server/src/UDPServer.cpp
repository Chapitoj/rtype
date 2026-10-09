/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** UDPServer
*/

#include "UDPServer.hpp"

rType::UDPServer::UDPServer(std::size_t port)
    : _ioContext(), _socket(_ioContext, udp::endpoint(udp::v4(), port))
{
    _mutex = std::make_shared<std::mutex>();
}

rType::UDPServer::~UDPServer()
{
    _socket.close();
    _ioContext.stop();
    if (_ioThread.joinable())
        _ioThread.join();
}

void rType::UDPServer::start(std::unique_ptr<command_map_t> commands)
{
    _commands = std::move(commands);
    _ioThread = std::thread([this] {
        serverLoop();
        _ioContext.run();
    });
}

asio::io_context &rType::UDPServer::getIoContext()
{
    return _ioContext;
}

std::size_t rType::UDPServer::getNbClients() const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    return _clients.size();
}

void rType::UDPServer::receive(const asio::error_code &error, std::size_t bytes)
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
    } catch (const std::exception &) {
    }
}

void rType::UDPServer::send(const Datatype &data)
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

void rType::UDPServer::send(const udp::endpoint &endpoint, const Datatype &data)
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

void rType::UDPServer::sendToEveryone(const Datatype &data)
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

void rType::UDPServer::serverLoop()
{
    _socket.async_receive_from(asio::buffer(_buffer), _endpoint, [this](const asio::error_code &error, std::size_t bytes) {
        receive(error, bytes);
        _buffer.fill(0);
        serverLoop();
    });
}


void rType::UDPServer::acceptClient(const std::string &username)
{
    bool isNew = false;

    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        auto known = std::find_if(_clients.begin(), _clients.end(), [this](const ClientData &client) {
            return client.endpoint == _endpoint;
        });

        if (known == _clients.end()) {
            _clients.emplace_back(ClientData{username, _endpoint});
            isNew = true;
        }
    }
    Datatype data;
    data.id = 0x01;
    data.length = 0;

    send(data);
    if (isNew) {
        std::cout << "New client connected: " << username << " on " << _endpoint.address().to_string() << ":" << _endpoint.port() << std::endl;
    }
}

void rType::UDPServer::removeClient()
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    auto it = std::find_if(_clients.begin(), _clients.end(), [this](const ClientData &client) {
        return client.endpoint == _endpoint;
    });

    if (it != _clients.end()) {
        _clients.erase(it);
    }
}

std::vector<rType::ClientData> rType::UDPServer::getClients() const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    return _clients;
}

std::shared_ptr<std::mutex> &rType::UDPServer::getMutex()
{
    return _mutex;
}
