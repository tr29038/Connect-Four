#pragma once

#include <array>
#include <optional>
#include <iostream>

class Board
{
    public:
        static const std::size_t board_rows = 6;
        static const std::size_t board_columns = 7;

        Board();

        bool insert(char, std::size_t);
        void print(std::ostream&) const;

    private:
        std::optional<std::size_t> next_available_row(std::size_t);

        std::array<std::array<char, board_columns>, board_rows> board;
};
