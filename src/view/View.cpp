#include "View.hpp"
#include <iostream>
#include "SudokuConfig.h"
#include "format/fstring.hpp"
#include <sstream>

void renderTitle() {
   size_t width { 50 };

   ext::fstring bars {width, '='};
   bars.color(ext::cfg::blue);

   std::ostringstream oss;
   oss << "Welcome to a terminal version of Sudoku, v" << sudoku_VERSION_MAJOR << "." << sudoku_VERSION_MINOR << "";
   ext::fstring title { oss.str() };
   title.align_center(width);
   title.color(ext::cfg::blue);

   ext::fstring copyright { "Copyright (C) 2023, Pedro Lucas M. Nascimento" };
   copyright.align_center(width);
   copyright.color(ext::cfg::blue);

   std::cout << bars << "\n";
   std::cout << title << "\n"; 
   std::cout << copyright << "\n";
   std::cout << bars << "\n";
}

void renderHelper(int default_checks_) {
   renderTitle();
   std::cout << "Usage: sudoku [-c <num>] [-h] <input_puzzle_file>\n";
   std::cout << "  Game options:\n";
   std::cout << "    -c <num> Number of checks per game. Default = " << default_checks_ << ".\n";
   std::cout << "    -h       Print this help text.\n";
}

void renderSelectData(std::vector<std::string> datas_) {
   renderTitle();
   std::cout << "\n";

   for (size_t index { 0 }; index != datas_.size(); ++index) {
      std::cout << index << ". " << datas_[index] << "\n";
   }

   std::cout << "\n >>> Enter one of the listed game banks: ";
}
