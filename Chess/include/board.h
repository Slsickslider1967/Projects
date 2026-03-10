#pragma once 

#include <SFML/Graphics.hpp>

using namespace sf;

struct board 
{
    char x[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    int y[8] = {1, 2, 3, 4, 5, 6, 7, 8};
};

RectangleShape Space(float x, float y, float Width, float Height, Color color)
{
    RectangleShape rect(Vector2f(Width, Height));
    rect.setPosition({x, y});
    rect.setFillColor(color);
    return rect;
}