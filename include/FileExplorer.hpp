/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** FileExplorer.hpp
*/

#ifndef FILE_EXPLORER_HPP_
    #define FILE_EXPLORER_HPP_

    #define SUCCESS 0
    #define FAILURE 84

    #define FONT_PATH "resources/Arial Regular.ttf"
    #define TEXT_SIZE 20
    #define FILE_SEP_SIZE 15

#include "FileBar.hpp"
#include <dirent.h>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace fe
{
    class FileExplorer {
        std::string _windowName;
        sf::Color _bgColor = sf::Color(45, 44, 43);
        bool _showHidden = false;

        std::unique_ptr<sf::RenderWindow> _window;
        std::unique_ptr<sf::Font> _font;
        std::unique_ptr<sf::Text> _text;

        std::string _dirPath;
        DIR* _dir;
        std::vector<std::unique_ptr<fe::FileBar>> _entries;
        std::string _selectedFile;

        void init();
        void getEntries();

        public:
            FileExplorer(std::string windowName);
            ~FileExplorer() = default;
            std::ifstream open();
    };
}

#endif /* FILE_EXPLORER_HPP_ */
