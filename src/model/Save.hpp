/**
 * @file Save.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Defines structures and functions for saving and loading Sudoku games.
 * @version 1.0
 * @date 2023-10-28
 *
 * This file contains structures and functions related to saving and loading
 * Sudoku games. The "Save" struct represents a saved game state, including the
 * bank and board IDs, a sequence of game actions, and the number of checks made
 * during the game. Additionally, it provides functions to save a game state to
 * a file and read a game state from a file.
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SAVE_HPP_
#define SAVE_HPP_

#include "Actions.hpp"
#include <string>
#include <vector>

struct Save {
   int m_bank_id; /**< The ID of the bank associated with the saved game. */
   int m_bank_version; /**< The version of the bank associated with the saved
                          game. */
   int m_board_id; /**< The ID of the board associated with the saved game. */
   std::vector<ActionGame> m_actions; /**< A sequence of game actions taken
                                         during the saved game. */
   int m_checks; /**< The number of checks made during the saved game. */
};

/**
 * @brief Saves a game state to a file.
 * @param save_ The game state to be saved.
 * @param file_ The filename for the saved game file.
 */
void saveGameToFile(Save const &save_, std::string file_);

/**
 * @brief Reads a game state from a file.
 * @param file_ The filename of the saved game file.
 * @return The game state loaded from the file.
 */
Save readGameFromFile(std::string file_);

#endif /// SAVE_HPP_
