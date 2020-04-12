#pragma once
#include <memory>
#include <stack>

#include "board.h"
#include "player.h"

using std::cin;

class Othello {
    Board board_;
    std::stack<Board> history_;
    int size_;
    int turn_;
    bool display_;
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;

public:
    Othello(int size, bool display = true)
        : board_(Board(size)), size_(size), turn_(1), display_(display) {}

    void register_player(Player* player) {
        int turn = player->is_first() ? 1 : -1;
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

    int run() {
        if (not this->display_ and (not player1 or not player2)) {
            std::cout << "No-Display mode cannot be started. You have to "
                         "register two players. ABORT."
                      << std::endl;
            exit(1);
        }
        this->board_ = Board(this->size_);
        int row, col;
        pair<int, int> hand;
        while (not board_.finished()) {
            if (this->display_) board_.display();
            board_.update_valid_table(turn_);

            if (this->display_)
                std::cout << "\n"
                          << (turn_ > 0 ? 'o' : 'x') << "'s turn." << std::endl;
            if (board_.pass()) {
                if (this->display_)
                    std::cout << "but no valid position for "
                              << (turn_ > 0 ? 'o' : 'x') << ". pass."
                              << std::endl;
            } else {
                if (turn_ == 1 and player1) {
                    auto p = player1->next(board_);
                    row = p.first, col = p.second;
                    if (this->display_)
                        std::cout << "player1's hand is (" << row << ", " << col
                                  << ")" << std::endl;
                } else if (turn_ == -1 and player2) {
                    auto p = player2->next(board_);
                    row = p.first, col = p.second;
                    if (this->display_)
                        std::cout << "player2's hand is (" << row << ", " << col
                                  << ")" << std::endl;
                } else {
                    do {
                        std::cout << "input [row] [col] > ";
                        cin >> row >> col;

                        if ((row == -1 or row == -2) and
                            history_.size() >= abs(row))
                            break;

                    } while (not board_.available(row, col));
                }

                if (row == -1) {
                    board_ = history_.top();
                    history_.pop();
                } else if (row == -2) {
                    history_.pop();
                    board_ = history_.top();
                    history_.pop();
                    turn_ *= -1;
                } else {
                    history_.push(board_);
                    int err = board_.put(row, col, turn_);
                    if (err < 0) {
                        std::cout << "ABORT." << std::endl;
                        exit(1);
                    }
                }
            }

            turn_ *= -1;
        }

        board_.display();
        board_.show_result();
        return board_.winner();
    }
};