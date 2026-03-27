/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** FileBar.cpp
*/

#include "FileBar.hpp"
#include <algorithm>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

static sf::Texture& getDirectoryTexture()
{
    static sf::Texture texture;
    static bool isLoaded = texture.loadFromFile(DIRECTORY_ICON_PATH);

    if (!isLoaded)
        throw std::runtime_error("Directory icon couldn't be loaded");
    return texture;
}

fe::FileBar::FileBar(struct dirent* file, const std::string& parentPath, sf::Font& font, sf::Vector2f size)
    : Button(size), _fileName(file->d_name), _path((fs::path(parentPath) / file->d_name).string()), _isDirectory(fs::is_directory(_path))
{
    this->_text = std::make_unique<sf::Text>();
    this->_text->setFont(font);
    this->_text->setCharacterSize(TEXT_SIZE);
    this->_text->setFillColor(sf::Color::White);

    if (this->_isDirectory) {
        sf::Texture& texture = getDirectoryTexture();

        this->_directoryIcon = std::make_unique<sf::Sprite>(texture);
        const sf::Vector2u textureSize = texture.getSize();

        if (textureSize.x > 0 && textureSize.y > 0) {
            const float maxDimension = static_cast<float>(std::max(textureSize.x, textureSize.y));
            const float scale = FILEBAR_ICON_SIZE / maxDimension;

            this->_directoryIcon->setScale(scale, scale);
        }
    }
}

std::string fe::FileBar::getFileName() const noexcept
{
    return this->_fileName;
}

bool fe::FileBar::isDirectory() const
{
    return this->_isDirectory;
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
    float textX = pos.x + FILEBAR_TEXT_PADDING;

    if (this->_isDirectory && this->_directoryIcon) {
        const sf::FloatRect buttonBounds = this->_rect->getGlobalBounds();
        const sf::FloatRect iconBounds = this->_directoryIcon->getGlobalBounds();

        this->_directoryIcon->setPosition(
            pos.x + FILEBAR_TEXT_PADDING,
            pos.y + (buttonBounds.height - iconBounds.height) / 2.f
        );
        textX += this->_directoryIcon->getGlobalBounds().width + FILEBAR_ICON_GAP;
        window.draw(*this->_directoryIcon);
    }

    const sf::FloatRect textBounds = this->_text->getLocalBounds();

    this->_text->setPosition(sf::Vector2f(
        textX,
        pos.y + (this->_rect->getGlobalBounds().height - textBounds.height) / 2.f - textBounds.top - 1.f
    ));
    window.draw(*this->_text);
}
