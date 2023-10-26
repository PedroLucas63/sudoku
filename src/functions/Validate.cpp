#include "Validate.hpp"

bool checkLines(Board const &board_) {
   for (int i{0}; i != BOARD_SIZE; ++i) {
      int values[BOARD_SIZE]{0};
      for (int j{0}; j != BOARD_SIZE; ++j) {
         int abs{std::abs(board_.m_values[i][j])};
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

bool checkColumns(Board const &board_) {
   for (int i{0}; i != BOARD_SIZE; ++i) {
      int values[BOARD_SIZE]{0};
      for (int j{0}; j != BOARD_SIZE; ++j) {
         int abs{std::abs(board_.m_values[j][i])};
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

bool checkBoard(Board const &board_) {
   return checkLines(board_) && checkColumns(board_);
}

bool validateBank(Bank const& bank_) {
   for (Board board : bank_.m_boards) {
      if (!checkBoard(board)) {
         return false;
      }
   }

   return true;
}

bool validateSave(Save const& save_) {
   for (ActionGame action : save_.m_actions) {
      if (action.m_x <= 0 || action.m_x > BOARD_SIZE) {
         return false;
      } else if (action.m_y <= 0 || action.m_y > BOARD_SIZE) {
         return false;
      } else if (action.m_action == Insert) {
         if (action.m_value <= 0 || action.m_value > BOARD_SIZE) {
            return false;
         }
      } else if (action.m_action == Remove && action.m_value != 0) {
            return false;
      }
   }

   return true;
}
