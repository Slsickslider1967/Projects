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

struct Pieces
{
    Piece PieceType; 
    int X;
    int Y;
};