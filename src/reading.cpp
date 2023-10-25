#include "Sudoku.hpp"
#include "cli/Cli.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
   ext::CLI cli;
   cli.addArgumentOutFile('d', "data", {".bak"});

   cli.parse(argc, argv);

   std::string output_file{cli.receiveOutFile("-d")};

   if (output_file == std::string()) {
      return 1;
   }

   Bank bank{readBankFromFile(output_file)};

   std::cout << " >>> Bank id: " << bank.m_id << "\n";
   std::cout << " >>> Bank version: " << bank.m_version << "\n";
   std::cout << " >>> Boards: " << bank.m_boards.size() << "\n";

   for (auto b : bank.m_boards) {
      Sudoku board{b, 3};
      std::cout << "\n >>> Board id: " << board.getBoardId() << "\n";
      board.draw();
   }

   return 0;
}