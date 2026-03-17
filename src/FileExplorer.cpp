/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** FileExplorer.cpp
*/

#include "Button.hpp"
#include "FileBar.hpp"
#include "FileExplorer.hpp"
#include <algorithm>
#include <dirent.h>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

fe::FileExplorer::FileExplorer(std::string windowName) : _windowName(windowName)
{
    this->_font = std::make_unique<sf::Font>();
    this->_text = std::make_unique<sf::Text>();

    if (!this->_font->loadFromFile(FONT_PATH))
        throw std::runtime_error("Font couldn't be loaded");
    this->_text->setFont(*this->_font);
    this->_text->setCharacterSize(TEXT_SIZE);
    this->_text->setFillColor(sf::Color::White);
}

void fe::FileExplorer::init()
{
    const char* pwd = std::getenv("PWD");
    if (!pwd)
        throw std::runtime_error("'PWD' isn't set in the environment");
    this->_dirPath = pwd;

    this->_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600, 32), this->_windowName, sf::Style::Titlebar | sf::Style::Close);
    this->_window->setFramerateLimit(FPS);

    this->_pwdRect = std::make_unique<sf::RectangleShape>(sf::Vector2f(this->_window->getSize().x, PWD_RECT_SIZE_Y));
    this->_pwdRect->setPosition(sf::Vector2f(0, 0));
    this->_pwdRect->setFillColor(this->_bgColor);
    this->_pwdRect->setOutlineColor(sf::Color(80, 80, 80));
    this->_pwdRect->setOutlineThickness(1);

    this->_pwdBarRect = std::make_unique<RoundedRectangleShape>(sf::Vector2f(this->_window->getSize().x - PWD_OFFSET * 2, 40), 10., 8);
    this->_pwdBarRect->setFillColor(sf::Color(80, 80, 80));
    this->_pwdBarRect->setPosition(sf::Vector2f(PWD_OFFSET, 5));

    this->getEntries();

}

void fe::FileExplorer::getEntries()
{
    struct dirent* entry;
    const char *homeEnv = std::getenv("HOME");

    if (!homeEnv)
        throw std::runtime_error("'HOME' isn't set in the environment");
    std::string home = homeEnv;

    this->_dir = opendir(this->_dirPath.c_str());
    if (!this->_dir)
        throw std::runtime_error("Couldn't open directory");
    this->_fileBarOffset = 0.f;
    this->_entries.clear();
    this->_pwdButtons.clear();
    sf::Vector2f fileBarSize(this->_window->getSize().x, TEXT_SIZE + FILE_SEP_SIZE * 0.8);
    while ((entry = readdir(this->_dir)) != nullptr) {
        std::string entryName = std::string(entry->d_name);
        if (entryName == "." || entryName == "..")
            continue;
        if (entryName[0] == '.' && !this->_showHidden)
            continue;
        this->_entries.push_back(std::make_unique<fe::FileBar>(entry, *this->_font, fileBarSize));
    }
    if (this->_dirPath.rfind(home, 0) == std::string::npos)
        return;
    sf::Vector2f pwdButtonPos(75, this->_pwdBarRect->getGlobalBounds().getSize().y - 10);
    this->_pwdButtons.push_back(std::make_unique<DirButton>(home, *this->_font, pwdButtonPos));

    std::string dir = home;
    auto nDirs = std::count(this->_dirPath.begin(), this->_dirPath.end(), '/');
    float pwdButtonsLength = this->_pwdButtons.back()->getGlobalX();
    for (std::size_t i = 2; i < nDirs; i++) {
        pwdButtonPos.x += this->_pwdButtons[this->_pwdButtons.size() - 1]->getGlobalX() + 10;

        std::string nextDirs = std::string(&this->_dirPath[dir.size() + 1]);
        auto slashPos = nextDirs.find('/');
        if (slashPos <= nextDirs.size())
            dir = this->_dirPath.substr(0, dir.size() + slashPos + 1);
        else
            dir = this->_dirPath;

        this->_pwdButtons.push_back(std::make_unique<DirButton>(dir, *this->_font, pwdButtonPos));
        pwdButtonsLength += this->_pwdButtons.back()->getGlobalX() + PWD_BUTTON_SEP;
        if (pwdButtonsLength > this->_window->getSize().x - PWD_OFFSET * 4) {
            pwdButtonsLength -= this->_pwdButtons.front()->getGlobalX();
            this->_pwdButtons.erase(this->_pwdButtons.begin());
        }
    }
}

bool fe::FileExplorer::handleEvents(std::ifstream& res)
{
    sf::Event event;
    while (this->_window->pollEvent(event)) {
        // Close Event
        if (event.type == sf::Event::Closed) {
            this->_window->close();
            return true;
        }

        // Mouse Button
        if (event.type == sf::Event::MouseButtonPressed)
            if (event.mouseButton.button == sf::Mouse::Left) {
                for (std::size_t i = 0; i < this->_entries.size(); i++)
                    if (this->_entries[i]->getHover()) {
                        if (!this->_entries[i]->isDirectory()) {
                            res.open(this->_dirPath + "/" + this->_entries[i]->getFileName());
                            this->_window->close();
                            return true;
                        } else {
                            closedir(this->_dir);
                            this->_dirPath += "/" + this->_entries[i]->getFileName();
                            this->getEntries();
                            return false;
                        }
                    }
                for (std::size_t i = 0; i < this->_pwdButtons.size(); i++) {
                    if (this->_pwdButtons[i]->getHover()) {
                        closedir(this->_dir);
                        this->_dirPath = this->_pwdButtons[i]->getDirPath();
                        this->getEntries();
                        return false;
                    }
                }
            }

        // Mouse Wheel
        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.wheel != sf::Mouse::VerticalWheel)
                continue;

            this->_fileBarOffset += event.mouseWheelScroll.delta * MOUSE_WHEEL_SENSITIVITY;

            const float visibleHeight = this->_window->getSize().y - this->_pwdRect->getSize().y;
            const float contentHeight = this->_entries.size() * (TEXT_SIZE + FILE_SEP_SIZE) + FILE_SEP_SIZE / 2.f;

            float minOffset = 0.f;
            if (contentHeight > visibleHeight)
                minOffset = visibleHeight - contentHeight;

            if (this->_fileBarOffset > 0.f)
                this->_fileBarOffset = 0.f;
            if (this->_fileBarOffset < minOffset)
                this->_fileBarOffset = minOffset;
        }
    }
    return false;
}

void fe::FileExplorer::display()
{
    this->_window->clear(this->_bgColor);

    // File Bars
    float pwdRectY = this->_pwdRect->getSize().y;
    for (std::size_t i = 0; i < this->_entries.size(); i++)
        this->_entries[i]->draw(sf::Vector2f(0, FILE_SEP_SIZE / 2 + i * (TEXT_SIZE + FILE_SEP_SIZE) + pwdRectY + this->_fileBarOffset), *this->_window);

    // PWD Bar
    this->_window->draw(*this->_pwdRect);
    this->_window->draw(*this->_pwdBarRect);
    this->_text->setString("/");
    float pwdButtonY = (this->_pwdBarRect->getGlobalBounds().getSize().y - this->_text->getGlobalBounds().getSize().y) / 2;
    float pwdOffset = PWD_OFFSET + 10;
    for (std::size_t i = 0; i < this->_pwdButtons.size(); i++) {
        this->_pwdButtons[i]->draw(sf::Vector2f(pwdOffset, this->_pwdBarRect->getPosition().y + 5), *this->_window);
        pwdOffset += this->_pwdButtons[i]->getGlobalX() + PWD_BUTTON_SEP;
        if (i != this->_pwdButtons.size() - 1) {
            this->_text->setPosition(sf::Vector2f(pwdOffset - (PWD_BUTTON_SEP + this->_text->getGlobalBounds().getSize().x) / 2, pwdButtonY));
            this->_window->draw(*this->_text);
        }
    }

    this->_window->display();
}

void fe::FileExplorer::update()
{
    for (size_t i = 0; i < this->_entries.size(); i++)
        this->_entries[i]->update(*this->_window);
    for (size_t i = 0; i < this->_pwdButtons.size(); i++)
        this->_pwdButtons[i]->update(*this->_window);
}

std::ifstream fe::FileExplorer::open()
{
    std::ifstream res;
    this->init();

    while (this->_window->isOpen()) {
        if (this->handleEvents(res))
            break;

        this->update();

        this->display();
    }

    closedir(this->_dir);
    return res;
}
