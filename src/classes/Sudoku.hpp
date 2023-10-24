#ifndef SUDOKU_HPP_
#define SUDOKU_HPP_

#include <vector>
#include "Actions.hpp"

constexpr int const BOARD_SIZE{9};

struct Board {
   int m_id;
   int m_values[BOARD_SIZE][BOARD_SIZE]{0};
};

struct Bank {
   int m_id;
   int m_version;
   std::vector<Board> m_boards;
};

class Sudoku {
private:
   Board m_board;
   Board m_current_board;
   int m_checks;
   std::vector<ActionGame> m_actions;

   Board replaceNegatives(Board const& board_);

   bool checkCurrentLinear();
   bool checkCurrentBlocks();
   bool checkCurrent();

public:
Sudoku(Board const& original_board_, int checks_, std::initializer_list<ActionGame> actions_ = {});
};

#endif /// SUDOKU_HPP_