/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** PropertiesDialog.hpp
*/

#ifndef PROPERTIES_DIALOG_HPP_
    #define PROPERTIES_DIALOG_HPP_

#include "RoundedRectangleShape.hpp"
#include "constants.hpp"
#include <array>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace fe {
    class PropertiesDialog {
        std::unique_ptr<RoundedRectangleShape> _dialogRect;
        std::unique_ptr<sf::Text> _titleText;
        std::unique_ptr<sf::Text> _hintText;
        std::array<std::unique_ptr<sf::Text>, 4> _propertyTexts;
        bool _open = false;

        void refreshLayout(const sf::RenderWindow& window);

        public:
            enum class EventResult {
                Ignored,
                Consumed
            };

            PropertiesDialog(sf::Font& font);

            void open(const std::array<std::string, 4>& properties);
            void close();
            bool isOpen() const noexcept;
            EventResult handleEvent(const sf::Event& event);
            void draw(sf::RenderWindow& window);
    };
}

#endif /* PROPERTIES_DIALOG_HPP_ */
