#pragma once 

enum Piece
{
    pawn,
    rook,
    knight,
    bishop,
    king,
    queen
};

enum PieceColor
{
    white,
    black
};

struct Pieces
{
    Piece PieceType;
    PieceColor Color;
    int X;
    int Y;
};