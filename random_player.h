#pragma once
#include <random>

#include "player.h"

class RandomPlayer : public Player {
    std::random_device rnd;

public:
    RandomPlayer(bool first = true) : Player(first) {}
    std::pair<int, int> next(const Board& bd) override {
        const auto& valid = bd.get_valid_table();
        const auto& board = bd.get_board();

        vector<pair<int, int>> candidate;
        for (int i = 0; i < bd.size(); i++)
            for (int j = 0; j < bd.size(); j++)
                if (valid[i][j]) candidate.push_back({i, j});

        return candidate[rnd() % candidate.size()];
    }
};