#pragma once

#include <array>
#include <optional>
#include <iostream>
#include <functional>
#include <vector>
#include <utility>
#include <climits>

class Board
{
    public:
        static const std::size_t board_rows = 6;
        static const std::size_t board_columns = 7;
        static const int connections_to_win = 4;

        using board_t = std::array<std::array<char, board_columns>, board_rows>;
        using column_score_t = std::vector<std::pair<std::size_t, int>>;

        Board();

        void play(int, bool, bool);
        void print(std::ostream&) const;

    private:
        std::size_t insert(char, std::size_t);
        std::optional<std::size_t> next_available_row(std::size_t) const;
        bool winner_winner_chicken_dinner(const board_t&) const;
        bool connected_four_diagonally(std::size_t, std::size_t, const board_t&) const;
        bool connected_four_vertically(std::size_t, std::size_t, const board_t&) const;
        bool connected_four_horizontally(std::size_t, std::size_t, const board_t&) const;
        bool is_full(const board_t&) const;

        // minimax functions
        std::size_t minimax(int, const board_t&, bool);
        std::pair<std::size_t, int> minimax(board_t, bool, int, bool, int = INT_MAX, int = INT_MIN, std::optional<std::size_t> = std::nullopt, std::optional<std::size_t> = std::nullopt);
        std::pair<board_t, std::size_t> insert(board_t, std::size_t, char);
        int calculate_score(char, const board_t&);
        int calculate_horizontal_score(char, const board_t&);
        int calculate_vertical_score(char, const board_t&);
        int calculate_diagonal_score(char, const board_t&);
        int get_score_based_on_window(int, int, int);
        std::vector<std::size_t> get_next_available_columns(const board_t&);

        board_t board;
};
