/**
 * @file Validate.cpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Implementation of Sudoku board and save data validation functions.
 * @version 1.0
 * @date 2023-10-28
 * 
 * @copyright Copyright (c) 2023
 */

#include "Validate.hpp"

/// @brief Check if the rows of a Sudoku board are valid.
/// @param board_ The Sudoku board to be checked.
/// @return True if all rows are valid; otherwise, false.
bool checkLines(Board const &board_) {
   // Iterate through each row of the Sudoku board.
   for (int row{0}; row != BOARD_SIZE; ++row) {
      int values[BOARD_SIZE]{
          0}; // Array to keep track of each value's occurrence.

      // Iterate through each column in the current row.
      for (int column{0}; column != BOARD_SIZE; ++column) {
         int absolute{std::abs(board_.m_values[row][column])};

         // Check if the value is out of the valid range (1-9).
         if (absolute < 1 || absolute > BOARD_SIZE) {
            return false; // If an invalid value is found, return false.
         } else {
            ++values[absolute -
                     1]; // Increment the count for the current value.
         }
      }

      // Ensure each value (1-9) appears exactly once in the row.
      for (int value{0}; value != BOARD_SIZE; ++value) {
         if (values[value] != 1) {
            return false; // If a value occurs more or less than once, return
                          // false.
         }
      }
   }

   return true; // If all rows are valid, return true.
}

/// @brief Check if the columns of a Sudoku board are valid.
/// @param board_ The Sudoku board to be checked.
/// @return True if all columns are valid; otherwise, false.
bool checkColumns(Board const &board_) {
   // Iterate through each column of the Sudoku board.
   for (int column{0}; column != BOARD_SIZE; ++column) {
      int values[BOARD_SIZE]{
          0}; // Array to keep track of each value's occurrence.

      // Iterate through each row in the current column.
      for (int row{0}; row != BOARD_SIZE; ++row) {
         int absolute{std::abs(board_.m_values[row][column])};

         // Check if the value is out of the valid range (1-9).
         if (absolute < 1 || absolute > BOARD_SIZE) {
            return false; // If an invalid value is found, return false.
         } else {
            ++values[absolute -
                     1]; // Increment the count for the current value.
         }
      }

      // Ensure each value (1-9) appears exactly once in the column.
      for (int value{0}; value != BOARD_SIZE; ++value) {
         if (values[value] != 1) {
            return false; // If a value occurs more or less than once, return
                          // false.
         }
      }
   }

   return true; // If all columns are valid, return true.
}

/// @brief Check if a Sudoku board is valid by verifying both rows and columns.
/// @param board_ The Sudoku board to be checked.
/// @return True if the board is valid; otherwise, false.
bool checkBoard(Board const &board_) {
   // Call the checkLines and checkColumns functions to validate the entire
   // board.
   return checkLines(board_) && checkColumns(board_);
}

/// @brief Validate a Sudoku bank by checking each board in it.
/// @param bank_ The Sudoku bank to be validated.
/// @return True if all boards in the bank are valid; otherwise, false.
bool validateBank(Bank const &bank_) {
   // Iterate through each board in the bank and check its validity using
   // checkBoard.
   for (Board board : bank_.m_boards) {
      if (!checkBoard(board)) {
         return false; // Return false if any board is invalid.
      }
   }

   return true; // If all boards are valid, return true.
}

/// @brief Validate a Sudoku save by checking each action in it.
/// @param save_ The Sudoku save to be validated.
/// @return True if all actions in the save are valid; otherwise, false.
bool validateSave(Save const &save_) {
   // Iterate through each action in the save and check its validity.
   for (ActionGame action : save_.m_actions) {
      if (action.m_x <= 0 || action.m_x > BOARD_SIZE) {
         return false; // Return false if the x-coordinate is out of range.
      } else if (action.m_y <= 0 || action.m_y > BOARD_SIZE) {
         return false; // Return false if the y-coordinate is out of range.
      } else if (action.m_action == Insert) {
         if (action.m_value <= 0 || action.m_value > BOARD_SIZE) {
            return false; // Return false if the value is out of range.
         }
      } else if (action.m_action == Remove && action.m_value != 0) {
         return false; // Return false if an invalid removal action is detected.
      }
   }

   return true; // If all actions are valid, return true.
}
