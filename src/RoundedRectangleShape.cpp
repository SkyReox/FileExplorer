/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** RoundedRectangleShape.cpp
*/

#include "RoundedRectangleShape.hpp"

RoundedRectangleShape::RoundedRectangleShape(sf::Vector2f size, float radius, std::size_t cornerPointCount) : _size(size), _radius(radius), _cornerPointCount(cornerPointCount)
{
    update();
}

std::size_t RoundedRectangleShape::getPointCount() const
{
    return _cornerPointCount * 4;
}

sf::Vector2f RoundedRectangleShape::getPoint(std::size_t index) const
{
    std::size_t corner = index / _cornerPointCount;
    float angle = (index % _cornerPointCount) * 90.f / (_cornerPointCount - 1);

    sf::Vector2f center;

    switch (corner)
{
    case 0: // top-left
        center = {_radius, _radius};
        angle += 180;
        break;

    case 1: // top-right
        center = {_size.x - _radius, _radius};
        angle += 270;
        break;

    case 2: // bottom-right
        center = {_size.x - _radius, _size.y - _radius};
        angle += 0;
        break;

    case 3: // bottom-left
        center = {_radius, _size.y - _radius};
        angle += 90;
        break;
}

    float rad = angle * 3.141592654f / 180.f;

    return {
        center.x + std::cos(rad) * _radius,
        center.y + std::sin(rad) * _radius
    };
}
