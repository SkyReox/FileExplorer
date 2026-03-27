/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** FileBar.cpp
*/

#include "FileBar.hpp"
#include <filesystem>

namespace fs = std::filesystem;

fe::FileBar::FileBar(struct dirent* file, const std::string& parentPath, sf::Font& font, sf::Vector2f size)
    : _fileName(file->d_name), _path((fs::path(parentPath) / file->d_name).string()), Button(size)
{
    this->_text = std::make_unique<sf::Text>();
    this->_text->setFont(font);
    this->_text->setCharacterSize(TEXT_SIZE);
    this->_text->setFillColor(sf::Color::White);
}

std::string fe::FileBar::getFileName() const noexcept
{
    return this->_fileName;
}

bool fe::FileBar::isDirectory() const
{
    return fs::is_directory(this->_path);
}

std::size_t fe::FileBar::getFileSize() const
{
    if (!fs::exists(this->_path))
        return 0;

    if (fs::is_regular_file(this->_path))
        return static_cast<std::size_t>(fs::file_size(this->_path));

    std::uintmax_t size = 0;

    for (const auto& entry : fs::recursive_directory_iterator(this->_path)) {
        if (fs::is_regular_file(entry.path()))
            size += fs::file_size(entry.path());
    }

    return static_cast<std::size_t>(size);
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

    this->_text->setString(this->_fileName);
    this->_text->setPosition(sf::Vector2f(pos.x + 5, pos.y + 3));
    window.draw(*this->_text);
}
