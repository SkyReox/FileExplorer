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
#include <array>
#include <dirent.h>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace {
    const std::array<std::string, 5> CONTEXT_MENU_LABELS = {{
        "Rename",
        "Copy",
        "Cut",
        "Move To Trash",
        "Properties"
    }};
}

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

    this->_pwdBarRect = std::make_unique<RoundedRectangleShape>(sf::Vector2f(this->_window->getSize().x - (PWD_OFFSET * 2 + SORT_BUTTON_WIDTH), 40), 10., 8);
    this->_pwdBarRect->setFillColor(sf::Color(80, 80, 80));
    this->_pwdBarRect->setPosition(sf::Vector2f(PWD_OFFSET, 5));
    this->initSortControls();
    this->initContextMenu();
    this->_renameDialog = std::make_unique<fe::RenameDialog>(*this->_font);

    this->getEntries();
}

void fe::FileExplorer::initSortControls()
{
    this->_sortButton = std::make_unique<TextButton>(
        "Sort",
        *this->_font,
        sf::Vector2f(SORT_BUTTON_WIDTH, SORT_BUTTON_HEIGHT)
    );
    this->_sortMenuRect = std::make_unique<RoundedRectangleShape>(
        sf::Vector2f(SORT_MENU_WIDTH, SORT_MENU_HEIGHT),
        10.f,
        8
    );
    this->_sortMenuRect->setFillColor(sf::Color(65, 65, 65));
    this->_sortMenuRect->setOutlineColor(sf::Color(100, 100, 100));
    this->_sortMenuRect->setOutlineThickness(1.f);

    const std::array<std::string, 4> labels = {
        "Name",
        "Size",
        "Ascending",
        "Descending"
    };

    for (std::size_t i = 0; i < this->_sortMenuButtons.size(); i++)
        this->_sortMenuButtons[i] = std::make_unique<TextButton>(labels[i], *this->_font, sf::Vector2f(150.f, SORT_BUTTON_HEIGHT));
    this->refreshSortLayout();
}

void fe::FileExplorer::initContextMenu()
{
    this->_contextMenuRect = std::make_unique<RoundedRectangleShape>(sf::Vector2f(CONTEXT_MENU_WIDTH, CONTEXT_MENU_HEIGHT), 10, 8);
    this->_contextMenuRect->setFillColor(sf::Color(65, 65, 65));
    this->_contextMenuRect->setOutlineColor(sf::Color(100, 100, 100));
    this->_contextMenuRect->setOutlineThickness(1);

    for (std::size_t i = 0; i < this->_contextMenuButtons.size(); i++)
        this->_contextMenuButtons[i] = std::make_unique<TextButton>(
            CONTEXT_MENU_LABELS[i],
            *this->_font,
            sf::Vector2f(170, SORT_BUTTON_HEIGHT)
        );
}

void fe::FileExplorer::refreshSortLayout()
{
    const sf::Vector2f barPos = this->_pwdBarRect->getPosition();
    const sf::FloatRect barBounds = this->_pwdBarRect->getGlobalBounds();
    const float buttonX = barPos.x + barBounds.width + SORT_BUTTON_GAP;
    const float buttonY = barPos.y + (barBounds.height - SORT_BUTTON_HEIGHT) / 2.f;

    this->_sortButton->setPosition(sf::Vector2f(buttonX, buttonY));
    this->_sortMenuRect->setPosition(sf::Vector2f(buttonX + SORT_BUTTON_WIDTH - this->_sortMenuRect->getGlobalBounds().width, barPos.y + barBounds.height + 5.f));

    const sf::Vector2f menuPos = this->_sortMenuRect->getPosition();
    for (std::size_t i = 0; i < this->_sortMenuButtons.size(); i++)
        this->_sortMenuButtons[i]->setPosition(sf::Vector2f(menuPos.x + 10.f, menuPos.y + 10.f + i * 36.f));
}

void fe::FileExplorer::refreshContextMenuLayout(const sf::Vector2f& mousePos)
{
    sf::Vector2f menuPos = mousePos;
    const sf::Vector2u windowSize = this->_window->getSize();
    const sf::FloatRect menuBounds = this->_contextMenuRect->getGlobalBounds();

    if (menuPos.x + menuBounds.width > static_cast<float>(windowSize.x))
        menuPos.x = static_cast<float>(windowSize.x) - menuBounds.width - 5.f;
    if (menuPos.y + menuBounds.height > static_cast<float>(windowSize.y))
        menuPos.y = static_cast<float>(windowSize.y) - menuBounds.height - 5.f;
    if (menuPos.x < 0.f)
        menuPos.x = 0.f;
    if (menuPos.y < 0.f)
        menuPos.y = 0.f;

    this->_contextMenuRect->setPosition(menuPos);
    for (std::size_t i = 0; i < this->_contextMenuButtons.size(); i++)
        this->_contextMenuButtons[i]->setPosition(sf::Vector2f(menuPos.x + 10.f, menuPos.y + 10.f + i * 36.f));
}

void fe::FileExplorer::getEntries()
{
    struct dirent* entry;
    const char *homeEnv = std::getenv("HOME");
    const auto getPwdButtonsWidth = [this]() {
        float totalWidth = 0.f;

        for (const auto& button : this->_pwdButtons)
            totalWidth += button->getGlobalX();
        if (!this->_pwdButtons.empty())
            totalWidth += static_cast<float>(this->_pwdButtons.size() - 1) * PWD_BUTTON_SEP;
        return totalWidth;
    };

    if (!homeEnv)
        throw std::runtime_error("'HOME' isn't set in the environment");
    std::string home = homeEnv;

    // Get Entries
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
        this->_entries.push_back(std::make_unique<fe::FileBar>(entry, this->_dirPath, *this->_font, fileBarSize));
    }
    this->sortEntries(this->_sortType, this->_ascending);

    // Dir Buttons
    if (this->_dirPath.rfind(home, 0) == std::string::npos)
        return;
    sf::Vector2f pwdButtonPos(75, this->_pwdBarRect->getGlobalBounds().getSize().y - 10);
    this->_pwdButtons.push_back(std::make_unique<DirButton>(home, *this->_font, pwdButtonPos));

    std::string dir = home;
    auto nDirs = std::count(this->_dirPath.begin(), this->_dirPath.end(), '/');
    const float availablePwdWidth = this->_pwdBarRect->getGlobalBounds().getSize().x - 20.f;
    for (std::size_t i = 2; i < nDirs; i++) {
        pwdButtonPos.x += this->_pwdButtons[this->_pwdButtons.size() - 1]->getGlobalX() + 10;

        std::string nextDirs = std::string(&this->_dirPath[dir.size() + 1]);
        auto slashPos = nextDirs.find('/');
        if (slashPos <= nextDirs.size())
            dir = this->_dirPath.substr(0, dir.size() + slashPos + 1);
        else
            dir = this->_dirPath;

        this->_pwdButtons.push_back(std::make_unique<DirButton>(dir, *this->_font, pwdButtonPos));
        while (this->_pwdButtons.size() > 1 && getPwdButtonsWidth() > availablePwdWidth)
            this->_pwdButtons.erase(this->_pwdButtons.begin());
    }
}

static std::string toLowerString(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return str;
}

void fe::FileExplorer::sortEntries(SortType type, bool ascending)
{
    switch (type) {
        case SortType::Name:
            std::sort(this->_entries.begin(), this->_entries.end(),
                [ascending](const auto& a, const auto& b) {
                    std::string aName = toLowerString(a->getFileName());
                    std::string bName = toLowerString(b->getFileName());

                    return ascending ? (aName < bName) : (aName > bName);
                });
            break;
        case SortType::Size:
            std::sort(this->_entries.begin(), this->_entries.end(),
                [ascending](const auto& a, const auto& b) {
                    std::size_t aSize = a->getFileSize();
                    std::size_t bSize = b->getFileSize();

                    return ascending ? (aSize < bSize) : (aSize > bSize);
                });
            break;
    }
}

bool fe::FileExplorer::isPointInsideSortMenu(const sf::Vector2f& mousePos) const
{
    if (!this->_sortMenuOpen)
        return false;
    if (this->_sortButton->getGlobalBounds().contains(mousePos))
        return true;
    return this->_sortMenuRect->getGlobalBounds().contains(mousePos);
}

bool fe::FileExplorer::isPointInsideContextMenu(const sf::Vector2f& mousePos) const
{
    if (!this->_contextMenuOpen)
        return false;
    return this->_contextMenuRect->getGlobalBounds().contains(mousePos);
}

void fe::FileExplorer::openRenameDialog()
{
    this->_renameDialog->open(this->_contextMenuTarget);
    this->_contextMenuOpen = false;
}

bool fe::FileExplorer::submitRename()
{
    const std::string& renameInput = this->_renameDialog->getInput();
    const std::string& renameTarget = this->_renameDialog->getTarget();

    if (renameInput.empty()) {
        this->_renameDialog->setError("Name can't be empty");
        return false;
    }
    if (renameInput.find('/') != std::string::npos) {
        this->_renameDialog->setError("Name can't contain '/'");
        return false;
    }
    if (renameInput == renameTarget) {
        this->_renameDialog->close();
        return true;
    }

    const std::filesystem::path sourcePath = std::filesystem::path(this->_dirPath) / renameTarget;
    const std::filesystem::path targetPath = std::filesystem::path(this->_dirPath) / renameInput;

    if (std::filesystem::exists(targetPath)) {
        this->_renameDialog->setError("A file with this name already exists");
        return false;
    }
    try {
        std::filesystem::rename(sourcePath, targetPath);
    } catch (const std::exception& e) {
        this->_renameDialog->setError(e.what());
        return false;
    }

    this->_renameDialog->close();
    closedir(this->_dir);
    this->getEntries();
    return true;
}

bool fe::FileExplorer::handleSortClick(const sf::Vector2f& mousePos)
{
    const std::array<std::pair<SortType, bool>, 4> actions = {{
        {SortType::Name, this->_ascending},
        {SortType::Size, this->_ascending},
        {this->_sortType, true},
        {this->_sortType, false}
    }};

    if (this->_sortButton->getGlobalBounds().contains(mousePos)) {
        this->_sortMenuOpen = !this->_sortMenuOpen;
        return true;
    }
    if (!this->_sortMenuOpen)
        return false;
    for (std::size_t i = 0; i < this->_sortMenuButtons.size(); i++) {
        if (!this->_sortMenuButtons[i]->getGlobalBounds().contains(mousePos))
            continue;
        this->_sortType = actions[i].first;
        this->_ascending = actions[i].second;
        this->sortEntries(this->_sortType, this->_ascending);
        this->_sortMenuOpen = false;
        return true;
    }
    if (this->isPointInsideSortMenu(mousePos))
        return true;
    this->_sortMenuOpen = false;
    return true;
}

bool fe::FileExplorer::handleContextMenuClick(const sf::Vector2f& mousePos)
{
    if (!this->_contextMenuOpen)
        return false;
    for (std::size_t i = 0; i < this->_contextMenuButtons.size(); i++) {
        if (!this->_contextMenuButtons[i]->getGlobalBounds().contains(mousePos))
            continue;
        if (i == 0) {
            this->openRenameDialog();
            return true;
        }
        std::cout << "[DEBUG] " << CONTEXT_MENU_LABELS[i] << " clicked for " << this->_contextMenuTarget << std::endl;
        this->_contextMenuOpen = false;
        return true;
    }
    if (this->isPointInsideContextMenu(mousePos))
        return true;
    this->_contextMenuOpen = false;
    return true;
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
        switch (this->_renameDialog->handleEvent(event)) {
            case fe::RenameDialog::EventResult::Submit:
                this->submitRename();
                continue;
            case fe::RenameDialog::EventResult::Consumed:
                continue;
            case fe::RenameDialog::EventResult::Ignored:
                break;
        }

        // Mouse Button
        if (event.type == sf::Event::MouseButtonPressed)
            if (event.mouseButton.button == sf::Mouse::Left) {
                const sf::Vector2f mousePos = this->_window->mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
                );

                if (this->handleContextMenuClick(mousePos))
                    continue;
                if (this->handleSortClick(mousePos))
                    continue;
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
                        if (this->_pwdButtons[i]->getDirPath() == this->_dirPath)
                            return false;
                        closedir(this->_dir);
                        this->_dirPath = this->_pwdButtons[i]->getDirPath();
                        this->getEntries();
                        return false;
                    }
                }
            }
            if (event.mouseButton.button == sf::Mouse::Right) {
                const sf::Vector2f mousePos = this->_window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                int entryIndex = -1;

                for (std::size_t i = 0; i < this->_entries.size(); i++) {
                    if (!this->_entries[i]->getHover())
                        continue;
                    entryIndex = static_cast<int>(i);
                    break;
                }

                this->_sortMenuOpen = false;
                if (entryIndex < 0) {
                    this->_contextMenuOpen = false;
                    continue;
                }
                this->_contextMenuTarget = this->_entries[entryIndex]->getFileName();
                this->refreshContextMenuLayout(mousePos);
                this->_contextMenuOpen = true;
                continue;
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
    for (std::size_t i = 0; i < this->_entries.size(); i++) {
        float entryY = FILE_SEP_SIZE / 2 + i * (TEXT_SIZE + FILE_SEP_SIZE) + pwdRectY + this->_fileBarOffset;
        if (entryY < 0 || entryY >= this->_window->getSize().y)
            continue;
        this->_entries[i]->draw(sf::Vector2f(0, entryY), *this->_window);
    }

    // PWD Bar
    this->_window->draw(*this->_pwdRect);
    this->_window->draw(*this->_pwdBarRect);
    this->refreshSortLayout();
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
    this->_sortButton->draw(
        sf::Vector2f(this->_sortButton->getGlobalBounds().left, this->_sortButton->getGlobalBounds().top),
        *this->_window
    );
    if (this->_sortMenuOpen) {
        this->_window->draw(*this->_sortMenuRect);
        for (std::size_t i = 0; i < this->_sortMenuButtons.size(); i++)
            this->_sortMenuButtons[i]->draw(
                sf::Vector2f(this->_sortMenuButtons[i]->getGlobalBounds().left, this->_sortMenuButtons[i]->getGlobalBounds().top),
                *this->_window
            );
    }
    if (this->_contextMenuOpen) {
        this->_window->draw(*this->_contextMenuRect);
        for (std::size_t i = 0; i < this->_contextMenuButtons.size(); i++)
            this->_contextMenuButtons[i]->draw(
                sf::Vector2f(this->_contextMenuButtons[i]->getGlobalBounds().left, this->_contextMenuButtons[i]->getGlobalBounds().top),
                *this->_window
            );
    }
    this->_renameDialog->draw(*this->_window);

    this->_window->display();
}

void fe::FileExplorer::update()
{
    if (this->_renameDialog->isOpen())
        return;
    for (size_t i = 0; i < this->_entries.size(); i++)
        this->_entries[i]->update(*this->_window);
    for (size_t i = 0; i < this->_pwdButtons.size(); i++)
        this->_pwdButtons[i]->update(*this->_window);
    this->_sortButton->update(*this->_window);
    if (this->_sortMenuOpen)
        for (std::size_t i = 0; i < this->_sortMenuButtons.size(); i++)
            this->_sortMenuButtons[i]->update(*this->_window);
    if (this->_contextMenuOpen)
        for (std::size_t i = 0; i < this->_contextMenuButtons.size(); i++)
            this->_contextMenuButtons[i]->update(*this->_window);
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
