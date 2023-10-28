/**
 * @file convert.cpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Convert a ".txt" data to ".sdk" bank.
 * @version 1.0
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 *
 * @details
 * This file format is a binary format that contains the following data:
 * - id (int): The identifier of the Sudoku bank.
 * - version (int): The version of the Sudoku bank.
 * - Quantity of boards (int): The number of Sudoku boards in the bank.
 * - Boards: Each board is represented as a struct with the following format:
 *     struct Board {
 *       int m_id;                              // The ID of the board.
 *       int m_values[BOARD_SIZE][BOARD_SIZE];  // The Sudoku board itself, a
 * 9x9 grid of integers.
 *     };
 */

#include "Sudoku.hpp"
#include "Validate.hpp"
#include "cli/Cli.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief The filename for saving and loading Sudoku bank data.
 *
 * This constant string represents the filename used for saving and loading
 * Sudoku bank data in the
 * ".sdk" file format. It is a default filename used by the program for managing
 * Sudoku data.
 */
std::string const FILENAME{"data.sdk"};

/**
 * @brief Reads a Sudoku board from an input file stream.
 *
 * This function reads a Sudoku board from the provided input file stream.
 *
 * @param input_file_ An input file stream containing the Sudoku board data.
 * @return The parsed Sudoku board.
 */
Board getBoard(std::ifstream &input_file_);

/**
 * @brief Reads multiple Sudoku boards from an input file.
 *
 * This function reads multiple Sudoku boards from the provided input file.
 *
 * @param input_file_ The path to the input file.
 * @return A vector of parsed Sudoku boards.
 */
std::vector<Board> getBoards(std::string input_file_);

/**
 * @brief Processes an input file, converts it to a Sudoku bank, and saves it to
 * a file.
 *
 * This function processes an input file, converts it to a Sudoku bank, and
 * saves it to a specified file.
 *
 * @param id_ The bank's identifier.
 * @param version_ The bank's version.
 * @param input_file_ The path to the input file.
 */
void processFile(int id_, int version_, std::string input_file_);

/**
 * @brief The main function of the Sudoku bank converter.
 *
 * This function handles command-line arguments, processes the input file,
 * and converts it into a Sudoku bank file.
 *
 * @param argc Number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return An integer representing the exit status of the program.
 */
int main(int argc, char *argv[]) {
   // Create a command-line argument parser
   ext::CLI cli;
   cli.addArgumentInteger('i', "id");
   cli.addArgumentInteger('v', "version");
   cli.addArgumentInFile('d', "data");

   // Parse command-line arguments
   cli.parse(argc, argv);

   int id{cli.receiveInteger("-i")};
   int version{cli.receiveInteger("-v")};
   std::string input_file{cli.receiveInFile("-d")};

   // Check if required parameters are provided
   if (id == int() || input_file == std::string()) {
      return 1;
   }

   // Process the input file and create a Sudoku bank
   processFile(id, version, input_file);

   return 0;
}

/// @brief Reads a Sudoku board from an input file stream.

Board getBoard(std::ifstream &input_file_) {
   int value;
   int row{0};
   int column{0};

   Board board;

   while (row != BOARD_SIZE && input_file_ >> std::ws >> value) {
      // Populate the Sudoku board with values
      board.m_values[row][column++] = value;

      if (column == BOARD_SIZE) {
         column = 0;
         ++row;
      }
   }

   return board;
}

/// @brief Reads multiple Sudoku boards from an input file.

std::vector<Board> getBoards(std::string input_file_) {
   std::vector<Board> boards;
   std::ifstream file{input_file_};

   int id{0};

   while (true) {
      // Read a Sudoku board and set its ID
      Board board = getBoard(file);
      board.m_id = id;
      if (checkBoard(board)) {
         boards.push_back(board);
         ++id;
      } else {
         break;
      }
   }

   return boards;
}

/// @brief Processes an input file, converts it to a Sudoku bank, and saves it
/// to a file.
void processFile(int id_, int version_, std::string input_file_) {
   std::cout << " >>> Converting file...\n";

   Bank bank;
   bank.m_id = id_;
   bank.m_version = version_;
   bank.m_boards = getBoards(input_file_);

   if (bank.m_boards.empty()) {
      std::cout << " >>> File not converted!\n";
      return;
   }

   // Save the Sudoku bank to a file
   saveBankToFile(bank, FILENAME);
   std::cout << " >>> File converted!\n";
}
