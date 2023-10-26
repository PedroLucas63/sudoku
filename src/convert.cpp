#include "Sudoku.hpp"
#include "Validate.hpp"
#include "cli/Cli.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::string const file_name{"data.sdk"};

Board getBoard(std::ifstream &file_) {
   int val;
   int i{0};
   int j{0};

   Board b;

   while (i != BOARD_SIZE && file_ >> std::ws >> val) {
      b.m_values[i][j++] = val;

      if (j == BOARD_SIZE) {
         j = 0;
         ++i;
      }
   }

   return b;
}

std::vector<Board> getBoards(std::string in_file_) {
   std::vector<Board> boards;
   std::ifstream file{in_file_};

   int id{0};

   while (true) {
      Board b = getBoard(file);
      b.m_id = id;
      if (checkBoard(b)) {
         boards.push_back(b);
         ++id;
      } else {
         break;
      }
   }

   return boards;
}

void processFile(int id_, int version_, std::string in_file_) {
   std::cout << " >>> Converting file...\n";

   Bank bank;
   bank.m_id = id_;
   bank.m_version = version_;
   bank.m_boards = getBoards(in_file_);

   if (bank.m_boards.empty()) {
      std::cout << " >>> File not convert!\n";
      return;
   }

   saveBankToFile(bank, file_name);
   std::cout << " >>> File convert!\n";
}

int main(int argc, char *argv[]) {
   ext::CLI cli;
   cli.addArgumentInteger('i', "id");
   cli.addArgumentInteger('v', "version");
   cli.addArgumentInFile('d', "data");

   cli.parse(argc, argv);

   int id{cli.receiveInteger("-i")};
   int version{cli.receiveInteger("-v")};
   std::string input_file{cli.receiveInFile("-d")};

   if (id == int() || input_file == std::string()) {
      return 1;
   }

   processFile(id, version, input_file);

   return 0;
}