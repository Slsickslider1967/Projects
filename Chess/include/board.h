#pragma once 

#include <SFML/Graphics.hpp>

using namespace sf;

struct board 
{
    char x[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    int y[8] = {1, 2, 3, 4, 5, 6, 7, 8};
};

struct Tile
{
    int X;
    int Y;
};

RectangleShape Tiles(float x, float y, float Width, float Height, Color color)
{
    RectangleShape rect(Vector2f(Width, Height));
    rect.setPosition({x, y});
    rect.setFillColor(color);
    return rect;
}