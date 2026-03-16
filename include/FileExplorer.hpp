/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** FileExplorer.hpp
*/

#ifndef FILE_EXPLORER_HPP_
    #define FILE_EXPLORER_HPP_

#include "Button.hpp"
#include "constants.hpp"
#include "DirButton.hpp"
#include "FileBar.hpp"
#include "RoundedRectangleShape.hpp"
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

        std::unique_ptr<sf::RectangleShape> _pwdRect;
        std::unique_ptr<RoundedRectangleShape> _pwdBarRect;
        std::vector<std::unique_ptr<DirButton>> _pwdButtons;

        void init();
        void getEntries();
        bool handleEvents(std::ifstream& res);
        void update();
        void display();

        public:
            FileExplorer(std::string windowName);
            ~FileExplorer() = default;
            std::ifstream open();
    };
}

#endif /* FILE_EXPLORER_HPP_ */
