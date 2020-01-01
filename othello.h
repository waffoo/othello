#pragma once
#include <memory>
#include "board.h"
#include "player.h"

using std::cin;

class Othello {
    Board board_;
    int turn_;
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;

public:
    Othello(int size) : board_(Board(size)), turn_(1) {}

    void register_player(Player* player, int turn = -1) {
        if (turn == 1) {
            if (not player1)
                player1.reset(player);
            else
                std::cout << "player1 has already been registered" << std::endl;
        } else if (turn == -1) {
            if (not player2)
                player2.reset(player);
            else
                std::cout << "player2 has already been registered" << std::endl;

        } else
            std::cout << "invalid turn parameter" << std::endl;
    }

    void run() {
        int row, col;
        pair<int, int> hand;
        while (not board_.finished()) {
            board_.display();
            board_.update_valid_table(turn_);

            std::cout << "\n"
                      << (turn_ > 0 ? 'o' : 'x') << "'s turn." << std::endl;
            if (board_.pass()) {
                std::cout << "but no valid position for "
                          << (turn_ > 0 ? 'o' : 'x') << ". pass." << std::endl;
            } else {
                if (turn_ == 1 and player1) {
                    auto p = player1->next(board_);
                    row = p.first, col = p.second;
                    std::cout << "player1's hand is (" << row << ", " << col
                              << ")" << std::endl;
                } else if (turn_ == -1 and player2) {
                    auto p = player2->next(board_);
                    row = p.first, col = p.second;
                    std::cout << "player2's hand is (" << row << ", " << col
                              << ")" << std::endl;
                } else {
                    do {
                        std::cout << "input [row] [col] > ";
                        cin >> row >> col;

                    } while (not board_.available(row, col));
                }

                int err = board_.put(row, col, turn_);
                if (err < 0) {
                    std::cout << "ABORT." << std::endl;
                    exit(1);
                }
            }

            turn_ *= -1;
        }

        board_.display();
        board_.show_result();
    }
};