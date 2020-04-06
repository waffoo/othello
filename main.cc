#include <iostream>

#include "board.h"
#include "first_player.h"
#include "othello.h"

int main() {
    int sz = 8;
    /*
    do {
        cout << "board size? (even positive integer) > ";
        cin >> sz;
    } while (sz <= 0 or sz % 2 > 0);
    */

    Othello ot(sz);
    ot.register_player(new FirstPlayer());
    ot.register_player(new FirstPlayer(false));

    ot.run();
}