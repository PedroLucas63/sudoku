#ifndef SUDOKU_BOARD_HPP_
#define SUDOKU_BOARD_HPP_

constexpr int const BOARD_SIZE{9};

struct SudokuBoard {
   int m_id{0};
   int m_board[BOARD_SIZE][BOARD_SIZE]{0};
};

#endif /// SUDOKU_BOARD_HPP_