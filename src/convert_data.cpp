#include "lib/cli/Cli.hpp"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

std::string const file_name {"data.bak"};
short const size { 9 };

struct Board {
   int id { 0 };
   int board[size][size] { };
};

bool checkLines(Board const& board_) {
   for (int i { 0 }; i != size; ++i) {
      int values[size] { 0 };
      for (int j { 0 }; j != size; ++j) {
         int abs { std::abs(board_.board[i][j]) };
         if (abs < 1 || abs > size) {
            return false;
         } else {
            ++values[abs - 1];
         }
      }
      for (int j { 0 }; j != size; ++j) {
         if (values[j] != 1) {
            return false;
         }
      }
   }

   return true;
}

bool checkColumns(Board const& board_) {
   for (int i { 0 }; i != size; ++i) {
      int values[size] { 0 };
      for (int j { 0 }; j != size; ++j) {
         int abs { std::abs(board_.board[j][i]) };
         if (abs < 1 || abs > size) {
            return false;
         } else {
            ++values[abs - 1];
         }
      }
      for (int j { 0 }; j != size; ++j) {
         if (values[j] != 1) {
            return false;
         }
      }
   }

   return true;
}

bool checkBoard(Board const& board_) {
   return checkLines(board_) && checkColumns(board_);
}

Board getBoard(std::ifstream& file_) {
   int val;
   int i { 0 };
   int j { 0 };

   Board b;

   while (i != size && file_ >> std::ws >> val) {
      b.board[i][j++] = val;

      if (j == size) {
         j = 0;
         ++i;
      }
   }

   return b;
}

std::vector<Board> getBoards(std::string in_file_) {
   std::vector<Board> boards;
   std::ifstream file {in_file_};

   int id { 0 };

   while (true) { 
      Board b = getBoard(file);
      b.id = id;
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
   std::vector<Board> boards {getBoards(in_file_)};

   std::ofstream file {file_name, std::ios::binary};

   size_t quant { boards.size() };

   file.write((char*)(&id_), sizeof(id_));
   file.write((char*)(&version_), sizeof(version_));
   file.write((char*)(&quant), sizeof(quant));

   for (Board b : boards) {
      file.write((char*)(&b), sizeof(b));
   }

   file.close();
}

int main(int argc, char* argv[]) {
   ext::CLI cli;
   cli.addArgumentInteger('i', "id");
   cli.addArgumentInteger('v', "version");
   cli.addArgumentInFile('d', "data");

   cli.parse(argc, argv);

   int id { cli.receiveInteger("-i")};
   int version { cli.receiveInteger("-v")};
   std::string input_file { cli.receiveInFile("-d")};

   if (id == int() || input_file == std::string()) {
      return 1;
   }

   processFile(id, version, input_file);

   return 0;
}