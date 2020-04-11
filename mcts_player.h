#pragma once
#include <cmath>
#include <random>
#include <vector>

#include "board.h"
#include "player.h"

class MCTS {
public:
    struct Node {
        std::vector<int> child;
        int cnt = 0;
        int total_reward = 0;
    };

    MCTS(std::vector<Node> tree, int threshold = 3)
        : tree_(tree), threshold_(threshold) {
        tree_.push_back(Node{});
    }

    void run(int times = 10000) {
        for (int i = 0; i < times; i++) {
            if (i % 10000 == 0) cout << "sample " << i << endl;
            Board board(8);
            search(board, 0, 1);
        }
    }

    vector<Node>& get_tree() { return this->tree_; }

    int max_child(vector<int>& child) {
        int total = 0;
        for (int x : child) total += tree_[x].cnt;
        const float c = sqrt(2);

        int max_idx = -1;
        float max_score = -1;
        for (int i = 0; i < child.size(); i++) {
            Node& node = tree_[child[i]];
            float base = node.cnt > 0 ? (float)node.total_reward / node.cnt : 0;
            float score = base + c * sqrt(log(total) / (1 + node.cnt));

            if (score > max_score) {
                max_score = score;
                max_idx = i;
            }
        }
        return max_idx;
    }

    int search(Board& bd, int idx, int turn) {
        bd.update_valid_table(turn);
        Node& node = tree_[idx];
        node.cnt++;

        int res = -1;
        if (bd.finished()) {
            res = bd.winner() == turn ? 1 : 0;
            node.total_reward += res;
            return 1 - res;
        }

        if (bd.pass()) {
            node.cnt--;
            res = search(bd, idx, -turn);
        } else {
            const vector<pair<int, int>>& moves = bd.get_candidates();

            if (node.child.size() > 0) {
                int move_num = max_child(node.child);
                auto [row, col] = moves[move_num];
                bd.put(row, col, turn);
                res = search(bd, node.child[move_num], -turn);
            } else {
                if (node.cnt < threshold_) {
                    res = rollout(bd, turn);
                } else {
                    vector<int> child;
                    for (int i = 0; i < moves.size(); i++) {
                        child.push_back(tree_.size());
                        tree_.push_back(Node());
                    }
                    tree_[idx].child = child;
                    res = search(bd, child[0], -turn);
                }
            }
        }

        tree_[idx].total_reward += res;
        return 1 - res;
    }

    int rollout(Board& bd, int turn) {
        int first_mark = turn;
        while (not bd.finished()) {
            bd.update_valid_table(turn);

            if (not bd.pass()) {
                const auto& moves = bd.get_candidates();
                auto [row, col] = moves[rnd() % moves.size()];
                bd.put(row, col, turn);
            }

            turn *= -1;
        }

        return bd.winner() == first_mark ? 1 : 0;
    }

private:
    std::random_device rnd;
    std::vector<Node> tree_;
    int threshold_;
};

class MCTSPlayer : public Player {
    std::random_device rnd;
    vector<MCTS::Node>& tree_;
    int tree_idx_;

public:
    MCTSPlayer(vector<MCTS::Node>& tree, bool first = true)
        : Player(first), tree_(tree), tree_idx_(0) {}
    std::pair<int, int> next(const Board& bd) override {
        const auto& candidate = bd.get_candidates();
        if (tree_[tree_idx_].child.size() == 0) {
            cout << "child not found. random behaviour" << endl;
            return candidate[rnd() % candidate.size()];
        }

        auto [prev_idx, prev_mark] = bd.get_prev_move();
        if (prev_mark == -this->mark_) {
            tree_idx_ = tree_[tree_idx_].child[prev_idx];
            if (tree_[tree_idx_].child.size() == 0) {
                cout << "child not found. random behaviour" << endl;
                return candidate[rnd() % candidate.size()];
            }
        }

        if (tree_[tree_idx_].child.size() != candidate.size()) {
            cout << "invalid tree child num. ABORT" << endl;
            exit(1);
        }

        int max_val = -1;
        int max_idx = -1;
        int max_tree_idx = -1;
        for (int i = 0; i < tree_[tree_idx_].child.size(); i++) {
            int child_idx = tree_[tree_idx_].child[i];
            if (tree_[child_idx].cnt > max_val) {
                max_val = tree_[child_idx].cnt;
                max_idx = i;
                max_tree_idx = child_idx;
            }
        }

        tree_idx_ = max_tree_idx;

        return candidate[max_idx];
    }
};