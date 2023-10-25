#ifndef SAVE_GAME_HPP_
#define SAVE_GAME_HPP_

#include "ActionGame.hpp"
#include <vector>

class SaveGame {
 private:
   int m_id_bank;
   int m_id_game;
   int m_checks;
   std::vector<ActionGame> m_actions;

 public:
   SaveGame(int id_bank_, int id_game_, int checks_)
       : m_id_bank{id_bank_}, m_id_game{id_game_}, m_checks{checks_} {}

   int getIdBank() const { return m_id_bank; }

   int getIdGame() const { return m_id_game; }

   int getChecks() const { return m_checks; }

   void pushAction(ActionGame const &action_) { m_actions.push_back(action_); }

   bool popAction() {
      if (m_actions.empty() || m_checks == 0) {
         return false;
      }

      m_actions.pop_back();
      --m_checks;

      return true;
   }

   bool actionsEmpty() const { return m_actions.empty(); }

   size_t getQuantifyOfAction() const { return m_actions.size(); }

   auto begin() const { return m_actions.begin(); }

   auto end() const { return m_actions.end(); }
};

#endif /// SAVE_GAME_HPP_