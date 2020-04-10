#pragma once
#include "player.h"

class FirstPlayer : public Player {
public:
    FirstPlayer(bool first = true) : Player(first) {}
    std::pair<int, int> next(const Board& bd) override {
        return bd.get_candidates().front();
    }
};