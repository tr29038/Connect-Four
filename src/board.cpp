#include "board.h"

#include <iomanip>
#include <string>

Board::Board()
{
    for (std::size_t row = 0; row < board_rows; ++row)
        board[row].fill(' ');
}

bool Board::insert(char player, std::size_t column)
{
    if (column >= board_columns)
        throw std::runtime_error("Cannot insert disc outside of valid rows : " + std::to_string(column));

    auto row = next_available_row(column);
    if (!row.has_value())
        return false;

    board[row.value()][column] = player;
    return true;
}

void Board::print(std::ostream& output_stream) const
{
    for (std::size_t row = 0; row < board_rows; ++row)
    {
        output_stream << std::right << std::setfill('-') << std::setw(board_columns * 2 + 1) << '-' << '\n';
        output_stream << '|';
        for (std::size_t column = 0; column < board_columns; ++column)
        {
            output_stream << board[row][column] << '|';
        }
        output_stream << '\n';
    }
    output_stream << std::right << std::setfill('-') << std::setw(board_columns * 2 + 1) << '-' << '\n';
}

std::optional<std::size_t> Board::next_available_row(std::size_t column)
{
    for (std::size_t row = board.size() - 1; row >= 0 && row < board.size(); --row)
    {
        if (board[row][column] == ' ')
            return row;
    }

    return std::nullopt;
}
