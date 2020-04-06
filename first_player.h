#pragma once
#include "player.h"

class FirstPlayer : public Player {
public:
    FirstPlayer(bool first = true) : Player(first) {}
    std::pair<int, int> next(const Board& bd) override {
        const auto& valid = bd.get_valid_table();
        const auto& board = bd.get_board();

        for (int i = 0; i < bd.size(); i++) {
            for (int j = 0; j < bd.size(); j++) {
                if (valid[i][j]) return {i, j};
            }
        }
        return {-1, -1};
    }
};