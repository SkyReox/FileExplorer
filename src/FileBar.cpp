/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** FileBar.cpp
*/

#include "FileBar.hpp"
#include "FileExplorer.hpp"

fe::FileBar::FileBar(struct dirent* file) : _file(file)
{
    this->_rect = std::make_unique<sf::RectangleShape>(sf::Vector2f(sf::VideoMode::getDesktopMode().width, TEXT_SIZE + FILE_SEP_SIZE * 0.8));
}

std::string fe::FileBar::getFileName() const noexcept
{
    return this->_file->d_name;
}

bool fe::FileBar::getHover() const noexcept
{
    return this->_hover;
}

bool fe::FileBar::isDirectory() const
{
    if (this->_file->d_type == DT_DIR)
        return true;
    return false;
}

void fe::FileBar::draw(sf::Vector2f pos, sf::Text& text, sf::RenderWindow& window)
{
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    if (this->_rect->getGlobalBounds().contains(mouseWorld)) {
        this->_hover = true;
        this->_rect->setFillColor(sf::Color(105, 104, 103));
    } else {
        this->_hover = false;
        this->_rect->setFillColor(sf::Color(75, 74, 73));
    }

    this->_rect->setPosition(pos);
    window.draw(*this->_rect.get());

    text.setString(this->_file->d_name);
    window.draw(text);
}
