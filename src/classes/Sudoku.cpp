#include "Sudoku.hpp"
#include <algorithm>
#include <math.h>
#include <iostream>
#include "format/fstring.hpp"

Sudoku::Sudoku(Board const& original_board_, int checks_, std::initializer_list<ActionGame> actions_) {
   m_board = original_board_;
   m_current_board = replaceNegatives(original_board_);
   m_checks = checks_;
   std::copy(actions_.begin(), actions_.end(), m_actions.begin()); /// Executar as ações de entrada no current_board
   executeSaveActions(actions_);
}

Board Sudoku::replaceNegatives(Board const& board_) {
   Board temp_board = board_;

   for (int y { 0 }; y != BOARD_SIZE; ++y) {
      for (int x { 0 }; x != BOARD_SIZE; ++x) {
         if (temp_board.m_values[y][x] < 0) {
            temp_board.m_values[y][x] = 0;
         }
      }
   }

   return temp_board;
}

void Sudoku::executeSaveActions(std::initializer_list<ActionGame> actions_) {
   for (auto act : actions_) {
      switch (act.m_action)
      {
      case Insert:
         insert(act.m_value, act.m_x, act.m_y);
         break;
      case Remove:
         remove(act.m_x, act.m_y);
      default:
         break;
      }

      m_actions.pop_back();
   }
}

/// TODO: Tentar usar template 
void Sudoku::executeSaveActions(std::vector<ActionGame> actions_) {
   for (auto act : actions_) {
      switch (act.m_action)
      {
      case Insert:
         insert(act.m_value, act.m_x, act.m_y);
         break;
      case Remove:
         remove(act.m_x, act.m_y);
      default:
         break;
      }

      m_actions.pop_back();
   }
}

bool Sudoku::checkCurrentLinear() {
   for (int y{0}; y != BOARD_SIZE; ++y) {
      int values_lin[BOARD_SIZE + 1]{0};
      int values_col[BOARD_SIZE + 1]{0};

      for (int x{0}; x != BOARD_SIZE; ++x) {
         int value_lin {m_current_board.m_values[y][x]};
         int value_col {m_current_board.m_values[x][y]};

         if (value_lin < 0 || value_lin > BOARD_SIZE) {
            return false;
         } else if (value_col < 0 || value_col > BOARD_SIZE) {
            return false;
         } else {
            ++values_lin[value_lin];
            ++values_col[value_col];
         }
      }

      for (int j{1}; j != BOARD_SIZE + 1; ++j) {
         if (values_lin[j] > 1 || values_col[j] > 1) {
            return false;
         }
      }
   }

   return true;
}

bool Sudoku::checkCurrentBlocks() {
   int sqrt {static_cast<int>(std::sqrt(BOARD_SIZE))};
   for (int y { 0 }; y != sqrt; ++y) {
      for (int x { 0 }; x != sqrt; ++x) {
         int values[BOARD_SIZE + 1] {0};
         int block_y {y * sqrt};
         int block_x {x * sqrt};

         while (block_y != block_y + sqrt) {
            int value {m_current_board.m_values[block_y][block_x]};

            if (value < 0 || value > BOARD_SIZE) {
               return false;
            } else {
               ++values[value];
            }

            if (block_x + 1 == block_y + sqrt) {
               block_x = 0;
               ++block_y;
            } else {
               ++block_x;
            }    
         }

         for (int j{1}; j != BOARD_SIZE + 1; ++j) {
            if (values[j] > 1) {
               return false;
            }
         }
      }
   }

   return true;
}

bool Sudoku::checkCurrent() {
   return checkCurrentLinear() && checkCurrentBlocks();
}

int Sudoku::getBoardId() const {
   return m_board.m_id;
}

int Sudoku::getChecks() const {
   return m_checks;
}

std::vector<ActionGame> Sudoku::getActions() const {
   return m_actions;
}

std::pair<bool, std::string> Sudoku::insert(int value_, int x_, int y_) {
   if (value_ <= 0 || value_ > BOARD_SIZE) {
      return {false, "The value passed for insertion is outside the range."};
   } else if (x_ <= 0 || x_ > BOARD_SIZE) {
      return {false, "The passed X coordinate is outside the board limits."};
   } else if (y_ <= 0 || y_ > BOARD_SIZE) {
      return {false, "The passed Y coordinate is outside the board limits."};
   } else if (m_board.m_values[y_ - 1][x_ - 1] > 0) {
      return {false, "The passed coordinate cannot be changed."};
   } else if (m_current_board.m_values[y_ - 1][x_ - 1] == value_) {
      return {false, "This insertion has already been performed previously."};
   }

   m_current_board.m_values[y_ - 1][x_ - 1] = value_;

   ActionGame act;
   act.m_x = x_;
   act.m_y = y_;
   act.m_value = value_;
   act.m_action = Insert;

   ActionGame last_act { m_actions.empty() ? ActionGame() : m_actions.back() };

   m_actions.push_back(act);
   
   if (!checkCurrent()) {
      return {true, "The move is valid, but it would be better if you paid "
                     "more attention."};
   }

   return {true, ""};
}

std::pair<bool, std::string> Sudoku::remove(int x_, int y_) {
   if (x_ <= 0 || x_ > BOARD_SIZE) {
      return {false, "The passed X coordinate is outside the board limits."};
   } else if (y_ <= 0 || y_ > BOARD_SIZE) {
      return {false, "The passed Y coordinate is outside the board limits."};
   } else if (m_board.m_values[y_ - 1][x_ - 1] > 0) {
      return {false, "The passed coordinate cannot be changed."};
   } else if (m_board.m_values[y_ - 1][x_ - 1] == 0) {
      return {false, "The passed coordinate has no value."};
   }

   m_current_board.m_values[y_ - 1][x_ - 1] = 0;

   ActionGame act;
   act.m_x = x_;
   act.m_y = y_;
   act.m_action = Remove;
   m_actions.push_back(act);

   return {true, ""};
}

std::pair<bool, std::string> Sudoku::undo() {
   if (m_actions.empty()) {
      return {false, "There are no moves to be undone."};
   }

   m_actions.pop_back();
   m_current_board = replaceNegatives(m_board);
   executeSaveActions(m_actions);
   return {true, ""};
}

void Sudoku::draw() const {
   /// Define algumas variaveis
   std::string board_table {"    +-------+-------+-------+"};
   int max_size_in_line { board_table.size() };
   int width_empty { 7 };
   int sqrt_board { static_cast<int>(std::sqrt(BOARD_SIZE)) };

   ext::fstring first_line{max_size_in_line, ' '};
   if (!m_actions.empty()) {
      int x {m_actions.back().m_x};

      int reference_location { width_empty + 2 * (((x - 1) / sqrt_board) + (x - 1)) - 1 };

      first_line[reference_location] = 'v';
      first_line.color(ext::cfg::red);
   }

   std::cout << first_line << "\n";

   // Imprime a segunda linha com os valores
   ext::fstring second_line {max_size_in_line, ' '};

   for (int reference { 1 }; reference != BOARD_SIZE + 1; ++reference) {
      int reference_location { width_empty + 2 * (((reference - 1) / sqrt_board) + (reference - 1)) - 1 };
      second_line[reference_location] = reference + '0';
   }

   second_line.color(ext::cfg::green);
   std::cout << second_line << "\n";
   
   /// Imprime a primeira borda:
   std::cout << board_table << "\n";

   for (int line { 0 }; line != BOARD_SIZE; ++line) {
      if (line != 0 && line % sqrt_board == 0) {
         std::cout <<"    |-------+-------+-------|\n";
      }

      /// Verifica se tem jogada e se a jogada atual é essa linha:
      if (!m_actions.empty()) {
         int y {m_actions.back().m_y};

         if (y == line + 1) {
            ext::fstring reference {">"};
            reference.color(ext::cfg::red);
            std::cout << reference;
         } else {
            std::cout << " ";
         }
      } else { /// Se não for, imprime um espaço em branco:
         std::cout << " ";
      }

      /// Imprime a letra correspondente:
      ext::fstring character { " " };
      character.push_back(line + 'A'); 
      character.color(ext::cfg::green);
      std::cout << character << " ";


      for (int number { 0 }; number != BOARD_SIZE; ++number) {
         if (number % sqrt_board == 0) {
            std::cout << "| ";
         }

         int current_value { m_current_board.m_values[line][number] };
         int original_value { m_board.m_values[line][number] };

         if (current_value == 0) {
            std::cout << "  ";
         } else if (current_value != original_value) {
            ext::fstring str_val {std::to_string(current_value)};
            str_val.color(ext::cfg::blue);
            std::cout << str_val << " ";
         } else {
            std::cout << current_value << " ";
         }

      }

      std::cout << "|\n";
   }

   /// Imprime a segunda borda (ultima):
   std::cout << board_table << "\n";
}
