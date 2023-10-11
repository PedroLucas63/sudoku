#include "SudokuGame.hpp"

SudokuGame::SudokuGame(short game_[SIZE*SIZE][SIZE*SIZE], short max_checks_) {
   m_checks_count = 0;
   m_max_checks = max_checks_;

   for (short row { 0 }; row != SIZE*SIZE; ++row) {
      for (short col { 0 }; col != SIZE*SIZE; ++col) {
         m_game[row][col] = game_[row][col];
         if (game_[row][col] < 0) {
            m_process_game[row][col] = 0;
         } else {
            m_process_game[row][col] = game_[row][col];
         }
      }
   }
}

stack<Action> SudokuGame::getActions() const {
   return m_actions;
}

short SudokuGame::getChecks() const {
   return m_checks_count;
}

short SudokuGame::getMaximumChecks() const {
   return m_max_checks;
}


short SudokuGame::setActions(stack<Action> actions_) {
   m_actions = actions_;
}

void SudokuGame::place(short row, short col, short value) {
   try {
      withinTheRange(row, col, value);

      if (!blockedElement(row, col)) {
         Action act{Action::Place, row, col, value};
         m_actions.push(act);
         m_process_game[row][col] = value;
      } else {
         throw std::logic_error("It is not possible to change an element considered standard.");
      }
   } catch (std::out_of_range& e) {
      throw e;
   }
}

void SudokuGame::remove(short row, short col) {
   try {
      withinTheRange(row, col);

      if (!blockedElement(row, col)) {
         Action act{Action::Remove, row, col};
         m_actions.push(act);
      } else {
         throw std::logic_error("It is not possible to change an element considered standard.");
      }
   } catch (std::out_of_range& e) {
      throw e;
   }
}

void SudokuGame::undo() {
   if (m_actions.size() > 0) {
      m_actions.pop();
   } else {
      throw std::logic_error("There are no moves to be canceled.");
   }
}

void SudokuGame::check() {
   if (m_checks_count < m_max_checks) {
      ++m_checks_count;
   } else {
      throw std::logic_error("The number of checks has already been exceeded.");
   }
}

void SudokuGame::draw() const {

}

bool SudokuGame::winner() const {
   for (short row { 0 }; row < SIZE*SIZE; ++row) {
      for (short col { 0 }; col < SIZE*SIZE; ++col) {
         if (std::abs(m_game[row][col]) != m_process_game[row][col]) {
            return false;
         }
      }
   }

   return true;
}

void SudokuGame::withinTheRange(short row, short col) {
   short minimum_value { 1 };

   if (row > SIZE*SIZE || row < minimum_value) {
      throw std::out_of_range("The row is outside the accepted range.");
   }

   if (col > SIZE*SIZE || col < minimum_value) {
      throw std::out_of_range("The col is outside the accepted range.");
   }
}

void SudokuGame::withinTheRange(short row, short col, short value) {
   short minimum_value { 1 };

   withinTheRange(row, col);

   if (value > SIZE*SIZE || value < minimum_value) {
      throw std::out_of_range("The value is outside the accepted range.");
   }
}

bool SudokuGame::blockedElement(short row, short col) {
   if (m_game[row][col] < 0) {
      return false;
   }

   return true;
}
