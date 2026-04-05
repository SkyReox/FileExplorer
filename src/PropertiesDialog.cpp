/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** PropertiesDialog.cpp
*/

#include "constants.hpp"
#include "PropertiesDialog.hpp"

fe::PropertiesDialog::PropertiesDialog(sf::Font& font)
{
    this->_dialogRect = std::make_unique<RoundedRectangleShape>(sf::Vector2f(PROPERTIES_DIALOG_WIDTH, PROPERTIES_DIALOG_HEIGHT), 12.f, 8);
    this->_dialogRect->setFillColor(sf::Color(55, 55, 55));
    this->_dialogRect->setOutlineColor(sf::Color(110, 110, 110));
    this->_dialogRect->setOutlineThickness(1.f);

    this->_titleText = std::make_unique<sf::Text>("Properties", font, TEXT_SIZE + 2);
    this->_hintText = std::make_unique<sf::Text>("Escape or click to close", font, 16);
    this->_titleText->setFillColor(sf::Color::White);
    this->_hintText->setFillColor(sf::Color(200, 200, 200));

    for (std::size_t i = 0; i < this->_propertyTexts.size(); i++) {
        this->_propertyTexts[i] = std::make_unique<sf::Text>("", font, 18);
        this->_propertyTexts[i]->setFillColor(sf::Color::White);
    }
}

void fe::PropertiesDialog::refreshLayout(const sf::RenderWindow& window)
{
    const sf::Vector2u windowSize = window.getSize();
    const sf::Vector2f dialogPos((windowSize.x - PROPERTIES_DIALOG_WIDTH) / 2, (windowSize.y - PROPERTIES_DIALOG_HEIGHT) / 2);

    this->_dialogRect->setPosition(dialogPos);
    this->_titleText->setPosition(dialogPos.x + 20.f, dialogPos.y + 14.f);
    this->_hintText->setPosition(dialogPos.x + 20.f, dialogPos.y + 214.f);
    for (std::size_t i = 0; i < this->_propertyTexts.size(); i++)
        this->_propertyTexts[i]->setPosition(dialogPos.x + 20.f, dialogPos.y + 58.f + static_cast<float>(i) * 34.f);
}

void fe::PropertiesDialog::open(const std::array<std::string, 4>& properties)
{
    for (std::size_t i = 0; i < this->_propertyTexts.size(); i++)
        this->_propertyTexts[i]->setString(properties[i]);
    this->_open = true;
}

void fe::PropertiesDialog::close()
{
    this->_open = false;
}

bool fe::PropertiesDialog::isOpen() const noexcept
{
    return this->_open;
}

fe::PropertiesDialog::EventResult fe::PropertiesDialog::handleEvent(const sf::Event& event)
{
    if (!this->_open)
        return EventResult::Ignored;
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        this->close();
        return EventResult::Consumed;
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        this->close();
        return EventResult::Consumed;
    }
    return EventResult::Consumed;
}

void fe::PropertiesDialog::draw(sf::RenderWindow& window)
{
    if (!this->_open)
        return;

    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));

    overlay.setFillColor(sf::Color(0, 0, 0, 120));
    window.draw(overlay);

    this->refreshLayout(window);
    window.draw(*this->_dialogRect);
    window.draw(*this->_titleText);
    window.draw(*this->_hintText);
    for (const auto& propertyText : this->_propertyTexts)
        window.draw(*propertyText);
}
