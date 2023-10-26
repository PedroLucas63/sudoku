#include "Save.hpp"
#include "Sudoku.hpp"
#include "Validate.hpp"
#include <iostream>

int main() {
   Bank bank{readBankFromFile("./data/basic.bak")};

   if (validateBank(bank)) {
      Save save{readGameFromFile("./save.sav")};

      if (validateSave(save)) {
         Sudoku game{bank.m_boards[1], 3, save.m_actions};

         game.check();
         game.drawCheck();

         game.undo();
         game.draw();
      } else {
         std::cout << "Save inválido!\n";
      }
   } else {
      std::cout << "Banco inválido!\n";
   }

}