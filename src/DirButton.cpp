/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** DirButton.cpp
*/

#include "DirButton.hpp"
#include "constants.hpp"

fe::DirButton::DirButton(std::string dirPath, sf::Font& font, sf::Vector2f size)
    : _dirPath(dirPath), Button(size)
{
    this->_text = std::make_unique<sf::Text>();
    this->_text->setFont(font);
    this->_text->setCharacterSize(TEXT_SIZE);
    this->_text->setFillColor(sf::Color::White);
    this->_text->setString(this->_dirPath.substr(this->_dirPath.find_last_of('/') + 1));

    float x = this->_text->getGlobalBounds().getSize().x;
    this->_rect->setSize(sf::Vector2f(x + 10, size.y));
}

std::string fe::DirButton::getDirPath() const noexcept
{
    return this->_dirPath;
}

float fe::DirButton::getGlobalX() const noexcept
{
    return this->_rect->getGlobalBounds().getSize().x;
}

void fe::DirButton::update(sf::RenderWindow& window)
{
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    if (mouseWorld.y >= PWD_RECT_SIZE_Y) {
        this->_hover = false;
        this->_rect->setFillColor(this->_color);
        return;
    }
    if (this->_rect->getGlobalBounds().contains(mouseWorld)) {
        this->_hover = true;
        this->_rect->setFillColor(this->_hoverColor);
    } else {
        this->_hover = false;
        this->_rect->setFillColor(this->_color);
    }
}

void fe::DirButton::draw(sf::Vector2f pos, sf::RenderWindow& window)
{
    this->_rect->setPosition(pos);
    window.draw(*this->_rect.get());

    this->_text->setPosition(sf::Vector2f(pos.x + 5, pos.y + 3));
    window.draw(*this->_text);
}
