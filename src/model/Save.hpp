#ifndef SAVE_HPP_
#define SAVE_HPP_

#include "Actions.hpp"
#include <string>
#include <vector>

struct Save {
   int m_bank_id;
   int m_bank_version;
   int m_board_id;
   std::vector<ActionGame> m_actions;
   int m_checks;
};

void saveGameToFile(Save const &save_, std::string file_);
Save readGameFromFile(std::string file_);

#endif /// SAVE_HPP_