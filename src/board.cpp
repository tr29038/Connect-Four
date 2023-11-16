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
        throw std::runtime_error("Cannot insert disc, the column (" + std::to_string(column) + ") is full");

    board[row.value()][column] = player;

    return winner_winner_chicken_dinner(column, row.value());
}

void Board::print(std::ostream& output_stream) const
{
    for (std::size_t row = 0; row < board_rows; ++row)
    {
        output_stream << std::setfill('-') << std::setw(board_columns * 2 + 1) << '-' << '\n';
        output_stream << '|';
        for (std::size_t column = 0; column < board_columns; ++column)
        {
            output_stream << board[row][column] << '|';
        }
        output_stream << '\n';
    }
    output_stream << std::setfill('-') << std::setw(board_columns * 2 + 1) << '-' << '\n';
}

std::optional<std::size_t> Board::next_available_row(std::size_t column) const
{
    for (std::size_t row = board.size() - 1; row >= 0 && row < board.size(); --row)
    {
        if (board[row][column] == ' ')
            return row;
    }

    return std::nullopt;
}

bool Board::winner_winner_chicken_dinner(std::size_t column, std::size_t row) const
{
    return connected_four_horizontally(column, row) || connected_four_vertically(column, row) || connected_four_diagonally(column, row);
}

bool Board::connected_four_diagonally(std::size_t column, std::size_t row) const
{
    auto diagonal_computations = [this](std::size_t column, std::size_t row, int& connected, char player)
    {
        if (row < board.size() && column < board[row].size())
        {
            if (board[row][column] == player)
            {
                if (++connected == connections_to_win)
                    return true;
            }
            else
            {
                connected = 0;
            }
        }

        return false;
    };

    int connected_top_left_start = 0;
    int connected_bottom_left_start = 0;
    char player = board[row][column];
    for (int xy_offset = -3; xy_offset <= 3; ++xy_offset)
    {
        if (diagonal_computations(column + xy_offset, row + xy_offset, connected_top_left_start, player))
            return true;

        if (diagonal_computations(column - xy_offset, row + xy_offset, connected_bottom_left_start, player))
            return true;
    }

    return false;
}

bool Board::connected_four_vertically(std::size_t column, std::size_t row) const
{
    int connected = 0;
    char player = board[row][column];
    std::size_t row_top = row + 3;
    row = row - 3 >= board.size() ? 0 : row - 3;
    for (; row <= row_top; ++row)
    {
        if (row >= board.size())
            break;

        if (board[row][column] == player)
        {
            if (++connected == connections_to_win)
                return true;
        }
        else
        {
            connected = 0;
        }
    }

    return false;
}

bool Board::connected_four_horizontally(std::size_t column, std::size_t row) const
{
    int connected = 0;
    char player = board[row][column];
    std::size_t column_right = column + 3;
    column = column - 3 >= board[row].size() ? 0 : column - 3;
    for (; column <= column_right; ++column)
    {
        if (column >= board[row].size())
            break;

        if (board[row][column] == player)
        {
            if (++connected == connections_to_win)
                return true;
        }
        else
        {
            connected = 0;
        }
    }

    return false;
}
