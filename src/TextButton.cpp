/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** TextButton.cpp
*/

#include "TextButton.hpp"
#include "constants.hpp"

fe::TextButton::TextButton(const std::string& label, sf::Font& font, sf::Vector2f size)
    : Button(size), _label(label)
{
    this->_text = std::make_unique<sf::Text>();
    this->_text->setFont(font);
    this->_text->setCharacterSize(TEXT_SIZE);
    this->_text->setFillColor(sf::Color::White);
    this->_text->setString(this->_label);
}

void fe::TextButton::setPosition(sf::Vector2f pos)
{
    this->_rect->setPosition(pos);

    const sf::FloatRect textBounds = this->_text->getLocalBounds();
    const sf::FloatRect buttonBounds = this->_rect->getGlobalBounds();

    this->_text->setPosition(
        pos.x + (buttonBounds.width - textBounds.width) / 2.f - textBounds.left,
        pos.y + (buttonBounds.height - textBounds.height) / 2.f - textBounds.top - 1.f
    );
}

void fe::TextButton::draw(sf::Vector2f pos, sf::RenderWindow& window)
{
    this->setPosition(pos);
    window.draw(*this->_rect);
    window.draw(*this->_text);
}

sf::FloatRect fe::TextButton::getGlobalBounds() const noexcept
{
    return this->_rect->getGlobalBounds();
}
