#ifndef SUDOKU_HPP_
#define SUDOKU_HPP_

#include "Actions.hpp"
#include "format/fstring.hpp"
#include <array>
#include <string>
#include <vector>

constexpr int const BOARD_SIZE{9};

struct Board {
   int m_id;
   int m_values[BOARD_SIZE][BOARD_SIZE]{0};
};

struct Bank {
   int m_id;
   int m_version;
   std::vector<Board> m_boards;
};

void saveBankToFile(Bank const &bank_, std::string const &file_);
Bank readBankFromFile(std::string const &file_);

class Sudoku {
 private:
   Board m_board;
   Board m_current_board;
   int m_checks;
   std::vector<ActionGame> m_actions;

   Board replaceNegatives(Board const &board_);
   void executeSaveActions(std::vector<ActionGame> actions_);

   bool checkCurrentLinear() const;
   bool checkCurrentBlocks() const;
   bool checkCurrent() const;

   /// Verifique apenas com um valor: linha, coluna e bloco
   bool checkUniqueValue(int x_, int y_) const;

   void drawWithColors(short correct_color_, short wrong_color_, short special_color_ = ext::cfg::none) const;

 public:
   Sudoku() {}
   Sudoku(Board const &original_board_, int checks_,
          std::vector<ActionGame> actions_ = {});

   int getBoardId() const;
   int getChecks() const;
   std::vector<ActionGame> getActions() const;
   size_t getActionsCount() const;
   std::array<int, BOARD_SIZE> getDigitsLeft() const;

   std::pair<bool, std::string> insert(int value_, int x_, int y_);

   std::pair<bool, std::string> remove(int x_, int y_);

   /// Undo
   std::pair<bool, std::string> undo();

   /// Check
   std::pair<bool, std::string> check();

   /// Draw
   void draw() const;

   /// Draw
   void drawCheck() const;

   /// Draw
   void drawOnlyWrong() const;

   /// Check full
   bool checkFull() const;
   /// Check winner
   bool checkWinner() const;
};

#endif /// SUDOKU_HPP_