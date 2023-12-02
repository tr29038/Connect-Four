#include "board.h"

#include <iomanip>
#include <string>
#include <map>
#include <cmath>
#include <random>

Board::Board()
{
    for (auto& row : board)
        row.fill(' ');
}

//! @brief Main function to play Connect-4 against AI.
//! @param difficulty_depth The depth in which the AI will traverse when running minimax.
//! @param player_first True if the player moves first, false otherwise.
//! @param alpha_beta_pruning True to use alpha-beta pruning, false otherwise.
void Board::play(int difficulty_depth, bool player_first, bool alpha_beta_pruning)
{
    if (player_first)
        print(std::cout);

    while (true)
    {
        if (is_full(board))
        {
            std::cout << "It's a tie!\n";
            return;
        }

        if (player_first)
        {
            // Player's turn
            while (true)
            {
                int column;
                // Disallow entering non-numeric characters and columns that are out of range or full.
                try
                {
                    std::cout << "Column #: ";
                    std::string column_str;
                    std::getline(std::cin, column_str);
                    column = std::stoi(column_str);
                    if (column < 0 || column >= board_columns)
                        throw std::runtime_error("nay, i say nay");
                }
                catch (const std::exception& e)
                {
                    std::cout << "Enter a valid column (0-6).\n";
                }

                try
                {
                    insert('Y', column);
                    break;
                }
                catch (const std::exception& e)
                {
                    std::cout << "Enter a non-full column.\n";
                }

                print(std::cout);

                if (winner_winner_chicken_dinner(board))
                {
                    std::cout << "Player won!\n";
                    return;
                }
            }
        }

        if (!player_first)
            player_first = true;

        // AI's turn
        auto ai_column = minimax(difficulty_depth, board, alpha_beta_pruning);
        insert('R', ai_column);

        print(std::cout);

        if (winner_winner_chicken_dinner(board))
        {
            std::cout << "AI won!\n";
            return;
        }
    }
}

//! @brief Print the game board.
//! @param output_stream The output stream to print to.
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

//! @brief Attempt to insert a player piece in column.
//! @param player The player piece to insert (max or min).
//! @param column The column to attempt to insert in.
//! @return The row the piece was inserted in, if successful.
//! @throw std::runtime_error If the column provided is invalid.
//! @throw std::runtime_error If the column provided has no more room.
std::size_t Board::insert(char player, std::size_t column)
{
    if (column >= board_columns)
        throw std::runtime_error("Cannot insert disc outside of valid columns : " + std::to_string(column));

    auto row = next_available_row(column);
    if (!row.has_value())
        throw std::runtime_error("Cannot insert disc, the column (" + std::to_string(column) + ") is full");

    board[row.value()][column] = player;

    return row.value();
}

//! @brief Get the next available row in the column, if one exists.
//! @param column The column to check.
//! @return The next available row in column if one exists, else std::nullopt.
std::optional<std::size_t> Board::next_available_row(std::size_t column) const
{
    for (std::size_t row = board.size() - 1; row >= 0 && row < board.size(); --row)
    {
        if (board[row][column] == ' ')
            return row;
    }

    return std::nullopt;
}

//! @brief Check if a winner exists from the given slot.
//! @return True if a winner exists from the current slot, false otherwise.
bool Board::winner_winner_chicken_dinner(const board_t& board) const
{
    for (std::size_t row = 0; row < board.size(); ++row)
    {
        for (std::size_t column = 0; column < board[row].size(); ++column)
        {
            if (board[row][column] == ' ')
                continue;

            if (connected_four_horizontally(column, row, board) || connected_four_vertically(column, row, board) || connected_four_diagonally(column, row, board))
                return true;
        }
    }

    return false;
}

//! @brief Check if four diagonal pieces, of the same type, are adjacent to each other.
//! @param column The column to begin checking from.
//! @param row The row to begin checking from.
//! @param board The board to check.
//! @return True if four pieces are connected diagonally, false otherwise.
bool Board::connected_four_diagonally(std::size_t column, std::size_t row, const board_t& board) const
{
    auto diagonal_computations = [&](std::size_t column, std::size_t row, int& connected, char player)
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

//! @brief Check if four vertical pieces, of the same type, are adjacent to each other.
//! @param column The column to begin checking from.
//! @param row The row to begin checking from.
//! @param board The board to check.
//! @return True if four pieces are connected vertically, false otherwise.
bool Board::connected_four_vertically(std::size_t column, std::size_t row, const board_t& board) const
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

//! @brief Check if four horizontal pieces, of the same type, are adjacent to each other.
//! @param column The column to begin checking from.
//! @param row The row to begin checking from.
//! @param board The board to check.
//! @return True if four pieces are connected horizontally, false otherwise.
bool Board::connected_four_horizontally(std::size_t column, std::size_t row, const board_t& board) const
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

//! @brief Check if the board is full.
//! @param board The board to check.
//! @return True if the board is full, false otherwise.
bool Board::is_full(const board_t& board) const
{
    for (std::size_t row = 0; row < board_rows; ++row)
    {
        for (std::size_t column = 0; column < board[row].size(); ++column)
        {
            if (board[row][column] == ' ')
                return false;
        }
    }

    return true;
}

//! @brief Begin the recusive minimax algorithm.
//! @param depth The maximum depth to go.
//! @param board The unmodified game board.
//! @param alpha_beta_pruning True to use alpha-beta pruning, false otherwise.
//! @return The ideal column for the next move.
std::size_t Board::minimax(int depth, const board_t& board, bool alpha_beta_pruning)
{
    return minimax(board, true, depth, alpha_beta_pruning).first;
}

//! @brief Run the minimax algorithm with alpha-beta pruning.
//! @param state The current state/board.
//! @param is_max True for max, false for min.
//! @param depth The current depth of the tree.
//! @param alpha_beta_pruning True to use alpha-beta pruning, false otherwise.
//! @param beta The current beta value.
//! @param alpha The current alpha value.
//! @param last_row The last inserted at row.
//! @param last_column The last inserted at column.
//! @return A pair containing: [first] -> The column of the current ideal state to insert in and [second] -> The score of the current ideal state.
std::pair<std::size_t, int> Board::minimax(board_t state, bool is_max, int depth, bool alpha_beta_pruning, int beta, int alpha, std::optional<std::size_t> last_row, std::optional<std::size_t> last_column)
{
    // If there's a winner, exit now.
    if (last_column.has_value() && last_row.has_value() && winner_winner_chicken_dinner(state))
    {
        int score;
        if (state[last_row.value()][last_column.value()] == 'R')
            score = INT_MAX;
        else
            score = INT_MIN;

        return {last_column.value(), score};
    }

    // If no more pieces can be played, exit now.
    if (is_full(state))
        return {-1, 0};

    // If the maximum specified depth has been reached, exit now.
    if (depth <= 0)
        return {-1, calculate_score('R', state)};

    auto columns = get_next_available_columns(state);
    std::random_device dev;
    std::mt19937 rng(dev());
    if (is_max)
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, columns.size() - 1);
        std::size_t chosen_column = columns[dist(rng)];
        int chosen_score = INT_MIN;
        while (!columns.empty())
        {
            std::uniform_int_distribution<std::mt19937::result_type> dist(0, columns.size() - 1);
            auto column_it = columns.begin() + dist(rng);
            auto column = *column_it;
            columns.erase(column_it);

            auto [successor_state, row] = insert(state, column, 'R');
            auto score = minimax(successor_state, false, depth - 1, alpha_beta_pruning, beta, alpha, row, column).second;
            if (score > chosen_score)
            {
                chosen_score = score;
                chosen_column = column;
            }

            if (!alpha_beta_pruning)
                continue;

            alpha = std::max(alpha, chosen_score);
            if (beta <= alpha)
                break;
        }

        return {chosen_column, chosen_score};
    }
    else
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, columns.size() - 1);
        std::size_t chosen_column = columns[dist(rng)];
        int chosen_score = INT_MAX;
        while (!columns.empty())
        {
            std::uniform_int_distribution<std::mt19937::result_type> dist(0, columns.size() - 1);
            auto column_it = columns.begin() + dist(rng);
            auto column = *column_it;
            columns.erase(column_it);

            auto [successor_state, row] = insert(state, column, 'Y');
            auto score = minimax(successor_state, true, depth - 1, alpha_beta_pruning, beta, alpha, row, column).second;
            if (score < chosen_score)
            {
                chosen_score = score;
                chosen_column = column;
            }

            if (!alpha_beta_pruning)
                continue;

            beta = std::min(beta, chosen_score);
            if (beta <= alpha)
                break;
        }

        return {chosen_column, chosen_score};
    }
}

//! @brief Insert player piece into column in state.
//! @param state The state/board to copy, insert, and return.
//! @param column The column to insert in.
//! @param player The player piece (max or min).
//! @return A pair containing: [first] -> the altered state/board and [second] -> the row the piece landed on.
std::pair<Board::board_t, std::size_t> Board::insert(board_t state, std::size_t column, char player)
{
    std::size_t row = state.size() - 1;
    for (; row >= 0 && row < state.size(); --row)
    {
        if (state[row][column] == ' ')
        {
            state[row][column] = player;
            break;
        }
    }

    return {state, row};
};

//! @brief Calculate the heuristic for the given state/board.
//! @param player The player piece (max or min).
//! @param state The state/board.
//! @return the calculated heuristic score.
int Board::calculate_score(char player, const board_t& state)
{
    auto horizontal_score = calculate_horizontal_score(player, state);
    auto vertical_score = calculate_vertical_score(player, state);
    auto diagonal_score = calculate_diagonal_score(player, state);

    return horizontal_score + vertical_score + diagonal_score;
}

//! @brief Calculate the sum the heuristics for every possible horizontal window (4 consecutive slots).
//! @param player Player piece (max or min).
//! @param state The board to check.
//! @return The summation of all window heuristics.
int Board::calculate_horizontal_score(char player, const board_t& state)
{
    int score = 0;
    for (std::size_t row = 0; row < state.size(); ++row)
    {
        std::size_t right = 3;
        while (right < state[row].size())
        {
            int player_pieces = 0;
            int opponent_pieces = 0;
            int blank_pieces = 0;
            for (std::size_t left = right - 3; left <= right; ++left)
            {
                if (state[row][left] == player)
                    ++player_pieces;
                else if (state[row][left] == ' ')
                    ++blank_pieces;
                else
                    ++opponent_pieces;
            }
            ++right;

            score += get_score_based_on_window(player_pieces, opponent_pieces, blank_pieces);
        }
    }

    return score;
}

//! @brief Calculate the sum the heuristics for every possible vertical window (4 consecutive slots).
//! @param player Player piece (max or min).
//! @param state The board to check.
//! @return The summation of all window heuristics.
int Board::calculate_vertical_score(char player, const board_t& state)
{
    int score = 0;
    for (std::size_t column = 0; column < board_columns; ++column)
    {
        std::size_t bottom = state.size() - 1 - 3;
        while (bottom < state.size())
        {
            int player_pieces = 0;
            int opponent_pieces = 0;
            int blank_pieces = 0;
            for (std::size_t top = bottom; top <= bottom + 3; ++top)
            {
                if (state[top][column] == player)
                    ++player_pieces;
                else if (state[top][column] == ' ')
                    ++blank_pieces;
                else
                    ++opponent_pieces;
            }
            --bottom;

            score += get_score_based_on_window(player_pieces, opponent_pieces, blank_pieces);
        }
    }

    return score;
}

//! @brief Calculate the sum the heuristics for every possible diagonal window (4 consecutive slots).
//! @param player Player piece (max or min).
//! @param state The board to check.
//! @return The summation of all window heuristics.
int Board::calculate_diagonal_score(char player, const board_t& state)
{
    int score = 0;

    // left -> right
    for (std::size_t row = 0; row < state.size() - 1 - 3; ++row)
    {
        for (std::size_t column = 0; column < state[row].size() - 1 - 3; ++column)
        {
            std::size_t bottom_left_row = row;
            std::size_t bottom_left_column = column;
            std::size_t top_right_row = bottom_left_row + 3;
            std::size_t top_right_column = bottom_left_column + 3;
            int player_pieces = 0;
            int opponent_pieces = 0;
            int blank_pieces = 0;
            while (top_right_row < state.size() && top_right_row >= bottom_left_row && top_right_column < state[top_right_row].size() && top_right_column >= bottom_left_column)
            {
                if (state[top_right_row][top_right_column] == player)
                    ++player_pieces;
                else if (state[top_right_row][top_right_column] == ' ')
                    ++blank_pieces;
                else
                    ++opponent_pieces;

                --top_right_row;
                --top_right_column;
            }
            score += get_score_based_on_window(player_pieces, opponent_pieces, blank_pieces);
        }
    }

    // right -> left
    for (std::size_t row = 0; row < state.size() - 1 - 3; ++row)
    {
        for (std::size_t column = state[row].size() - 1; column >= 3; --column)
        {
            std::size_t bottom_right_row = row;
            std::size_t bottom_right_column = column;
            std::size_t top_left_row = bottom_right_row + 3;
            std::size_t top_left_column = bottom_right_column - 3;
            int player_pieces = 0;
            int opponent_pieces = 0;
            int blank_pieces = 0;
            while (top_left_row < state.size() && top_left_row >= bottom_right_row && top_left_column < state[top_left_row].size() && top_left_column <= bottom_right_column)
            {
                if (state[top_left_row][top_left_column] == player)
                    ++player_pieces;
                else if (state[top_left_row][top_left_column] == ' ')
                    ++blank_pieces;
                else
                    ++opponent_pieces;

                --top_left_row;
                ++top_left_column;
            }
            score += get_score_based_on_window(player_pieces, opponent_pieces, blank_pieces);
        }
    }

    return score;
}

//! @brief Calculate the heuristic given the number of pieces in the window (4 slots).
//!        I.e., player_pieces + opponent_pieces + blank_pieces == 4.
//! @param player_pieces The number of player pieces in the window.
//! @param opponent_pieces The number of opponent pieces in the window.
//! @param blank_pieces The number of blank pieces in the window.
//! @return The calculated heuristic score for the given window.
int Board::get_score_based_on_window(int player_pieces, int opponent_pieces, int blank_pieces)
{
    int score = 0;

    if (player_pieces == 4)
        return 1000;

    if (opponent_pieces == 4)
        return -1000;

    if (player_pieces == 3 && blank_pieces == 1)
        score += 10;
    else if (player_pieces == 2 && blank_pieces == 2)
        score += 3;

    if (opponent_pieces == 3 && blank_pieces == 1)
        score -= 10;
    else if (opponent_pieces == 2 && blank_pieces == 2)
        score -= 3;

    return score;
}

//! @brief Get all non-full columns in board.
//! @param board The board to check.
//! @return All non-full column indexes.
std::vector<std::size_t> Board::get_next_available_columns(const board_t& board)
{
    std::vector<std::size_t> valid_columns;
    for (std::size_t column = 0; column < 7; ++column)
    {
        if (board[0][column] == ' ')
            valid_columns.push_back(column);
    }

    return valid_columns;
}
