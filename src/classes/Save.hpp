#ifndef SAVE_HPP_
#define SAVE_HPP_

#include <vector>
#include "Actions.hpp"

struct Save {
   int m_bank_id;
   int m_bank_version;
   int m_board_id;
   std::vector<ActionGame> m_actions;
   int m_checks;
};

#endif /// SAVE_HPP_