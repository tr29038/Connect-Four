#include "board.h"

#include <iostream>

int main(int argc, char* argv[])
{
    std::unordered_map<std::string, int> difficulty_depth_map;
    difficulty_depth_map["easy"] = 3;
    difficulty_depth_map["medium"] = 5;
    difficulty_depth_map["hard"] = 7;

    Board board;

    std::string difficulty = argc == 2 ? argv[1] : "medium";
    bool player_first = argc == 3 ? true : argv[2] == "player-first";
    if (difficulty != "easy" && difficulty != "medium" && difficulty != "hard")
    {
        std::cout << "Correct usage: " << argv[0] << " [easy|medium|hard] [ai-first|player-first]\n";
        return 1;
    }

    board.play(difficulty_depth_map[difficulty], player_first);

    return 0;
}
