#include "View.hpp"
#include <iostream>
#include "SudokuConfig.h"


void renderHelper(int default_checks_) {
   std::cout << "Sudoku v" << sudoku_VERSION_MAJOR << "." << sudoku_VERSION_MINOR << "\n";
   std::cout << "Usage: sudoku [-c <num>] [-h] <input_puzzle_file>\n";
   std::cout << "  Game options:\n";
   std::cout << "    -c <num> Number of checks per game. Default = " << default_checks_ << ".\n";
   std::cout << "    -h       Print this help text.\n";
}
