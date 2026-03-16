/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** Button.hpp
*/

#ifndef BUTTON_HPP_
    #define BUTTON_HPP_

#include "IElement.hpp"
#include "RoundedRectangleShape.hpp"
#include <memory>

namespace fe {
    class Button : public IElement {
        protected:
            std::unique_ptr<RoundedRectangleShape> _rect;
            bool _hover = false;
            sf::Color _color;
            sf::Color _hoverColor;

        public:
            Button(sf::Vector2f size, sf::Color color = sf::Color(75, 74, 73), sf::Color hoverColor = sf::Color(105, 104, 103));
            virtual void update(sf::RenderWindow& window) override;
            virtual void draw(sf::Vector2f pos, sf::RenderWindow& window) override;

            bool getHover() const noexcept;
    };
}

#endif /* BUTTON_HPP_ */
