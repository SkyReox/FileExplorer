/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** DirButton.hpp
*/

#ifndef DIRBUTTON_HPP_
    #define DIRBUTTON_HPP_

#include "Button.hpp"
#include <string>

namespace fe {
    class DirButton : public Button {
        std::string _dirPath;
        std::unique_ptr<sf::Text> _text;

        public:
            DirButton(std::string dirPath, sf::Font& font, sf::Vector2f size = {50., 40.});

            ~DirButton() = default;
            DirButton(const DirButton&) = delete;
            DirButton& operator=(const DirButton&) = delete;
            DirButton(DirButton&&) noexcept = default;
            DirButton& operator=(DirButton&&) noexcept = default;

            void draw(sf::Vector2f pos, sf::RenderWindow& window) final;
            std::string getDirPath() const noexcept;
            float getGlobalX() const noexcept;
    };
}

#endif /* DIRBUTTON_HPP_ */
