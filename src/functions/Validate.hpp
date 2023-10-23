#include "SudokuBoard.hpp"
#include <cmath>

bool checkLines(SudokuBoard const &game_) {
   for (int y{0}; y != BOARD_SIZE; ++y) {
      int values[BOARD_SIZE]{0};
      for (int x{0}; x != BOARD_SIZE; ++x) {
         int abs{std::abs(game_.m_board[y][x])};
         if (abs < 1 || abs > BOARD_SIZE) {
            return false;
         } else {
            ++values[abs - 1];
         }
      }
      for (int j{0}; j != BOARD_SIZE; ++j) {
         if (values[j] != 1) {
            return false;
         }
      }
   }

   return true;
}

bool checkColumns(SudokuBoard const &game_) {
   for (int y{0}; y != BOARD_SIZE; ++y) {
      int values[BOARD_SIZE]{0};
      for (int x{0}; x != BOARD_SIZE; ++x) {
         int abs{std::abs(game_.m_board[x][y])};
         if (abs < 1 || abs > BOARD_SIZE) {
            return false;
         } else {
            ++values[abs - 1];
         }
      }
      for (int j{0}; j != BOARD_SIZE; ++j) {
         if (values[j] != 1) {
            return false;
         }
      }
   }

   return true;
}

bool checkBoard(SudokuBoard const &game_) {
   return checkLines(game_) && checkColumns(game_);
}
