/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** FileExplorer.cpp
*/

#include "FileBar.hpp"
#include "FileExplorer.hpp"
#include <dirent.h>
#include <exception>
#include <iostream>
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
    this->_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600, 32), this->_windowName, sf::Style::Titlebar | sf::Style::Close);
}

void fe::FileExplorer::getEntries()
{
    struct dirent* entry;

    while ((entry = readdir(this->_dir)) != nullptr) {
        std::string entryName = std::string(entry->d_name);
        if (entryName == "." || entryName == "..")
            continue;
        this->_entries.push_back(std::make_unique<fe::FileBar>(entry));
    }
}

std::ifstream fe::FileExplorer::open()
{
    std::ifstream res;
    this->_dirPath = std::getenv("PWD");

    // Open Directory from environment
    if (this->_dirPath.empty())
        throw std::runtime_error("'PWD' isn't set in the environment");
    this->_dir = opendir(this->_dirPath.c_str());
    if (!this->_dir)
        throw std::runtime_error("Couldn't open 'PWD' directory");
    this->getEntries();

    // Main Loop
    this->init();
    while (this->_window->isOpen()) {
        // Events
        sf::Event event;
        while (this->_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->_window->close();
                continue;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (std::size_t i = 0; i < this->_entries.size(); i++) {
                    if (this->_entries[i]->getHover()) {
                        if (!this->_entries[i]->isDirectory()) {
                            res.open(this->_dirPath + "/" + this->_entries[i]->getFileName());
                            this->_window->close();
                            break;
                        }
                    }
                }
            }
        }

        // Show
        this->_window->clear(this->_bgColor);

        for (std::size_t i = 0; i < this->_entries.size(); i++) {
            this->_text->setPosition(sf::Vector2f(10, 10 + i * (TEXT_SIZE + FILE_SEP_SIZE)));
            this->_entries[i]->draw(sf::Vector2f(0, FILE_SEP_SIZE / 2 + i * (TEXT_SIZE + FILE_SEP_SIZE)), *this->_text, *this->_window);
        }

        this->_window->display();
    }

    closedir(this->_dir);
    return res;
}
