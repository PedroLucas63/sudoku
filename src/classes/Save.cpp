#include "Save.hpp"
#include <fstream>

void saveGameToFile(Save const &save_, std::string file_) {
   std::ofstream file{file_, std::ios::out | std::ios::binary};

   int bank_id{save_.m_bank_id};
   int version{save_.m_bank_version};
   int board_id{save_.m_board_id};
   int checks{save_.m_checks};
   int quant_of_actions{static_cast<int>(save_.m_actions.size())};

   file.write(reinterpret_cast<char *>(&bank_id), sizeof(bank_id));
   file.write(reinterpret_cast<char *>(&version), sizeof(version));
   file.write(reinterpret_cast<char *>(&board_id), sizeof(board_id));
   file.write(reinterpret_cast<char *>(&checks), sizeof(checks));
   file.write(reinterpret_cast<char *>(&quant_of_actions),
              sizeof(quant_of_actions));

   for (ActionGame act : save_.m_actions) {
      file.write(reinterpret_cast<char *>(&act), sizeof(act));
   }

   file.close();
}

Save readGameFromFile(std::string file_) {
   std::ifstream file{file_, std::ios::in | std::ios::binary};

   Save save_game;

   int quant_of_actions{0};

   file.read(reinterpret_cast<char *>(&save_game.m_bank_id),
             sizeof(save_game.m_bank_id));
   file.read(reinterpret_cast<char *>(&save_game.m_bank_version),
             sizeof(save_game.m_bank_version));
   file.read(reinterpret_cast<char *>(&save_game.m_board_id),
             sizeof(save_game.m_board_id));
   file.read(reinterpret_cast<char *>(&save_game.m_checks),
             sizeof(save_game.m_checks));
   file.read(reinterpret_cast<char *>(&quant_of_actions),
             sizeof(quant_of_actions));

   for (int act{0}; act != quant_of_actions; ++act) {
      ActionGame action;
      file.read(reinterpret_cast<char *>(&action), sizeof(action));
      save_game.m_actions.push_back(action);
   }

   file.close();

   return save_game;
}