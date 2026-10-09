/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Network
*/

#ifndef NETWORK_HPP_
    #define NETWORK_HPP_

    #include <array>
    #include <cstdint>
    #include <cstring>
    #include <functional>
    #include <map>
    #include <stdexcept>
    #include <string>
    #include <vector>

#define BUFFSIZE 1200

struct Datatype {
    uint16_t magic = 0x38A2;
    uint16_t id = 0;
    uint16_t length = 0;
    std::vector<uint8_t> payload;

    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> buffer;

        buffer.emplace_back((magic >> 8) & 0xFF);
        buffer.emplace_back(magic & 0xFF);
        buffer.emplace_back((id >> 8) & 0xFF);
        buffer.emplace_back(id & 0xFF);
        buffer.emplace_back((length >> 8) & 0xFF);
        buffer.emplace_back(length & 0xFF);
        for (const uint8_t item : payload) {
            buffer.emplace_back(item);
        }
        return buffer;
    }

    static Datatype deserialize(const std::array<uint8_t, BUFFSIZE> &buffer, std::size_t bytes) {
        if (bytes < 6) {
            throw std::runtime_error("Invalid buffer size.");
        }

        Datatype msg;
        msg.magic = (buffer[0] << 8) | buffer[1];
        if (msg.magic != 0x38A2) {
            throw std::runtime_error("Invalid magic number.");
        }
        msg.id = (buffer[2] << 8) | buffer[3];
        msg.length = (buffer[4] << 8) | buffer[5];

        if (msg.length != bytes - 6) {
            throw std::runtime_error("Payload length mismatch.");
        }

        for (std::size_t i = 6; i < bytes; i++) {
            msg.payload.emplace_back(buffer[i]);
        }
        return msg;
    }
};

typedef std::function<void(const Datatype &)> command_handler_t;
typedef std::map<uint16_t, command_handler_t> command_map_t;

#endif /* !NETWORK_HPP_ */
