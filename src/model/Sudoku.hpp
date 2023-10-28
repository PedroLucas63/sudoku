/**
 * @file Sudoku.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief This header file contains the declarations for the Sudoku game,
 including data structures, functions, and the `Sudoku` class.
 *

 * @version 1.0
 * @date 2023-10-28
 *
 * Sudoku is a popular number puzzle game where the objective is to fill a 9x9
 grid with digits so that each column, each row, and each of the nine 3x3
 subgrids (also known as "blocks") that compose the grid contains all of the
 digits from 1 to 9, without repetition.
 *
 * The file defines the following key components:
 * - Structures `Board` and `Bank` for representing Sudoku boards and a bank of
 boards.
 * - Functions for saving and reading banks to/from files.
 * - The `Sudoku` class, which encapsulates the Sudoku game logic, including
 methods for inserting, removing, undoing, and checking values, as well as
 drawing the game grid.
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SUDOKU_HPP_
#define SUDOKU_HPP_

#include "Actions.hpp"
#include "format/fstring.hpp"
#include <array>
#include <string>
#include <vector>

/// Size of the Sudoku board.
constexpr int const BOARD_SIZE{9};

/**
 * @struct Board
 * @brief Represents a Sudoku board.
 */
struct Board {
   int m_id; ///< Identifier for the board.
   int m_values[BOARD_SIZE][BOARD_SIZE]{
       0}; ///< 2D array representing the Sudoku grid.
};

/**
 * @struct Bank
 * @brief Represents a bank of Sudoku boards.
 */
struct Bank {
   int m_id;                    ///< Identifier for the bank.
   int m_version;               ///< Version number for the bank.
   std::vector<Board> m_boards; ///< Vector of Sudoku boards.
};

/**
 * @brief Saves a bank to a file.
 * @param bank_ The bank to be saved.
 * @param file_ The file name for saving the bank.
 */
void saveBankToFile(Bank const &bank_, std::string const &file_);

/**
 * @brief Reads a bank from a file.
 * @param file_ The file name from which to read the bank.
 * @return The bank read from the file.
 */
Bank readBankFromFile(std::string const &file_);

/**
 * @class Sudoku
 * @brief Represents a Sudoku game.
 */
class Sudoku {
 private:
   Board m_board;         ///< The original Sudoku board.
   Board m_current_board; ///< The current state of the Sudoku board.
   int m_checks;          ///< The number of checks made during the game.
   std::vector<ActionGame> m_actions; ///< List of game actions.

   /**
    * @brief Replaces negative values in a board with zeros.
    * @param board_ The board to process.
    * @return The board with negative values replaced by zeros.
    */
   Board replaceNegatives(Board const &board_);

   /**
    * @brief Executes a list of save actions.
    * @param actions_ The list of actions to execute.
    */
   void executeSaveActions(std::vector<ActionGame> actions_);

   /**
    * @brief Checks if the current board is valid in a linear fashion.
    * @return True if the current board is valid; otherwise, false.
    */
   bool checkCurrentLinear() const;

   /**
    * @brief Checks if the current board is valid within blocks.
    * @return True if the current board is valid within blocks; otherwise,
    * false.
    */
   bool checkCurrentBlocks() const;

   /**
    * @brief Checks if the current board is valid.
    * @return True if the current board is valid; otherwise, false.
    */
   bool checkCurrent() const;

   /**
    * @brief Checks if a unique value can be inserted at a specified position.
    * @param x_ The x-coordinate of the position.
    * @param y_ The y-coordinate of the position.
    * @return True if a unique value can be inserted; otherwise, false.
    */
   bool checkUniqueValue(int x_, int y_) const;

   /**
    * @brief Draws the Sudoku board with specified colors.
    * @param correct_color_ Color for correctly placed values.
    * @param wrong_color_ Color for incorrectly placed values.
    * @param special_color_ Special color (optional) for specific elements.
    */
   void drawWithColors(short correct_color_, short wrong_color_,
                       short special_color_ = ext::cfg::none) const;

 public:
   /**
    * @brief Default constructor for Sudoku class.
    */
   Sudoku() {}

   /**
    * @brief Constructor with parameters for Sudoku class.
    * @param original_board_ The original Sudoku board.
    * @param checks_ The number of checks made during the game.
    * @param actions_ List of game actions (default empty).
    */
   Sudoku(Board const &original_board_, int checks_,
          std::vector<ActionGame> actions_ = {});

   /**
    * @brief Get the identifier of the current board.
    * @return The board identifier.
    */
   int getBoardId() const;

   /**
    * @brief Get the number of checks made during the game.
    * @return The number of checks.
    */
   int getChecks() const;

   /**
    * @brief Get the list of game actions.
    * @return The list of game actions.
    */
   std::vector<ActionGame> getActions() const;

   /**
    * @brief Get the count of game actions.
    * @return The count of game actions.
    */
   size_t getActionsCount() const;

   /**
    * @brief Get the counts of remaining digits for each value (1-9) that can be
    * placed on the Sudoku board.
    * @return An array where each element represents the count of remaining
    * occurrences for each digit value (1-9).
    */
   std::array<int, BOARD_SIZE> getDigitsLeft() const;

   /**
    * @brief Insert a value at the specified position.
    * @param value_ The value to insert.
    * @param x_ The x-coordinate of the position.
    * @param y_ The y-coordinate of the position.
    * @return A pair indicating success (true/false) and an associated message.
    */
   std::pair<bool, std::string> insert(int value_, int x_, int y_);

   /**
    * @brief Remove a value from the specified position.
    * @param x_ The x-coordinate of the position.
    * @param y_ The y-coordinate of the position.
    * @return A pair indicating success (true/false) and an associated message.
    */
   std::pair<bool, std::string> remove(int x_, int y_);

   /**
    * @brief Undo the last action.
    * @return A pair indicating success (true/false) and an associated message.
    */
   std::pair<bool, std::string> undo();

   /**
    * @brief Check the game for correctness.
    * @return A pair indicating success (true/false) and an associated message.
    */
   std::pair<bool, std::string> check();

   /**
    * @brief Draw the Sudoku board.
    */
   void draw() const;

   /**
    * @brief Draw the Sudoku board with correctness checks.
    */
   void drawCheck() const;

   /**
    * @brief Draw the Sudoku board with only incorrectly placed values.
    */
   void drawOnlyWrong() const;

   /**
    * @brief Check if the Sudoku board is completely filled.
    * @return True if the board is full; otherwise, false.
    */
   bool checkFull() const;

   /**
    * @brief Check if the player has won the game.
    * @return True if the player has won; otherwise, false.
    */
   bool checkWinner() const;
};

#endif /// SUDOKU_HPP_
