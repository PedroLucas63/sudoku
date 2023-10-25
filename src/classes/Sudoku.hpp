#ifndef SUDOKU_HPP_
#define SUDOKU_HPP_

#include <vector>
#include "Actions.hpp"
#include <string>

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

class Sudoku {
private:
   Board m_board;
   Board m_current_board;
   int m_checks;
   std::vector<ActionGame> m_actions;

   Board replaceNegatives(Board const& board_);
   void executeSaveActions(std::initializer_list<ActionGame> actions_);
   void executeSaveActions(std::vector<ActionGame> actions_);

   bool checkCurrentLinear();
   bool checkCurrentBlocks();
   bool checkCurrent();

   /// Verifique apenas com um valor: linha, coluna e bloco

public:
Sudoku(Board const& original_board_, int checks_, std::initializer_list<ActionGame> actions_ = {});

int getBoardId() const;
int getChecks() const;
std::vector<ActionGame> getActions() const;

std::pair<bool, std::string> insert(int value_, int x_, int y_);

std::pair<bool, std::string> remove(int x_, int y_);

/// Undo
std::pair<bool, std::string> undo();

/// Check

/// Draw
void draw() const;
};

#endif /// SUDOKU_HPP_