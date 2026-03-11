/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** FileBar.hpp
*/

#ifndef FILEBAR_HPP_
    #define FILEBAR_HPP_

#include <dirent.h>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace fe {
    class FileBar {
        struct dirent* _file;
        std::unique_ptr<sf::RectangleShape> _rect;
        bool _hover = false;

        public:
            FileBar(struct dirent* file);

            ~FileBar() = default;
            FileBar(const FileBar&) = delete;
            FileBar& operator=(const FileBar&) = delete;
            FileBar(FileBar&&) noexcept = default;
            FileBar& operator=(FileBar&&) noexcept = default;

            void draw(sf::Vector2f pos, sf::Text& text, sf::RenderWindow& window);
            bool isDirectory() const;
            std::string getFileName() const noexcept;
            bool getHover() const noexcept;
    };
}

#endif /* FILEBAR_HPP_ */
