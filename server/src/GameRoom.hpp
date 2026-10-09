/*
** EPITECH PROJECT, 2025
** rtape
** File description:
** GameRoom
*/

#ifndef GAMEROOM_HPP_
    #define GAMEROOM_HPP_

    #include <iostream>
    #include <vector>
    #include <algorithm>
    #include <mutex>
    #include <memory>
    #include <atomic>
    #include <thread>
    #include <queue>
    #include "Lobby.hpp"
    #include "Scene.hpp"
    #include "System.hpp"

    using asio::ip::udp;

namespace rType {
    class ECSLoader;

    class GameRoom {
        public:
            GameRoom(std::size_t roomId, asio::io_context &context, std::shared_ptr<std::mutex> mutex, std::shared_ptr<std::atomic<bool>> running);
            ~GameRoom();
            std::size_t getRoomId() const;
            std::size_t getPort() const;
            std::size_t getNbPlayers() const;
            bool getGameStatus() const;

            void sendPlayersList();
            void acceptClient(const Datatype &);
            void removeClient(const Datatype &);
            void receiveStartGame(const Datatype &);
            void checkMovePlayer(const Datatype & data);
            void checkPlayerShoot(const Datatype &);

        private:
            void loadRegistry();
            void spawnEnemy(std::size_t nbPlayers);
            void initComponents();
            void initSystems();
            void initGame();
            void processCommands();
            bool checkEndGame();
            void runAnimationSystems();
            void startGame();

            Lobby _lobby;
            Scene _scene;
            std::size_t _roomId = 0;
            bool _bossSpawned = false;
            bool _timerBoss = false;
            std::thread _gameThread;
            std::shared_ptr<std::atomic<bool>> _running;
            std::atomic<bool> _gameStarted{false};
            std::shared_ptr<ECSLoader> _ecsLoader;
            std::shared_ptr<registry> _registry;
            sf::Clock _clock;
            std::queue<Command> _commandsQueue;
            std::mutex _mutex;
            std::vector<sf::Clock> _clockEnemy;
            int _nbScene = 1;
    };
};

#endif // GAMEROOM_HPP_
