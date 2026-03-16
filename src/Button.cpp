/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** Button.cpp
*/

#include "Button.hpp"
#include "constants.hpp"

fe::Button::Button(sf::Vector2f size, sf::Color color, sf::Color hoverColor) : _color(color), _hoverColor(hoverColor)
{
    this->_rect = std::make_unique<RoundedRectangleShape>(size, 10., 8);
}

bool fe::Button::getHover() const noexcept
{
    return this->_hover;
}

void fe::Button::update(sf::RenderWindow& window)
{
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    if (this->_rect->getGlobalBounds().contains(mouseWorld)) {
        this->_hover = true;
        this->_rect->setFillColor(this->_hoverColor);
    } else {
        this->_hover = false;
        this->_rect->setFillColor(this->_color);
    }
}

void fe::Button::draw(sf::Vector2f pos, sf::RenderWindow& window)
{
    this->_rect->setPosition(pos);
    window.draw(*this->_rect.get());
}
