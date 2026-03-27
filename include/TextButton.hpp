/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** TextButton.hpp
*/

#ifndef TEXT_BUTTON_HPP_
    #define TEXT_BUTTON_HPP_

#include "Button.hpp"
#include <string>

namespace fe {
    class TextButton : public Button {
        std::unique_ptr<sf::Text> _text;
        std::string _label;

        public:
            TextButton(const std::string& label, sf::Font& font, sf::Vector2f size);

            ~TextButton() = default;
            TextButton(const TextButton&) = delete;
            TextButton& operator=(const TextButton&) = delete;
            TextButton(TextButton&&) noexcept = default;
            TextButton& operator=(TextButton&&) noexcept = default;

            void draw(sf::Vector2f pos, sf::RenderWindow& window) final;
            sf::FloatRect getGlobalBounds() const noexcept;
            void setPosition(sf::Vector2f pos);
    };
}

#endif /* TEXT_BUTTON_HPP_ */
