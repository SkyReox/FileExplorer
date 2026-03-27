/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** FileBar.hpp
*/

#ifndef FILEBAR_HPP_
    #define FILEBAR_HPP_

#include "Button.hpp"
#include "constants.hpp"
#include <dirent.h>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace fe {
    class FileBar : public Button {
        std::string _fileName;
        std::string _path;
        std::unique_ptr<sf::Text> _text;

        public:
            FileBar(struct dirent* file, const std::string& parentPath, sf::Font& font, sf::Vector2f size = sf::Vector2f(sf::VideoMode::getDesktopMode().width, TEXT_SIZE + FILE_SEP_SIZE * 0.8));

            ~FileBar() = default;
            FileBar(const FileBar&) = delete;
            FileBar& operator=(const FileBar&) = delete;
            FileBar(FileBar&&) noexcept = default;
            FileBar& operator=(FileBar&&) noexcept = default;

            void update(sf::RenderWindow& window) final;
            void draw(sf::Vector2f pos, sf::RenderWindow& window) final;
            bool isDirectory() const;
            std::string getFileName() const noexcept;
            std::size_t getFileSize() const;
    };
}

#endif /* FILEBAR_HPP_ */
