#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>

#include "../include/board.h"

// --Prototpes--


int main()
{
    board PlayingBoard;
    std::cout << "Hello world" << std::endl;

    for(auto& x : PlayingBoard.x)
    {   
        for(auto& y : PlayingBoard.y)
        {
            std::cout << "S";
        }
        std::cout << "\n";
    }

    return 0;
}

