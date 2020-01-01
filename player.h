#pragma once
#include <utility>
#include "board.h"

class Player {
public:
    Player() {}
    virtual std::pair<int, int> next(const Board&) = 0;
};