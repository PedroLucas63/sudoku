#ifndef SUDOKU_GAME_HPP_
#define SUDOKU_GAME_HPP_

#include "Action.hpp"
#include <stack>
using std::stack;
#include <stdexcept>
#include <sstream>

constexpr short SIZE { 3 };
constexpr short DEFAULT_MAX_CHECKS { 3 };

class SudokuGame {
   public:
   SudokuGame() = default;
   SudokuGame(short game_[SIZE*SIZE][SIZE*SIZE], short max_checks_ = DEFAULT_MAX_CHECKS);
   ~SudokuGame() = default;

   stack<Action> getActions() const;
   short getChecks() const;
   short getMaximumChecks() const;

   short setActions(stack<Action> actions_);

   void place(short row, short col, short value);
   void remove(short row, short col);
   void undo();
   void check();
   void draw() const;
   bool winner() const;

   private:
   short m_game[SIZE*SIZE][SIZE*SIZE];
   short m_process_game[SIZE*SIZE][SIZE*SIZE];
   stack<Action> m_actions;
   short m_checks_count;
   short m_max_checks;

   void withinTheRange(short row, short col);
   void withinTheRange(short row, short col, short value);
   bool blockedElement(short row, short col);
};

#endif /// SUDOKU_GAME_HPP_