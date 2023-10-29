/**
 * @file Sudoku.cpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Implementation of classes related to the Sudoku game.
 * @version 1.0
 * @date 2023-10-28
 * 
 * @copyright Copyright (c) 2023
 */

#include "Sudoku.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>

/// @brief Saves a bank of Sudoku boards to a binary file.
/// @param bank_ The Bank object to be saved to the file.
/// @param file_ The file name where the bank will be saved.
void saveBankToFile(Bank const &bank_, std::string const &file_) {
   std::ofstream file{file_, std::ios::out | std::ios::binary};

   // Save the bank's identifier to the file.
   int id{bank_.m_id};
   file.write(reinterpret_cast<char *>(&id), sizeof(id));

   // Save the bank's version number to the file.
   int version{bank_.m_version};
   file.write(reinterpret_cast<char *>(&version), sizeof(version));

   // Save the number of boards in the bank to the file.
   int quant_of_boards{static_cast<int>(bank_.m_boards.size())};
   file.write(reinterpret_cast<char *>(&quant_of_boards),
              sizeof(quant_of_boards));

   // Save each board in the bank to the file.
   for (Board board : bank_.m_boards) {
      file.write(reinterpret_cast<char *>(&board), sizeof(board));
   }

   file.close();
}

/// @brief Reads a bank of Sudoku boards from a binary file.
/// @param file_ The file name from which to read the bank.
/// @return The Bank object read from the file.
Bank readBankFromFile(std::string const &file_) {
   std::ifstream file{file_, std::ios::in | std::ios::binary};

   Bank bank;
   int quant_of_boards{0};

   // Read the bank's identifier from the file.
   file.read(reinterpret_cast<char *>(&bank.m_id), sizeof(bank.m_id));

   // Read the bank's version number from the file.
   file.read(reinterpret_cast<char *>(&bank.m_version), sizeof(bank.m_version));

   // Read the number of boards in the bank from the file.
   file.read(reinterpret_cast<char *>(&quant_of_boards),
             sizeof(quant_of_boards));

   // Read each board in the bank from the file and add it to the Bank object.
   for (int index{0}; index != quant_of_boards; ++index) {
      Board board;
      file.read(reinterpret_cast<char *>(&board), sizeof(board));
      bank.m_boards.push_back(board);
   }

   file.close();

   return bank;
}

/// @brief Constructor for the Sudoku class.
/// @param original_board_ The original Sudoku board to initialize the game.
/// @param checks_ The number of checks or moves allowed in the game.
/// @param actions_ A vector of previous game actions.
Sudoku::Sudoku(Board const &original_board_, int checks_,
               std::vector<ActionGame> actions_) {
   // Initialize the Sudoku board and game parameters.
   m_board = original_board_;
   m_current_board = replaceNegatives(original_board_);
   m_checks = checks_;

   // Reserve memory for the actions and copy the provided actions.
   m_actions.reserve(actions_.size());
   std::copy(
       actions_.begin(), actions_.end(),
       m_actions.begin()); /// Executar as ações de entrada no current_board

   // Execute save actions to update the current board based on previous
   // actions.
   executeSaveActions(actions_);
}

/// @brief Replaces negative values in the Sudoku board with zeros.
/// @param board_ The input Sudoku board to process.
/// @return A new board with negative values replaced by zeros.
Board Sudoku::replaceNegatives(Board const &board_) {
   // Create a temporary board to hold the modified values.
   Board temp_board = board_;

   // Iterate through each cell in the board.
   for (int row{0}; row != BOARD_SIZE; ++row) {
      for (int column{0}; column != BOARD_SIZE; ++column) {
         // Check if the value in the cell is negative.
         if (temp_board.m_values[row][column] < 0) {
            // Replace negative values with zeros.
            temp_board.m_values[row][column] = 0;
         }
      }
   }

   return temp_board; // Return the modified board.
}

/// @brief Executes a series of saved game actions to update the Sudoku board.
/// @param actions_ A vector of previous game actions to apply to the board.
void Sudoku::executeSaveActions(std::vector<ActionGame> actions_) {
   for (auto action : actions_) {
      switch (action.m_action) {
      case Insert:
         // If the action is to insert a value, call the insert method.
         insert(action.m_value, action.m_x, action.m_y);
         break;
      case Remove:
         // If the action is to remove a value, call the remove method.
         remove(action.m_x, action.m_y);
         break;
      default:
         // Handle any other actions (if needed).
         break;
      }
   }
}
/// @brief Checks the current Sudoku board for correctness in linear regions
/// (rows and columns).
/// @return True if the linear regions are correct, otherwise false.
bool Sudoku::checkCurrentLinear() const {
   for (int row{0}; row != BOARD_SIZE; ++row) {
      int values_row[BOARD_SIZE + 1]{0};
      int values_column[BOARD_SIZE + 1]{0};

      for (int column{0}; column != BOARD_SIZE; ++column) {
         int value_row{m_current_board.m_values[row][column]};
         int value_column{m_current_board.m_values[column][row]};

         // Check if values are out of bounds.
         if (value_row < 0 || value_row > BOARD_SIZE) {
            return false;
         } else if (value_column < 0 || value_column > BOARD_SIZE) {
            return false;
         } else {
            ++values_row[value_row];
            ++values_column[value_column];
         }
      }

      // Check for duplicate values in rows and columns.
      for (int value{1}; value != BOARD_SIZE + 1; ++value) {
         if (values_row[value] > 1 || values_column[value] > 1) {
            return false;
         }
      }
   }

   return true; // Linear regions are correct.
}

/// @brief Checks the current Sudoku board for correctness in square blocks.
/// @return True if the square blocks are correct, otherwise false.
bool Sudoku::checkCurrentBlocks() const {
   int sqrt{static_cast<int>(std::sqrt(BOARD_SIZE))};

   for (int row{0}; row != sqrt; ++row) {
      for (int column{0}; column != sqrt; ++column) {
         int values[BOARD_SIZE + 1]{0};
         int block_row{row * sqrt};
         int block_column{column * sqrt};
         int initial_block_column{block_column};

         int last_row{block_row + sqrt};
         int last_column{block_column + sqrt};

         while (block_row != last_row) {
            int value{m_current_board.m_values[block_row][block_column]};

            // Check if values are out of bounds.
            if (value < 0 || value > BOARD_SIZE) {
               return false; // Value is out of bounds.
            } else {
               ++values[value]; // Count occurrences of each value in the block.
            }

            if (block_column + 1 == last_column) {
               block_column = initial_block_column;
               ++block_row;
            } else {
               ++block_column;
            }
         }

         // Check for duplicate values within the square block.
         for (int value{1}; value != BOARD_SIZE + 1; ++value) {
            if (values[value] > 1) {
               return false; // Duplicate value found in the block.
            }
         }
      }
   }

   return true; // Square blocks are correct.
}

/// @brief Checks the current Sudoku board for correctness by combining linear
/// and block checks.
/// @return True if the board is correct, otherwise false.
bool Sudoku::checkCurrent() const {
   // Check correctness in linear regions (rows and columns) and square blocks.
   return checkCurrentLinear() && checkCurrentBlocks();
}

/// @brief Checks if a value in a specific cell is unique within its row,
/// column, and square block.
/// @param x_ The x-coordinate (column) of the cell.
/// @param y_ The y-coordinate (row) of the cell.
/// @return True if the value is unique within its row, column, and square
/// block, otherwise false.
bool Sudoku::checkUniqueValue(int x_, int y_) const {
   int value{m_current_board.m_values[y_ - 1][x_ - 1]};

   // Check uniqueness in the same row.
   for (int column{0}; column != BOARD_SIZE; ++column) {
      int test_value{m_current_board.m_values[y_ - 1][column]};

      if (test_value == value && column + 1 != x_) {
         return false; // Value is not unique in the row.
      }
   }

   // Check uniqueness in the same column.
   for (int row{0}; row != BOARD_SIZE; ++row) {
      int test_value{m_current_board.m_values[row][x_ - 1]};

      if (test_value == value && row + 1 != y_) {
         return false; // Value is not unique in the column.
      }
   }

   int sqrt{static_cast<int>(std::sqrt(BOARD_SIZE))};
   int initial_row{(y_ - 1) / sqrt * sqrt};
   int initial_column{(x_ - 1) / sqrt * sqrt};

   // Check uniqueness in the same square block.
   for (int row{initial_row}; row != initial_row + sqrt; ++row) {
      for (int column{initial_column}; column != initial_column + sqrt;
           ++column) {
         int test_value{m_current_board.m_values[row][column]};

         if (test_value == value && (row + 1 != y_ || column + 1 != x_)) {
            return false; // Value is not unique in the square block.
         }
      }
   }

   return true; // Value is unique in its row, column, and square block.
}

/// @brief Draws the Sudoku board with colored text.
/// @param correct_color_ The color for correct values.
/// @param wrong_color_ The color for incorrect values.
/// @param special_color_ The color for special values (optional).
void Sudoku::drawWithColors(short correct_color_, short wrong_color_,
                            short special_color_) const {
   // Define the top line of the board.
   std::string board_table{"    +-------+-------+-------+"};
   size_t max_size_in_line{board_table.size()};
   int width_empty{7};
   int sqrt_board{static_cast<int>(std::sqrt(BOARD_SIZE))};

   // Create and configure the first line to indicate the last action.
   ext::fstring first_line{max_size_in_line, ' '};
   if (!m_actions.empty()) {
      int x{m_actions.back().m_x};

      int reference_location{width_empty +
                             2 * (((x - 1) / sqrt_board) + (x - 1)) - 1};

      first_line[reference_location] = 'v';
      first_line.color(ext::cfg::bright_yellow);
      first_line.style(ext::stl::bold);
   }

   std::cout << first_line << "\n";

   // Create and configure the second line with column numbers.
   ext::fstring second_line{max_size_in_line, ' '};
   for (int reference{1}; reference != BOARD_SIZE + 1; ++reference) {
      int reference_location{
          width_empty + 2 * (((reference - 1) / sqrt_board) + (reference - 1)) -
          1};
      second_line[reference_location] = reference + '0';
   }
   second_line.color(ext::cfg::green);
   std::cout << second_line << "\n";

   std::cout << board_table << "\n";

   // Loop through each row of the Sudoku board.
   for (int line{0}; line != BOARD_SIZE; ++line) {
      if (line != 0 && line % sqrt_board == 0) {
         std::cout << "    |-------+-------+-------|\n";
      }

      // Check if there is a recent action on this row.
      if (!m_actions.empty()) {
         int row{m_actions.back().m_y};

         // If the action was on this row, indicate it with a marker.
         if (row == line + 1) {
            ext::fstring reference{">"};
            reference.color(ext::cfg::bright_yellow);
            reference.style(ext::stl::bold);
            std::cout << reference;
         } else {
            std::cout << " ";
         }
      } else {
         std::cout << " ";
      }

      // Display the row character (A, B, C, etc.).
      ext::fstring character{" "};
      character.push_back(line + 'A');
      character.color(ext::cfg::green);
      std::cout << character << " ";

      // Loop through each cell in the row.
      for (int number{0}; number != BOARD_SIZE; ++number) {
         if (number % sqrt_board == 0) {
            std::cout << "| ";
         }

         int current_value{m_current_board.m_values[line][number]};
         int original_value{m_board.m_values[line][number]};

         // Check if the cell is empty.
         if (current_value == 0) {
            std::cout << "  ";
         }
         // Check if the value is the same as the original board.
         else if (current_value == original_value) {
            ext::fstring original_str{std::to_string(current_value)};
            original_str.style(ext::stl::bold);
            std::cout << original_str << " ";
         }
         // Check for a special value with unique color.
         else if (!m_actions.empty() &&
                  !checkUniqueValue(number + 1, line + 1) &&
                  special_color_ != ext::cfg::none) {
            ext::fstring special_value{std::to_string(current_value)};
            special_value.color(special_color_);
            std::cout << special_value << " ";
         }
         // Check for a correct value with the specified color.
         else if (current_value == -original_value) {
            ext::fstring correct_value{std::to_string(current_value)};
            correct_value.color(correct_color_);
            std::cout << correct_value << " ";
         }
         // If none of the above conditions are met, consider it an incorrect
         // value.
         else {
            ext::fstring incorrect_value{std::to_string(current_value)};
            incorrect_value.color(wrong_color_);
            std::cout << incorrect_value << " ";
         }
      }

      std::cout << "|\n";
   }

   std::cout << board_table << "\n";
}

/// @brief Get the identifier of the current Sudoku board.
/// @return The identifier of the current Sudoku board.
int Sudoku::getBoardId() const { return m_board.m_id; }

/// @brief Get the number of checks made on the Sudoku board.
/// @return The number of checks made.
int Sudoku::getChecks() const { return m_checks; }

/// @brief Get the list of actions performed on the Sudoku board.
/// @return A vector containing the actions on the board.
std::vector<ActionGame> Sudoku::getActions() const { return m_actions; }

/// @brief Get the count of actions performed on the Sudoku board.
/// @return The count of actions in the history.
size_t Sudoku::getActionsCount() const { return m_actions.size(); }

/// @brief Get the count of remaining digits for each number in the Sudoku
/// board.
/// @return An array representing the count of remaining digits for each number.
std::array<int, BOARD_SIZE> Sudoku::getDigitsLeft() const {
   std::array<int, BOARD_SIZE> digits;
   digits.fill(BOARD_SIZE);

   for (int y{0}; y != BOARD_SIZE; ++y) {
      for (int x{0}; x != BOARD_SIZE; ++x) {
         int value{m_current_board.m_values[y][x]};

         // If a value is found in the cell, decrement the corresponding digit
         // count.
         if (value != 0) {
            --digits[value - 1];
         }
      }
   }

   return digits;
}

/// @brief Inserts a value into the Sudoku board at a specified coordinate.
/// @param value_ The value to be inserted.
/// @param x_ The X coordinate (column) for the insertion.
/// @param y_ The Y coordinate (row) for the insertion.
/// @return A pair indicating the success of the insertion and an optional
/// message.

std::pair<bool, std::string> Sudoku::insert(int value_, int x_, int y_) {
   // Check if the value is within a valid range.
   if (value_ <= 0 || value_ > BOARD_SIZE) {
      return {false, "The value passed for insertion is outside the range."};
   }

   // Check if the X coordinate is within the board limits.
   if (x_ <= 0 || x_ > BOARD_SIZE) {
      return {false, "The passed X coordinate is outside the board limits."};
   }

   // Check if the Y coordinate is within the board limits.
   if (y_ <= 0 || y_ > BOARD_SIZE) {
      return {false, "The passed Y coordinate is outside the board limits."};
   }

   // Check if the specified coordinate is fixed and cannot be changed.
   if (m_board.m_values[y_ - 1][x_ - 1] > 0) {
      return {false, "The passed coordinate cannot be changed."};
   }

   // Check if the insertion has already been performed previously.
   if (m_current_board.m_values[y_ - 1][x_ - 1] == value_) {
      return {false, "This insertion has already been performed previously."};
   }

   // Update the current board with the inserted value.
   m_current_board.m_values[y_ - 1][x_ - 1] = value_;

   // Create an action object for the insertion and add it to the actions
   // history.
   ActionGame action;
   action.m_x = x_;
   action.m_y = y_;
   action.m_value = value_;
   action.m_action = Insert;

   m_actions.push_back(action);

   // Check if the move is valid but could be improved.
   if (!checkUniqueValue(x_, y_)) {
      return {true, "The move is valid, but it would be better if you paid "
                    "more attention."};
   }

   // The move is valid and successful.
   return {true, ""};
}

/// @brief Removes a value from the Sudoku board at a specified coordinate.
/// @param x_ The X coordinate (column) for the removal.
/// @param y_ The Y coordinate (row) for the removal.
/// @return A pair indicating the success of the removal and an optional
/// message.

std::pair<bool, std::string> Sudoku::remove(int x_, int y_) {
   // Check if the X coordinate is within the board limits.
   if (x_ <= 0 || x_ > BOARD_SIZE) {
      return {false, "The passed X coordinate is outside the board limits."};
   }

   // Check if the Y coordinate is within the board limits.
   if (y_ <= 0 || y_ > BOARD_SIZE) {
      return {false, "The passed Y coordinate is outside the board limits."};
   }

   // Check if the specified coordinate is fixed and cannot be changed.
   if (m_board.m_values[y_ - 1][x_ - 1] > 0) {
      return {false, "The passed coordinate cannot be changed."};
   }

   // Check if the specified coordinate has no value to remove.
   if (m_board.m_values[y_ - 1][x_ - 1] == 0) {
      return {false, "The passed coordinate has no value."};
   }

   // Remove the value from the current board.
   m_current_board.m_values[y_ - 1][x_ - 1] = 0;

   // Create an action object for the removal and add it to the actions history.
   ActionGame action;
   action.m_x = x_;
   action.m_y = y_;
   action.m_action = Remove;
   m_actions.push_back(action);

   // The removal is successful.
   return {true, ""};
}

/// @brief Undoes the last action (insertion or removal) in the Sudoku game.
/// @return A pair indicating the success of the undo operation and an optional
/// message.

std::pair<bool, std::string> Sudoku::undo() {
   // Check if there are no moves to be undone.
   if (m_actions.empty()) {
      return {false, "There are no moves to be undone."};
   }

   // Copy the current actions and clear the action history.
   auto actions{m_actions};
   m_actions.clear();

   // Remove the last action from the copied list.
   actions.pop_back();

   // Reset the current board to its original state by replacing negatives.
   m_current_board = replaceNegatives(m_board);

   // Execute the saved actions to bring the current board up to date.
   executeSaveActions(actions);

   // The undo operation is successful.
   return {true, ""};
}

/// @brief Performs a check operation in the Sudoku game to verify the board's
/// validity.
/// @return A pair indicating the success of the check operation and an optional
/// message.

std::pair<bool, std::string> Sudoku::check() {
   // Check if the number of available checks has been exhausted.
   if (m_checks == 0) {
      return {false, "The number of checks has already been exhausted."};
   }

   // Decrement the remaining number of checks.
   --m_checks;

   // The check operation is successful.
   return {true, ""};
}

/// @brief Draws the Sudoku board with default colors for correct and incorrect
/// values.
void Sudoku::draw() const {
   drawWithColors(ext::cfg::bright_blue, ext::cfg::bright_blue,
                  ext::cfg::bright_red);
}

/// @brief Draws the Sudoku board with specific colors for check operation
/// results.
void Sudoku::drawCheck() const {
   drawWithColors(ext::cfg::bright_green, ext::cfg::bright_red);
}

/// @brief Draws the Sudoku board with specific colors for highlighting
/// incorrect values.
void Sudoku::drawOnlyWrong() const {
   drawWithColors(ext::cfg::bright_blue, ext::cfg::bright_red);
}

/// @brief Checks if the Sudoku board is completely filled with valid values.
/// @return True if the board is full, false otherwise.
bool Sudoku::checkFull() const {
   for (int row{0}; row != BOARD_SIZE; ++row) {
      for (int column{0}; column != BOARD_SIZE; ++column) {
         int value{m_current_board.m_values[row][column]};
         if (value == 0) {
            return false;
         }
      }
   }

   return true;
}

/// @brief Checks if the Sudoku board represents a winning state.
/// @return True if the board is a winning state, false otherwise.
bool Sudoku::checkWinner() const {
   for (int row{0}; row != BOARD_SIZE; ++row) {
      for (int column{0}; column != BOARD_SIZE; ++column) {
         int value{m_current_board.m_values[row][column]};
         int correct_value{std::abs(m_board.m_values[row][column])};

         if (value != correct_value) {
            return false;
         }
      }
   }

   return true;
}
