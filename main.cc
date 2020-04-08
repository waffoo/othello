#include <iostream>

#include "alpha_beta_player.h"
#include "board.h"
#include "othello.h"
#include "random_player.h"

int main() {
    int sz = 8;
    /*
    do {
        cout << "board size? (even positive integer) > ";
        cin >> sz;
    } while (sz <= 0 or sz % 2 > 0);
    */

    Othello ot(sz, false);
    ot.register_player(new AlphaBetaPlayer(true, 4));  // o
    ot.register_player(new RandomPlayer(false));       // x
    int times = 100;
    int win_cnt = 0;
    for (int i = 0; i < times; i++) {
        int res = ot.run();
        if (res > 0) win_cnt++;
    }
    cout << (double)win_cnt / times << endl;
}
