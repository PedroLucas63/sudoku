#include "Sudoku.hpp"
#include <algorithm>
#include <math.h>

Sudoku::Sudoku(Board const& original_board_, int checks_, std::initializer_list<ActionGame> actions_) {
   m_board = original_board_;
   m_current_board = original_board_; /// Substituir negativos por 0
   m_checks = checks_;
   std::copy(actions_.begin(), actions_.end(), m_actions.begin());
}

Board Sudoku::replaceNegatives(Board const& board_) {
   Board temp_board = board_;

   for (int y { 0 }; y != BOARD_SIZE; ++y) {
      for (int x { 0 }; x != BOARD_SIZE; ++x) {
         if (temp_board.m_values[y][x] < 0) {
            temp_board.m_values[y][x] = 0;
         }
      }
   }

   return temp_board;
}

bool Sudoku::checkCurrentLinear() {
   for (int y{0}; y != BOARD_SIZE; ++y) {
      int values_lin[BOARD_SIZE + 1]{0};
      int values_col[BOARD_SIZE + 1]{0};

      for (int x{0}; x != BOARD_SIZE; ++x) {
         int value_lin {m_current_board.m_values[y][x]};
         int value_col {m_current_board.m_values[x][y]};

         if (value_lin < 0 || value_lin > BOARD_SIZE) {
            return false;
         } else if (value_col < 0 || value_col > BOARD_SIZE) {
            return false;
         } else {
            ++values_lin[value_lin];
            ++values_col[value_col];
         }
      }

      for (int j{1}; j != BOARD_SIZE + 1; ++j) {
         if (values_lin[j] > 1 || values_col[j] > 1) {
            return false;
         }
      }
   }

   return true;
}

bool Sudoku::checkCurrentBlocks() {
   int sqrt { std::sqrt(BOARD_SIZE) };
   for (int y { 0 }; y != sqrt; ++y) {
      for (int x { 0 }; x != sqrt; ++x) {
         int values[BOARD_SIZE + 1] {0};
         int block_y {y * sqrt};
         int block_x {x * sqrt};

         while (block_y != block_y + sqrt) {
            int value {m_current_board.m_values[block_y][block_x]};

            if (value < 0 || value > BOARD_SIZE) {
               return false;
            } else {
               ++values[value];
            }

            if (block_x + 1 == block_y + sqrt) {
               block_x = 0;
               ++block_y;
            } else {
               ++block_x;
            }    
         }

         for (int j{1}; j != BOARD_SIZE + 1; ++j) {
            if (values[j] > 1) {
               return false;
            }
         }
      }
   }

   return true;
}

bool Sudoku::checkCurrent() {
   return checkCurrentLinear() && checkCurrentBlocks();
}