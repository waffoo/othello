#include <iostream>

#include "alpha_beta_player.h"
#include "board.h"
#include "mcts_player.h"
#include "othello.h"
#include "random_player.h"

int main() {
    vector<MCTS::Node> tree;
    MCTS mcts(tree);
    mcts.run(1e6);
    auto& trained_tree = mcts.get_tree();

    Othello ot(8);

    ot.register_player(new MCTSPlayer(trained_tree, true));  // o
    ot.register_player(new RandomPlayer(false));             // x

    int times = 1;
    int win_cnt = 0;
    for (int i = 0; i < times; i++) {
        int res = ot.run();
        if (res > 0) win_cnt++;
    }
    cout << (double)win_cnt / times << endl;
}
