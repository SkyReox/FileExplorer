/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** IElement.hpp
*/

#ifndef IELEMENT_HPP_
    #define IELEMENT_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace fe {
    class IElement {
        public:
            virtual void update(sf::RenderWindow& window) = 0;
            virtual void draw(sf::Vector2f pos, sf::RenderWindow& window) = 0;
    };
}

#endif /* IELEMENT_HPP_ */
