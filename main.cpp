#include "board.h"

#include <iostream>

int main()
{
    // Temporary
    Board board;
    std::size_t input;
    do
    {
        board.print(std::cout);
        std::cout << "Column: ";
        std::cin >> input;
    } while (!board.insert('R', input));
    board.print(std::cout);

    return 0;
}
