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
#include "RenameDialog.hpp"
#include "RoundedRectangleShape.hpp"
#include "TextButton.hpp"
#include <array>
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
        float _fileBarOffset = 0.f;

        std::unique_ptr<sf::RectangleShape> _pwdRect;
        std::unique_ptr<RoundedRectangleShape> _pwdBarRect;
        std::vector<std::unique_ptr<DirButton>> _pwdButtons;
        std::unique_ptr<TextButton> _sortButton;
        std::array<std::unique_ptr<TextButton>, 4> _sortMenuButtons;
        std::unique_ptr<RoundedRectangleShape> _sortMenuRect;
        bool _sortMenuOpen = false;
        std::array<std::unique_ptr<TextButton>, 5> _contextMenuButtons;
        std::unique_ptr<RoundedRectangleShape> _contextMenuRect;
        bool _contextMenuOpen = false;
        std::string _contextMenuTarget;
        std::unique_ptr<RenameDialog> _renameDialog;

        void init();
        void getEntries();
        bool handleEvents(std::ifstream& res);
        void update();
        void display();
        void initSortControls();
        void initContextMenu();
        void refreshSortLayout();
        void refreshContextMenuLayout(const sf::Vector2f& mousePos);
        bool handleSortClick(const sf::Vector2f& mouseWorld);
        bool handleContextMenuClick(const sf::Vector2f& mousePos);
        bool isPointInsideSortMenu(const sf::Vector2f& mouseWorld) const;
        bool isPointInsideContextMenu(const sf::Vector2f& mousePos) const;
        void openRenameDialog();
        bool submitRename();

        public:
            enum SortType {
                Name,
                Size
            };

            FileExplorer(std::string windowName);
            ~FileExplorer() = default;
            std::ifstream open();

        private:
            SortType _sortType = SortType::Name;
            bool _ascending = true;
            void sortEntries(SortType type, bool ascending = true);
    };
}

#endif /* FILE_EXPLORER_HPP_ */
