#include "board.h"

#include <iostream>

int main(int argc, char* argv[])
{
    std::unordered_map<std::string, int> difficulty_depth_map;
    difficulty_depth_map["easy"] = 2;
    difficulty_depth_map["medium"] = 4;
    difficulty_depth_map["hard"] = 6;

    Board board;

    std::string difficulty = argc >= 2 ? argv[1] : "medium";
    bool alpha_beta_pruning = argc >= 3 ? (std::string(argv[2]) == "prune") : true;
    bool player_first = argc >= 4 ? (std::string(argv[3]) == "player-first") : true;
    if (difficulty != "easy" && difficulty != "medium" && difficulty != "hard")
    {
        std::cout << "Correct usage: " << argv[0] << " [easy|medium|hard] [prune|no-prune] [ai-first|player-first]\n";
        return 1;
    }

    board.play(difficulty_depth_map[difficulty], player_first, alpha_beta_pruning);

    return 0;
}
