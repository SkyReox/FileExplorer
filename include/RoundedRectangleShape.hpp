/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** RoundedRectangleShape.hpp
*/

#ifndef ROUNDED_RECTANGLE_SHAPE_HPP
    #define ROUNDED_RECTANGLE_SHAPE_HPP

#include <cmath>
#include <SFML/Graphics.hpp>

class RoundedRectangleShape : public sf::Shape
{
    sf::Vector2f _size;
    float _radius;
    std::size_t _cornerPointCount;

    public:
        RoundedRectangleShape(sf::Vector2f size, float radius, std::size_t cornerPointCount);

        virtual std::size_t getPointCount() const;
        virtual sf::Vector2f getPoint(std::size_t index) const;
        void setSize(sf::Vector2f size) { this->_size = size; update(); }
};

#endif /* ROUNDED_RECTANGLE_SHAPE_HPP */
