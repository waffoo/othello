#pragma once
#include <random>

#include "player.h"

class RandomPlayer : public Player {
    std::random_device rnd;

public:
    RandomPlayer(bool first = true) : Player(first) {}
    std::pair<int, int> next(const Board& bd) override {
        const auto& candidate = bd.get_candidates();
        return candidate[rnd() % candidate.size()];
    }
};