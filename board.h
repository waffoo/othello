#pragma once
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::pair;
using std::vector;

class Board {
    int board_size_;
    vector<vector<int>> board_;
    vector<vector<bool>> valid_;
    int blank_num_;
    int result_;
    bool pass_;
    int pass_cnt_;
    vector<pair<int, int>> dirs_ = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                                    {0, 1},   {1, -1}, {1, 0},  {1, 1}};

    char num_to_mark(int n) const {
        switch (n) {
            case -1:
                return 'x';
            case 0:
                return ' ';
            case 1:
                return 'o';
            default:
                return '-';
        }
    }

    bool on_board(int row, int col) const {
        return 0 <= row and row < board_size_ and 0 <= col and
               col < board_size_;
    }

    bool valid_mark(int mark) const { return abs(mark) == 1; }

    bool calc_valid(int row, int col, int mark) {
        if (not is_blank(row, col)) return false;
        int other = -mark;
        int nowr, nowc;
        for (auto d : dirs_) {
            nowr = row + d.first, nowc = col + d.second;
            if (not(on_board(nowr, nowc) and board_[nowr][nowc] == other))
                continue;

            bool found = false;
            while (true) {
                nowr += d.first, nowc += d.second;
                if (not on_board(nowr, nowc) or is_blank(nowr, nowc))
                    break;
                else if (board_[nowr][nowc] == mark) {
                    found = true;
                    break;
                }
            }
            if (found) return true;
        }

        return false;
    }

    bool is_blank(int row, int col) const { return board_[row][col] == 0; }
    bool no_blank() const { return blank_num_ == 0; }

    // pass
    bool deadlock() const { return pass_cnt_ > 1; }

public:
    Board(int size)
        : board_size_(size),
          board_(vector<vector<int>>(size, vector<int>(size))),
          valid_(vector<vector<bool>>(size, vector<bool>(size))),
          blank_num_(size * size - 4),
          result_(0),
          pass_(false),
          pass_cnt_(0) {
        int mid = size / 2;
        board_[mid - 1][mid - 1] = 1;
        board_[mid][mid] = 1;
        board_[mid - 1][mid] = -1;
        board_[mid][mid - 1] = -1;

        update_valid_table(1);
    }

    void display() const {
        cout << "\n    ";
        for (int i = 0; i < board_size_; i++) cout << i << "   ";
        cout << endl;

        cout << "  ";
        for (int i = 0; i < board_size_ * 4 + 1; i++) cout << '-';
        cout << endl;

        for (int i = 0; i < board_size_; i++) {
            cout << i << " | ";
            for (int j = 0; j < board_size_; j++) {
                cout << num_to_mark(board_[i][j]) << " | ";
            }
            cout << endl;

            cout << "  ";
            for (int j = 0; j < board_size_ * 4 + 1; j++) cout << '-';
            cout << endl;
        }
    }

    void update_valid_table(int mark) {
        pass_ = true;
        for (int i = 0; i < board_size_; i++)
            for (int j = 0; j < board_size_; j++) {
                valid_[i][j] = calc_valid(i, j, mark);
                if (pass_ and valid_[i][j]) pass_ = false;
            }
    }

    int size() const { return board_size_; }
    vector<vector<bool>> get_valid_table() const { return valid_; }
    vector<vector<int>> get_board() const { return board_; }

    bool pass() {
        if (pass_)
            pass_cnt_++;
        else
            pass_cnt_ = 0;
        return pass_;
    }

    bool available(int row, int col) const {
        bool res = on_board(row, col) and valid_[row][col];
        if (not res)
            cout << "pos (" << row << ", " << col << ") is not valid.\n"
                 << endl;
        return res;
    }

    int put(int row, int col, int mark) {
        if (not valid_mark(mark)) return -1;
        if (not available(row, col)) return -1;

        board_[row][col] = mark;
        blank_num_--;

        int other = -mark;
        int nowr, nowc;
        for (auto d : dirs_) {
            nowr = row + d.first, nowc = col + d.second;
            if (not(on_board(nowr, nowc) and board_[nowr][nowc] == other))
                continue;

            bool found = false;
            while (true) {
                nowr += d.first, nowc += d.second;
                if (not on_board(nowr, nowc) or is_blank(nowr, nowc))
                    break;
                else if (board_[nowr][nowc] == mark) {
                    found = true;
                    break;
                }
            }
            if (found) {
                nowr -= d.first, nowc -= d.second;
                while (not(nowr == row and nowc == col)) {
                    board_[nowr][nowc] *= -1;
                    nowr -= d.first, nowc -= d.second;
                }
            }
        }

        return 0;
    }

    bool finished() const { return no_blank() or deadlock(); }

    void show_result() const {
        int ocnt = 0;
        int xcnt = 0;
        for (int i = 0; i < board_size_; i++)
            for (int j = 0; j < board_size_; j++)
                if (board_[i][j] == 1)
                    ocnt++;
                else if (board_[i][j] == -1)
                    xcnt++;

        cout << "o has " << ocnt << " stones." << endl;
        cout << "x has " << xcnt << " stones." << endl;

        if (ocnt > xcnt)
            cout << "o wins!!!!" << endl;
        else if (xcnt > ocnt)
            cout << "x wins!!!!!" << endl;
        else
            cout << "draw game!" << endl;
    }
};