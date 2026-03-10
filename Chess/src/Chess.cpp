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
Pieces BlackPieces[16];

Pieces Selected = {pawn, white, -1, -1};
list<Tile> PossibleMoves;

const float TileWidth = 122.5f;
const float TileHeight = 122.5f;

int WhiteTurnCount = 0;
int BlackTurnCount = 0;
bool WhiteTurn = true;

// --Prototpes--
void CreateWindow();
void InitWhitePieces();
void InitBlackPieces();

void Inputs(const Event &event, RenderWindow &window);
void HandleMovement(int BoardX, int BoardY);

void DrawPieces();
void Draw();
void DrawBoard();
void DrawPossibleMoves();

Pieces GetTileInformation(int BoardX, int BoardY);
void GetPossibleMoves(Pieces SelectedPiece);
bool CheckIfTileIsOccupied(int BoardX, int BoardY);
bool ValidTake(int BoardX, int BoardY);
void HandleTake(PieceColor color, int BoardX, int BoardY);
bool IsOnBoard(int BoardX, int BoardY);

int main()
{
    CreateWindow();
    InitWhitePieces();
    InitBlackPieces();

    while (_ChessWindow.isOpen())
    {
        while (auto event = _ChessWindow.pollEvent())
        {
            Inputs(*event, _ChessWindow);
        }

        Draw();
        PossibleMoves.clear();

        if (Selected.X != -1)
        {
            GetPossibleMoves(Selected);
        }
    }

    return 0;
}


// --Input Handling--

void Inputs(const Event &event, RenderWindow &window)
{
    if (event.is<Event::Closed>())
        window.close();

    if (const auto *keyPressed = event.getIf<Event::KeyPressed>())
    {
        if (keyPressed->code == Keyboard::Key::Escape)
            window.close();
    }

    if (const auto *mousePressed = event.getIf<Event::MouseButtonPressed>())
    {
        if (mousePressed->button == Mouse::Button::Left)
        {

            int boardX = static_cast<int>((mousePressed->position.x - 50.f) / TileWidth);
            int boardY = static_cast<int>((mousePressed->position.y - 50.f) / TileHeight);

            if (Selected.X == -1)
            {

                if (boardX >= 0 && boardX < 8 && boardY >= 0 && boardY < 8)
                {
                    Selected = GetTileInformation(boardX, boardY);
                    if (Selected.X != -1)
                    {
                        cout << "Selected peice is at " << PlayingBoard.x[Selected.X] << ", " << PlayingBoard.y[Selected.Y] << endl;
                    }
                }

            }
            else
            {
                HandleMovement(boardX, boardY);
            }
        }
    }
}

void HandleMovement(int BoardX, int BoardY)
{
    bool validMove = false;

    if ((WhiteTurn && Selected.Color != white) || (!WhiteTurn && Selected.Color != black))
    {
        cout << "Not your turn!" << endl;
        Selected = {pawn, white, -1, -1};
        return;
    }

    for (const auto& move : PossibleMoves)
    {
        if (move.X == BoardX && move.Y == BoardY)
        {
            validMove = true;
            break;
        }
    }

    if (CheckIfTileIsOccupied(BoardX, BoardY) && !ValidTake(BoardX, BoardY))
    {
        validMove = false;
        return;
    }

    if (!validMove)
    {
        Selected = GetTileInformation(BoardX, BoardY);
        return;
    }

    Pieces* pieceSet = (Selected.Color == white) ? WhitePieces : BlackPieces;

    for (int i = 0; i < 16; i++)
    {
        if (pieceSet[i].X == Selected.X && pieceSet[i].Y == Selected.Y && pieceSet[i].PieceType == Selected.PieceType)
        {
            pieceSet[i].X = BoardX;
            pieceSet[i].Y = BoardY;
            HandleTake(Selected.Color, BoardX, BoardY);
            break;
        }
    }

    if (Selected.Color == white)
        WhiteTurnCount++;
    else
        BlackTurnCount++;

    WhiteTurn = !WhiteTurn;

    Selected = {pawn, white, -1, -1};
    PossibleMoves.clear();
}

// --Inits--

void CreateWindow()
{
    _ChessWindow.create(VideoMode({1080, 1080}), "Chess Window");
    _ChessWindow.setFramerateLimit(60);
}

void InitWhitePieces()
{
    WhitePieces[0] = {rook, white, 0, 7};
    WhitePieces[1] = {knight, white, 1, 7};
    WhitePieces[2] = {bishop, white, 2, 7};
    WhitePieces[3] = {queen, white, 3, 7};
    WhitePieces[4] = {king, white, 4, 7};
    WhitePieces[5] = {bishop, white, 5, 7};
    WhitePieces[6] = {knight, white, 6, 7};
    WhitePieces[7] = {rook, white, 7, 7};

    for (int i = 8; i < 16; i++)
        WhitePieces[i] = {pawn, white, i - 8, 6};
}

void InitBlackPieces()
{
    BlackPieces[0] = {rook, black, 0, 0};
    BlackPieces[1] = {knight, black, 1, 0};
    BlackPieces[2] = {bishop, black, 2, 0};
    BlackPieces[3] = {queen, black, 3, 0};
    BlackPieces[4] = {king, black, 4, 0};
    BlackPieces[5] = {bishop, black, 5, 0};
    BlackPieces[6] = {knight, black, 6, 0};
    BlackPieces[7] = {rook, black, 7, 0};

    for (int i = 8; i < 16; i++)
        BlackPieces[i] = {pawn, black, i - 8, 1};
}

//  --Draw voids--

void Draw()
{
    _ChessWindow.clear(Color(38, 22, 4));

    DrawBoard();
    DrawPieces();
    DrawPossibleMoves();

    _ChessWindow.display();
}

void DrawPieces()
{
    const float boardLeft = 50.f;
    const float boardTop = 50.f;

    for (int i = 0; i < 16; i++)
    {
        float Piece_x = boardLeft + WhitePieces[i].X * TileWidth + TileWidth * 0.5f;
        float Piece_y = boardTop + WhitePieces[i].Y * TileHeight + TileHeight * 0.5f;

        CircleShape piece(TileWidth * 0.35f);
        piece.setOrigin({TileWidth * 0.35f, TileWidth * 0.35f});
        piece.setPosition({Piece_x, Piece_y});
        piece.setFillColor(Color::White);

        _ChessWindow.draw(piece);
    }

    for (int i = 0; i < 16; i++)
    {
        float Piece_x = boardLeft + BlackPieces[i].X * TileWidth + TileWidth * 0.5f;
        float Piece_y = boardTop + BlackPieces[i].Y * TileHeight + TileHeight * 0.5f;

        CircleShape piece(TileWidth * 0.35f);
        piece.setOrigin({TileWidth * 0.35f, TileWidth * 0.35f});
        piece.setPosition({Piece_x, Piece_y});
        piece.setFillColor(Color::Black);

        _ChessWindow.draw(piece);
    }
}

void DrawBoard()
{
    float X_Offset = 0;
    float Y_Offset = 0;

    bool Colour = false;

    for (auto &x : PlayingBoard.x)
    {
        for (auto &y : PlayingBoard.y)
        {
            if (Colour)
            {
                _ChessWindow.draw(Tiles(50 + X_Offset, 50 + Y_Offset, TileWidth, TileHeight, Color(191, 111, 21)));
                Colour = !Colour;
            }
            else
            {
                _ChessWindow.draw(Tiles(50 + X_Offset, 50 + Y_Offset, TileWidth, TileHeight, Color(76, 44, 8)));
                Colour = !Colour;
            }

            X_Offset += TileWidth;
        }

        Y_Offset += TileHeight;
        X_Offset = 0;
        Colour = !Colour;
    }

    if (Selected.X != -1)
    {
        _ChessWindow.draw(Tiles(50.f + Selected.X * TileWidth, 50.f + Selected.Y * TileHeight, TileWidth, TileHeight, Color(255, 255, 0, 120)));
    }
}

void DrawPossibleMoves()
{
    for (auto Possbile : PossibleMoves)
    {
        if (CheckIfTileIsOccupied(Possbile.X, Possbile.Y) && !ValidTake(Possbile.X, Possbile.Y))
            _ChessWindow.draw(Tiles(50.f + Possbile.X * TileWidth, 50.f + Possbile.Y * TileHeight, TileWidth, TileHeight, Color(255, 0, 0, 120)));
        else
            _ChessWindow.draw(Tiles(50.f + Possbile.X * TileWidth, 50.f + Possbile.Y * TileHeight, TileWidth, TileHeight, Color(255, 255, 0, 120)));
    }
}

// --Tools and Help--

Pieces GetTileInformation(int BoardX, int BoardY)
{
    for (int i = 0; i < 16; i++)
    {
        if (WhitePieces[i].X == BoardX && WhitePieces[i].Y == BoardY)
            return WhitePieces[i];

        if (BlackPieces[i].X == BoardX && BlackPieces[i].Y == BoardY)
            return BlackPieces[i];
    }

    return {pawn, white, -1, -1};
}

void GetPossibleMoves(Pieces SelectedPiece)
{
    PossibleMoves.clear();

    switch (SelectedPiece.PieceType)
    {
    case pawn:
    {
        int direction = (SelectedPiece.Color == white) ? -1 : 1;
        int startRow = (SelectedPiece.Color == white) ? 6 : 1;
        int forwardY = SelectedPiece.Y + direction;

        if (IsOnBoard(SelectedPiece.X, forwardY) && !CheckIfTileIsOccupied(SelectedPiece.X, forwardY))
        {
            PossibleMoves.push_back({SelectedPiece.X, forwardY});

            int doubleForwardY = SelectedPiece.Y + 2 * direction;
            if (SelectedPiece.Y == startRow &&
                IsOnBoard(SelectedPiece.X, doubleForwardY) &&
                !CheckIfTileIsOccupied(SelectedPiece.X, doubleForwardY))
            {
                PossibleMoves.push_back({SelectedPiece.X, doubleForwardY});
            }
        }

        int leftX = SelectedPiece.X - 1;
        int rightX = SelectedPiece.X + 1;

        if (IsOnBoard(leftX, forwardY) && ValidTake(leftX, forwardY))
            PossibleMoves.push_back({leftX, forwardY});

        if (IsOnBoard(rightX, forwardY) && ValidTake(rightX, forwardY))
            PossibleMoves.push_back({rightX, forwardY});

        break;
    }
    case rook:
    {
        int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        for (auto &direction : directions)
        {
            int x = SelectedPiece.X + direction[0];
            int y = SelectedPiece.Y + direction[1];

            while (IsOnBoard(x, y))
            {
                Pieces targetPiece = GetTileInformation(x, y);

                if (targetPiece.X == -1)
                {
                    PossibleMoves.push_back({x, y});
                }
                else
                {
                    if (targetPiece.Color != SelectedPiece.Color)
                        PossibleMoves.push_back({x, y});

                    break;
                }

                x += direction[0];
                y += direction[1];
            }
        }
        break;
    }
    case knight:
    {
        int moves[8][2] = {{-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}};

        for (auto &move : moves)
        {
            int x = SelectedPiece.X + move[0];
            int y = SelectedPiece.Y + move[1];

            if (!IsOnBoard(x, y))
                continue;

            Pieces targetPiece = GetTileInformation(x, y);
            if (targetPiece.X == -1 || targetPiece.Color != SelectedPiece.Color)
                PossibleMoves.push_back({x, y});
        }
        break;
    }
    case bishop:
    {
        int directions[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};

        for (auto &direction : directions)
        {
            int x = SelectedPiece.X + direction[0];
            int y = SelectedPiece.Y + direction[1];

            while (IsOnBoard(x, y))
            {
                Pieces targetPiece = GetTileInformation(x, y);

                if (targetPiece.X == -1)
                {
                    PossibleMoves.push_back({x, y});
                }
                else
                {
                    if (targetPiece.Color != SelectedPiece.Color)
                        PossibleMoves.push_back({x, y});

                    break;
                }

                x += direction[0];
                y += direction[1];
            }
        }
        break;
    }
    }

    cout << "Possible moves are:" << endl;
    for (auto Possible : PossibleMoves)
    {
        if (!IsOnBoard(Possible.X, Possible.Y))
            continue;

        cout << PlayingBoard.x[Possible.X] << ", " << PlayingBoard.y[Possible.Y] << endl;
    }
}

bool CheckIfTileIsOccupied(int BoardX, int BoardY)
{
    Pieces targetTilePiece = GetTileInformation(BoardX, BoardY);
    if (targetTilePiece.X != -1)
    {
        cout << "Tile is occupied." << endl;
        return true;
    }
    return false;
}

bool ValidTake(int BoardX, int BoardY)
{
    Pieces targetPiece = GetTileInformation(BoardX, BoardY);

    if (targetPiece.X == -1)
        return false;

    if (targetPiece.Color == Selected.Color)
        return false;

    if (Selected.PieceType == pawn)
    {
        int direction = (Selected.Color == white) ? -1 : 1;

        if (BoardY == Selected.Y + direction &&
            (BoardX == Selected.X - 1 || BoardX == Selected.X + 1))
        {
            return true;
        }

        return false;
    }

    return true;
}

bool IsOnBoard(int BoardX, int BoardY)
{
    return BoardX >= 0 && BoardX < 8 && BoardY >= 0 && BoardY < 8;
}

void HandleTake(PieceColor color, int BoardX, int BoardY)
{
    Pieces* opponentPieces = (color == white) ? BlackPieces : WhitePieces;

    for (int i = 0; i < 16; i++)
    {
        if (opponentPieces[i].X == BoardX && opponentPieces[i].Y == BoardY)
        {
            opponentPieces[i].X = -1;
            opponentPieces[i].Y = -1;
            cout << "Piece taken at " << PlayingBoard.x[BoardX] << ", " << PlayingBoard.y[BoardY] << endl;
            break;
        }
    }
}