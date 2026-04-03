/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** RenameDialog.hpp
*/

#ifndef RENAME_DIALOG_HPP_
    #define RENAME_DIALOG_HPP_

#include "RoundedRectangleShape.hpp"
#include "constants.hpp"
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace fe {
    class RenameDialog {
        std::unique_ptr<RoundedRectangleShape> _dialogRect;
        std::unique_ptr<RoundedRectangleShape> _inputRect;
        std::unique_ptr<sf::Text> _titleText;
        std::unique_ptr<sf::Text> _hintText;
        std::unique_ptr<sf::Text> _inputText;
        std::unique_ptr<sf::Text> _errorText;
        bool _open = false;
        std::string _target;
        std::string _input;
        std::string _error;

        void refreshLayout(const sf::RenderWindow& window);

        public:
            enum class EventResult {
                Ignored,
                Consumed,
                Submit
            };

            explicit RenameDialog(sf::Font& font);

            void open(const std::string& target);
            void close();
            bool isOpen() const noexcept;
            EventResult handleEvent(const sf::Event& event);
            void draw(sf::RenderWindow& window);
            void setError(const std::string& error);
            const std::string& getTarget() const noexcept;
            const std::string& getInput() const noexcept;
    };
}

#endif /* RENAME_DIALOG_HPP_ */
