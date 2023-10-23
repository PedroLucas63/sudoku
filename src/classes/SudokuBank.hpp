#ifndef SUDOKU_BANK_HPP_
#define SUDOKU_BANK_HPP_

#include "SudokuBoard.hpp"
#include <vector>

class SudokuBank {
 private:
   int m_id;
   int m_version;
   std::vector<SudokuBoard> m_games;

 public:
   SudokuBank(int id_, int version_ = 1) : m_id{id_}, m_version{version_} {}

   int getId() const { return m_id; }

   int getVersion() const { return m_version; }

   bool setGame(SudokuBoard const &game_) {
      for (SudokuBoard &game : m_games) {
         if (game.m_id == game_.m_id) {
            return false;
         }
      }

      m_games.push_back(game_);
      return true;
   }

   SudokuBoard getGame(size_t index_) {
      if (index_ >= m_games.size()) {
         return {};
      }

      return m_games[index_];
   }

   size_t getQuantifyOfGames() const { return m_games.size(); }
};

#endif /// SUDOKU_BANK_HPP_