/**
 * @file convert.cpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Test the conversion of a ".txt" file to a ".sdk" bank using the
 * previous program.
 * @version 1.0
 * @date 2023-10-28
 *
 * @details
 * This program is used to test the conversion of a ".txt" file containing
 * Sudoku data to a ".sdk" bank using the functionalities provided by the
 * previous program. It reads a Sudoku bank from a ".sdk" file, displays bank
 * information, and prints each Sudoku board.
 */

#include "Sudoku.hpp"
#include "cli/Cli.hpp"
#include <iostream>

/**
 * @brief The main function of the Sudoku data conversion test program.
 *
 * This function tests the conversion of a ".txt" file to a ".sdk" bank using
 * the previous program's functionality.
 *
 * @param argc Number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return An integer representing the exit status of the program.
 */
int main(int argc, char *argv[]) {
   // Create a command-line argument parser
   ext::CLI cli;
   cli.addArgumentOutFile('d', "data", {".sdk"});

   // Parse command-line arguments
   cli.parse(argc, argv);

   // Retrieve the output file name
   std::string output_file{cli.receiveOutFile("-d")};

   // Check if a valid output file name is provided
   if (output_file == std::string()) {
      return 1;
   }

   // Read the Sudoku bank from the specified ".sdk" file
   Bank bank{readBankFromFile(output_file)};

   // Display bank information
   std::cout << " >>> Bank id: " << bank.m_id << "\n";
   std::cout << " >>> Bank version: " << bank.m_version << "\n";
   std::cout << " >>> Boards: " << bank.m_boards.size() << "\n";

   // Iterate through each Sudoku board and print it
   for (auto board : bank.m_boards) {
      Sudoku game{board, 3};
      std::cout << "\n >>> Board id: " << game.getBoardId() << "\n";
      game.draw();
   }

   return 0;
}
