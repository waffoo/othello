#pragma once
#include <utility>

#include "board.h"

class Player {
protected:
    int mark_;

public:
    Player(bool first = true) : mark_(first ? 1 : -1) {}
    bool is_first() { return mark_ > 0; }
    virtual std::pair<int, int> next(const Board&) = 0;
};