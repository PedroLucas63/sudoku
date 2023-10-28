/**
 * @file Save.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Defines the structures and functions for saving and loading Sudoku
 * game states.
 * @version 1.0
 * @date 2023-10-28
 *
 * This file contains the definitions of structures and functions necessary for
 * saving and loading Sudoku game states. This includes the 'Save' data
 * structure, which stores information about the game state, as well as
 * functions for saving and loading games in binary format.
 *
 * @see Actions.hpp
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Save.hpp"
#include <fstream>

/// @brief Saves a Sudoku game state to a binary file.
/// @param save_ The Sudoku game state to be saved.
/// @param file_ The filename for the saved game file.
void saveGameToFile(Save const &save_, std::string file_) {
   // Open the binary file for writing.
   std::ofstream file{file_, std::ios::out | std::ios::binary};

   // Extract various components from the game state.
   int bank_id{save_.m_bank_id};
   int version{save_.m_bank_version};
   int board_id{save_.m_board_id};
   int checks{save_.m_checks};
   int quant_of_actions{static_cast<int>(save_.m_actions.size())};

   // Write bank-related information to the file.
   file.write(reinterpret_cast<char *>(&bank_id), sizeof(bank_id));
   file.write(reinterpret_cast<char *>(&version), sizeof(version));
   file.write(reinterpret_cast<char *>(&board_id), sizeof(board_id));

   // Write the number of checks and the quantity of game actions to the file.
   file.write(reinterpret_cast<char *>(&checks), sizeof(checks));
   file.write(reinterpret_cast<char *>(&quant_of_actions),
              sizeof(quant_of_actions));

   // Write each game action to the file.
   for (ActionGame action : save_.m_actions) {
      file.write(reinterpret_cast<char *>(&action), sizeof(action));
   }

   // Close the file after writing the game state.
   file.close();
}

/// @brief Reads a saved Sudoku game state from a binary file.
/// @param file_ The filename of the saved game file.
/// @return The loaded Sudoku game state.
Save readGameFromFile(std::string file_) {
   // Open the binary file for reading.
   std::ifstream file{file_, std::ios::in | std::ios::binary};

   Save save_game; // Create an instance to store the loaded game state.

   int quant_of_actions{
       0}; // Initialize the variable to store the number of actions.

   // Read the game state information from the file.
   file.read(reinterpret_cast<char *>(&save_game.m_bank_id),
             sizeof(save_game.m_bank_id));
   file.read(reinterpret_cast<char *>(&save_game.m_bank_version),
             sizeof(save_game.m_bank_version));
   file.read(reinterpret_cast<char *>(&save_game.m_board_id),
             sizeof(save_game.m_board_id));
   file.read(reinterpret_cast<char *>(&save_game.m_checks),
             sizeof(save_game.m_checks));
   file.read(reinterpret_cast<char *>(&quant_of_actions),
             sizeof(quant_of_actions));

   // Read and add each game action to the list of actions in the loaded game
   // state.
   for (int index{0}; index != quant_of_actions; ++index) {
      ActionGame action;
      file.read(reinterpret_cast<char *>(&action), sizeof(action));
      save_game.m_actions.push_back(action);
   }

   // Close the file after reading the game state.
   file.close();

   return save_game; // Return the loaded game state.
}
