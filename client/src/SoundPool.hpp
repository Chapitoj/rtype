/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** SoundPool
*/

#ifndef SOUNDPOOL_HPP_
    #define SOUNDPOOL_HPP_

#include <SFML/Audio.hpp>
#include <vector>
#include <memory>

namespace rType {
    class SoundPool {
        public:
            SoundPool(const std::string& filePath, std::size_t poolSize, float volume = 20.0f) {
                if (!buffer.loadFromFile(filePath)) {
                    throw std::runtime_error("Failed to load sound file: " + filePath);
                }
                for (std::size_t i = 0; i < poolSize; ++i) {
                    sounds.emplace_back(sf::Sound(buffer));
                    sounds.back().setVolume(volume);
                }
            }

            void play() {
                for (auto& sound : sounds) {
                    if (sound.getStatus() != sf::Sound::Playing) {
                        sound.play();
                        return;
                    }
                }
            }

        private:
            sf::SoundBuffer buffer;
            std::vector<sf::Sound> sounds;
    };
};

#endif /* !SOUNDPOOL_HPP_ */
