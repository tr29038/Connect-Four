#include "board.h"

#include <iostream>

int main()
{
    // Temporary
    Board board;
    board.print(std::cout);
    board.insert('R', 3);
    board.insert('Y', 3);
    board.print(std::cout);

    return 0;
}
