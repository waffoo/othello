#pragma once
#include <chrono>
#include <climits>
#include <iostream>
#include <vector>

#include "player.h"

class AlphaBetaPlayer : public Player {
    int limit;
    int cntcnt = 0;

    int determined(const Board& bd) {
        const auto& board = bd.get_board();
        int sum = 0;

        int ul = board[0][0];
        if (abs(ul) > 0) {
            sum += ul;
            for (int i = 1; i < 8; i++) {
                if (board[0][i] == ul)
                    sum += ul;
                else
                    break;
            }
            for (int i = 1; i < 8; i++) {
                if (board[i][0] == ul) {
                    sum += ul;
                    if (i == 7) sum -= ul * 7;
                } else
                    break;
            }
        }

        int ur = board[0][7];
        if (abs(ur) > 0) {
            sum += ur;
            for (int i = 1; i < 8; i++) {
                if (board[i][7] == ur)
                    sum += ur;
                else
                    break;
            }
            for (int i = 6; i >= 0; i--) {
                if (board[0][i] == ur) {
                    sum += ur;
                    if (i == 0) sum -= ur * 7;
                } else
                    break;
            }
        }

        int lr = board[7][7];
        if (abs(lr) > 0) {
            sum += lr;
            for (int i = 6; i >= 0; i--) {
                if (board[7][i] == lr)
                    sum += lr;
                else
                    break;
            }
            for (int i = 6; i >= 0; i--) {
                if (board[i][7] == lr) {
                    sum += lr;
                    if (i == 0) sum -= lr * 7;
                } else
                    break;
            }
        }

        int ll = board[7][0];
        if (abs(ll) > 0) {
            sum += ll;
            for (int i = 6; i >= 0; i--) {
                if (board[i][0] == ll)
                    sum += ll;
                else
                    break;
            }
            for (int i = 1; i < 8; i++) {
                if (board[7][i] == ll) {
                    sum += ll;
                    if (i == 7) sum -= ll * 7;
                } else
                    break;
            }
        }

        return sum;
    }

    int point(int row, int col) {
        int r = std::min(row, 7 - row), c = std::min(col, 7 - col);
        if (r < c) std::swap(r, c);
        if (r == 0 and c == 0) return 45;
        if (r == 1 and c == 0) return -11;
        if (r == 2 and c == 0) return 4;
        if (r == 3 and c == 0) return -1;
        if (r == 1 and c == 1) return -16;
        if (r == 2 and c == 1) return -1;
        if (r == 3 and c == 1) return -3;
        if (r == 2 and c == 2) return 2;
        if (r == 3 and c == 2) return -1;
        if (r == 3 and c == 3) return 0;
        return -1000000;
    }

    int eval_board(const Board& bd) {
        const auto& board = bd.get_board();
        int sum = 0;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (board[row][col] == this->mark_)
                    sum += point(row, col);
                else if (board[row][col] == this->mark_)
                    sum -= point(row, col);
            }
        }

        int det = this->determined(bd) * 20;
        sum += det * this->mark_;

        return sum;
    }

    int dfs(Board bd, int depth, int turn, int alpha, int beta) {
        this->cntcnt++;
        if (depth == this->limit) return this->eval_board(bd);
        if (bd.finished()) {
            constexpr int result = 10000;
            int win = bd.winner();
            if (win == this->mark_)
                return eval_board(bd) + result;
            else if (win == -this->mark_)
                return eval_board(bd) - result;
        }

        bd.update_valid_table(turn);

        if (bd.pass()) return dfs(bd, depth + 1, -turn, alpha, beta);

        const auto& moves = bd.get_candidates();

        if (turn == this->mark_) {
            int max_val = -INT_MAX;
            for (auto [i, j] : moves) {
                auto next_board = bd;
                next_board.put(i, j, turn);
                int eval = dfs(next_board, depth + 1, -turn, alpha, beta);
                if (eval > max_val) {
                    max_val = eval;
                    alpha = max_val;
                }
                if (alpha >= beta) return max_val;
            }
            return max_val;

        } else {
            int min_val = INT_MAX;
            for (auto [i, j] : moves) {
                auto next_board = bd;
                next_board.put(i, j, turn);
                int eval = dfs(next_board, depth + 1, -turn, alpha, beta);
                if (eval < min_val) {
                    min_val = eval;
                    beta = min_val;
                }
                if (alpha >= beta) return min_val;
            }
            return min_val;
        }
    }

public:
    AlphaBetaPlayer(bool first = true, int limit = 6)
        : Player(first), limit(limit) {}

    std::pair<int, int> next(const Board& bd) override {
        auto st = std::chrono::system_clock::now();
        this->cntcnt = 0;

        const auto& board = bd.get_board();
        const auto& moves = bd.get_candidates();

        int max_eval = -10000000;
        std::pair<int, int> best_hand = {-1, -1};

        for (auto [i, j] : moves) {
            auto next_board = bd;
            next_board.put(i, j, this->mark_);

            int eval = dfs(next_board, 1, -this->mark_, -INT_MAX, INT_MAX);
            if (eval > max_eval) {
                max_eval = eval;
                best_hand = {i, j};
            }
        }

        auto ed = std::chrono::system_clock::now();
        double elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(ed - st)
                .count();
        std::cout << "cnt = " << this->cntcnt << " elapsed : " << elapsed
                  << " ms" << std::endl;

        return best_hand;
    }
};