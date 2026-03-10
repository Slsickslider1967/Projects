#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>

#include "../include/board.h"
#include "../include/pieces.h"

using namespace std;
using namespace sf;

// --Variables--
RenderWindow _ChessWindow;
board PlayingBoard;

Pieces WhitePieces[16];

const float TileWidth = 122.5f;
const float TileHeight = 122.5f;

// --Prototpes--
void CreateWindow();
void Draw();
void InitWhitePieces();
void DrawPieces();

int main()
{
    CreateWindow();
    InitWhitePieces();

    while (_ChessWindow.isOpen())
    {
        while (auto event = _ChessWindow.pollEvent())
        {
            if (event->is<Event::Closed>())
                _ChessWindow.close();
        }
        Draw();
    }

    return 0;
}

void Draw()
{
    float X_Offset = 0;
    float Y_Offset = 0;

    bool Colour = false;

    _ChessWindow.clear(Color(38, 22, 4));

    for (auto &x : PlayingBoard.x)
    {
        for (auto &y : PlayingBoard.y)
        {
            if (Colour)
            {
                _ChessWindow.draw(Space(50 + X_Offset, 50 + Y_Offset, TileWidth, TileHeight, Color(191, 111, 21)));
                Colour = !Colour;
            }
            else 
            {
                _ChessWindow.draw(Space(50 + X_Offset, 50 + Y_Offset, TileWidth, TileHeight, Color(76, 44, 8)));
                Colour = !Colour;
            }

            X_Offset += TileWidth;
        }

        Y_Offset += TileHeight;
        X_Offset = 0;
        Colour = !Colour;

    }

    DrawPieces();
    _ChessWindow.display();
}

void CreateWindow()
{
    _ChessWindow.create(VideoMode({1080, 1080}), "Chess Window");
    _ChessWindow.setFramerateLimit(60);
}

void InitWhitePieces()
{
    WhitePieces[0] = {rook,   0, 7};
    WhitePieces[1] = {knight, 1, 7};
    WhitePieces[2] = {bishop, 2, 7};
    WhitePieces[3] = {queen,  3, 7};
    WhitePieces[4] = {king,   4, 7};
    WhitePieces[5] = {bishop, 5, 7};
    WhitePieces[6] = {knight, 6, 7};
    WhitePieces[7] = {rook,   7, 7};

    for (int i = 8; i < 16; i++)
        WhitePieces[i] = {pawn, i - 8, 6}; 
}

void DrawPieces()
{
    const float boardLeft = 50.f;
    const float boardTop = 50.f;

    for (int i = 0; i < 16; i++)
    {
        float px = boardLeft + WhitePieces[i].X * TileWidth + TileWidth * 0.5f;
        float py = boardTop  + WhitePieces[i].Y * TileHeight + TileHeight * 0.5f;

        CircleShape piece(TileWidth * 0.35f);
        piece.setOrigin({TileWidth * 0.35f, TileWidth * 0.35f});
        piece.setPosition({px, py});
        piece.setFillColor(Color::White);

        _ChessWindow.draw(piece);
    }
}