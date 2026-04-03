/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** RenameDialog.cpp
*/

#include "RenameDialog.hpp"

fe::RenameDialog::RenameDialog(sf::Font& font)
{
    this->_dialogRect = std::make_unique<RoundedRectangleShape>(sf::Vector2f(RENAME_DIALOG_WIDTH, RENAME_DIALOG_HEIGHT), 12.f, 8);
    this->_dialogRect->setFillColor(sf::Color(55, 55, 55));
    this->_dialogRect->setOutlineColor(sf::Color(110, 110, 110));
    this->_dialogRect->setOutlineThickness(1.f);

    this->_inputRect = std::make_unique<RoundedRectangleShape>(sf::Vector2f(RENAME_DIALOG_WIDTH - 40.f, RENAME_INPUT_HEIGHT), 8.f, 8);
    this->_inputRect->setFillColor(sf::Color(40, 40, 40));
    this->_inputRect->setOutlineColor(sf::Color(120, 120, 120));
    this->_inputRect->setOutlineThickness(1.f);

    this->_titleText = std::make_unique<sf::Text>("Rename", font, TEXT_SIZE + 2);
    this->_hintText = std::make_unique<sf::Text>("Enter to confirm, Escape to cancel", font, 16);
    this->_inputText = std::make_unique<sf::Text>("", font, TEXT_SIZE);
    this->_errorText = std::make_unique<sf::Text>("", font, 16);

    this->_titleText->setFillColor(sf::Color::White);
    this->_hintText->setFillColor(sf::Color(200, 200, 200));
    this->_inputText->setFillColor(sf::Color::White);
    this->_errorText->setFillColor(sf::Color(255, 120, 120));
}

void fe::RenameDialog::refreshLayout(const sf::RenderWindow& window)
{
    const sf::Vector2u windowSize = window.getSize();
    const sf::Vector2f dialogPos((static_cast<float>(windowSize.x) - RENAME_DIALOG_WIDTH) / 2.f, (static_cast<float>(windowSize.y) - RENAME_DIALOG_HEIGHT) / 2.f);

    this->_dialogRect->setPosition(dialogPos);
    this->_inputRect->setPosition(sf::Vector2f(dialogPos.x + 20.f, dialogPos.y + 58.f));
    this->_titleText->setPosition(dialogPos.x + 20.f, dialogPos.y + 14.f);
    this->_hintText->setPosition(dialogPos.x + 20.f, dialogPos.y + 112.f);
    this->_inputText->setPosition(dialogPos.x + 30.f, dialogPos.y + 66.f);
    this->_errorText->setPosition(dialogPos.x + 20.f, dialogPos.y + 136.f);
}

void fe::RenameDialog::open(const std::string& target)
{
    this->_target = target;
    this->_input = target;
    this->_error.clear();
    this->_open = true;
}

void fe::RenameDialog::close()
{
    this->_open = false;
    this->_target.clear();
    this->_input.clear();
    this->_error.clear();
}

bool fe::RenameDialog::isOpen() const noexcept
{
    return this->_open;
}

fe::RenameDialog::EventResult fe::RenameDialog::handleEvent(const sf::Event& event)
{
    if (!this->_open)
        return EventResult::Ignored;
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) {
            if (!this->_input.empty())
                this->_input.pop_back();
            this->_error.clear();
            return EventResult::Consumed;
        }
        if (event.text.unicode >= 32 && event.text.unicode <= 126) {
            this->_input += static_cast<char>(event.text.unicode);
            this->_error.clear();
            return EventResult::Consumed;
        }
        return EventResult::Consumed;
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return)
            return EventResult::Submit;
        if (event.key.code == sf::Keyboard::Escape) {
            this->close();
            return EventResult::Consumed;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed)
        return EventResult::Consumed;
    return EventResult::Ignored;
}

void fe::RenameDialog::draw(sf::RenderWindow& window)
{
    if (!this->_open)
        return;

    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));

    overlay.setFillColor(sf::Color(0, 0, 0, 120));
    window.draw(overlay);

    this->refreshLayout(window);
    this->_inputText->setString(this->_input);
    this->_errorText->setString(this->_error);

    window.draw(*this->_dialogRect);
    window.draw(*this->_inputRect);
    window.draw(*this->_titleText);
    window.draw(*this->_hintText);
    window.draw(*this->_inputText);
    window.draw(*this->_errorText);
}

void fe::RenameDialog::setError(const std::string& error)
{
    this->_error = error;
}

const std::string& fe::RenameDialog::getTarget() const noexcept
{
    return this->_target;
}

const std::string& fe::RenameDialog::getInput() const noexcept
{
    return this->_input;
}
