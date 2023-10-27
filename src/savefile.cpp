#include "Save.hpp"
#include "Sudoku.hpp"

int main() {
   Bank bank{readBankFromFile("./data/basic.sdk")};
   Sudoku game{bank.m_boards[1], 3};

   game.insert(1, 1, 2);
   game.insert(3, 4, 2);
   game.insert(3, 9, 7);
   game.insert(9, 8, 6);
   game.insert(5, 4, 3);
   game.insert(9, 3, 3);

   game.check();
   game.drawCheck();

   Save save;
   save.m_bank_id = bank.m_id;
   save.m_bank_version = bank.m_version;
   save.m_board_id = game.getBoardId();
   save.m_checks = game.getChecks();
   save.m_actions = game.getActions();

   saveGameToFile(save, "save.sav");
}