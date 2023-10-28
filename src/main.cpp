/**
 * @file main.cpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Sudoku is a classic number puzzle game.
 * @version 1.0
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "GameController.hpp"

/**
 * @brief The main function of the Sudoku game.
 *
 * This function initializes the game, runs the game loop, and cleans up
 * resources when the game is finished.
 *
 * @param argc Number of command-line arguments.
 * @param argv An array of command-line arguments.
 *
 * @return An integer representing the exit status of the program.
 */
int main(int argc, char *argv[]) {
   // Create an instance of the GameController
   GameController &game{GameController::getInstance()};

   // Initialize the game with command-line arguments
   game.initialize(argc, argv);

   // Main game loop
   while (!game.exit()) {
      // Process user input
      game.process();

      // Update the game state
      game.update();

      // Render the game to the screen
      game.render();
   }

   // Clean up and exit the game
   game.destruct();
   return 0;
}
