/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** FileBar.cpp
*/

#include "FileBar.hpp"

fe::FileBar::FileBar(struct dirent* file, sf::Font& font, sf::Vector2f size)
    : _file(file), Button(size)
{
    this->_text = std::make_unique<sf::Text>();
    this->_text->setFont(font);
    this->_text->setCharacterSize(TEXT_SIZE);
    this->_text->setFillColor(sf::Color::White);
}

std::string fe::FileBar::getFileName() const noexcept
{
    return this->_file->d_name;
}

bool fe::FileBar::isDirectory() const
{
    if (this->_file->d_type == DT_DIR)
        return true;
    return false;
}

void fe::FileBar::update(sf::RenderWindow& window)
{
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    if (mouseWorld.y < PWD_RECT_SIZE_Y) {
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

void fe::FileBar::draw(sf::Vector2f pos, sf::RenderWindow& window)
{
    this->_rect->setPosition(pos);
    window.draw(*this->_rect.get());

    this->_text->setString(this->_file->d_name);
    this->_text->setPosition(sf::Vector2f(pos.x + 5, pos.y + 3));
    window.draw(*this->_text);
}
