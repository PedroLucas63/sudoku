#include "Save.hpp"
#include "Sudoku.hpp"
#include <iostream>

int main() {
   Bank bank{readBankFromFile("./data/basic.bak")};
   Save save{readGameFromFile("./save.sav")};
   Sudoku game{bank.m_boards[1], 3, save.m_actions};

   game.check();
   game.drawCheck();

   game.undo();
   game.draw();
}