#include "Sudoku.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>

void saveBankToFile(Bank const &bank_, std::string const &file_) {
   std::ofstream file{file_, std::ios::out | std::ios::binary};

   int id{bank_.m_id};
   int version{bank_.m_version};
   int quant_of_boards{static_cast<int>(bank_.m_boards.size())};

   file.write(reinterpret_cast<char *>(&id), sizeof(id));
   file.write(reinterpret_cast<char *>(&version), sizeof(version));
   file.write(reinterpret_cast<char *>(&quant_of_boards),
              sizeof(quant_of_boards));

   for (Board board : bank_.m_boards) {
      file.write(reinterpret_cast<char *>(&board), sizeof(board));
   }

   file.close();
}

Bank readBankFromFile(std::string const &file_) {
   std::ifstream file{file_, std::ios::in | std::ios::binary};

   Bank bank;

   int quant_of_boards{0};

   file.read(reinterpret_cast<char *>(&bank.m_id), sizeof(bank.m_id));
   file.read(reinterpret_cast<char *>(&bank.m_version), sizeof(bank.m_version));
   file.read(reinterpret_cast<char *>(&quant_of_boards),
             sizeof(quant_of_boards));

   for (int bdr{0}; bdr != quant_of_boards; ++bdr) {
      Board board;
      file.read(reinterpret_cast<char *>(&board), sizeof(board));
      bank.m_boards.push_back(board);
   }

   file.close();

   return bank;
}

Sudoku::Sudoku(Board const &original_board_, int checks_,
               std::vector<ActionGame> actions_) {
   m_board = original_board_;
   m_current_board = replaceNegatives(original_board_);
   m_checks = checks_;

   m_actions.reserve(actions_.size());
   std::copy(
       actions_.begin(), actions_.end(),
       m_actions.begin()); /// Executar as ações de entrada no current_board

   executeSaveActions(actions_);
}

Board Sudoku::replaceNegatives(Board const &board_) {
   Board temp_board = board_;

   for (int y{0}; y != BOARD_SIZE; ++y) {
      for (int x{0}; x != BOARD_SIZE; ++x) {
         if (temp_board.m_values[y][x] < 0) {
            temp_board.m_values[y][x] = 0;
         }
      }
   }

   return temp_board;
}

void Sudoku::executeSaveActions(std::vector<ActionGame> actions_) {
   for (auto act : actions_) {
      switch (act.m_action) {
      case Insert:
         insert(act.m_value, act.m_x, act.m_y);
         break;
      case Remove:
         remove(act.m_x, act.m_y);
      default:
         break;
      }
   }
}

bool Sudoku::checkCurrentLinear() const {
   for (int y{0}; y != BOARD_SIZE; ++y) {
      int values_lin[BOARD_SIZE + 1]{0};
      int values_col[BOARD_SIZE + 1]{0};

      for (int x{0}; x != BOARD_SIZE; ++x) {
         int value_lin{m_current_board.m_values[y][x]};
         int value_col{m_current_board.m_values[x][y]};

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

bool Sudoku::checkCurrentBlocks() const {
   int sqrt{static_cast<int>(std::sqrt(BOARD_SIZE))};
   for (int y{0}; y != sqrt; ++y) {
      for (int x{0}; x != sqrt; ++x) {
         int values[BOARD_SIZE + 1]{0};
         int block_y{y * sqrt};
         int block_x{x * sqrt};
         int initial_block_x{block_x};

         int last_y{block_y + sqrt};
         int last_x{block_x + sqrt};

         while (block_y != last_y) {
            int value{m_current_board.m_values[block_y][block_x]};

            if (value < 0 || value > BOARD_SIZE) {
               return false;
            } else {
               ++values[value];
            }

            if (block_x + 1 == last_x) {
               block_x = initial_block_x;
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

bool Sudoku::checkCurrent() const {
   return checkCurrentLinear() && checkCurrentBlocks();
}

bool Sudoku::checkUniqueValue(int x_, int y_) const {
   int value{m_current_board.m_values[y_ - 1][x_ - 1]};

   /// Verifica o eixo x
   for (int x{0}; x != BOARD_SIZE; ++x) {
      int test_value{m_current_board.m_values[y_ - 1][x]};

      if (test_value == value && x + 1 != x_) {
         return false;
      }
   }

   /// Verifica o eixo y
   for (int y{0}; y != BOARD_SIZE; ++y) {
      int test_value{m_current_board.m_values[y][x_ - 1]};

      if (test_value == value && y + 1 != y_) {
         return false;
      }
   }

   /// Verifica bloco:
   int sqrt{static_cast<int>(std::sqrt(BOARD_SIZE))};
   int initial_y{(y_ - 1) / sqrt * sqrt};
   int initial_x{(x_ - 1) / sqrt * sqrt};

   for (int y{initial_y}; y != initial_y + sqrt; ++y) {
      for (int x{initial_x}; x != initial_x + sqrt; ++x) {
         int test_value{m_current_board.m_values[y][x]};

         if (test_value == value && (y + 1 != y_ || x + 1 != x_)) {
            return false;
         }
      }
   }

   return true;
}

void Sudoku::drawWithColors(short correct_color_, short wrong_color_, short special_color_) const {
   /// Define algumas variaveis
   std::string board_table{"    +-------+-------+-------+"};
   size_t max_size_in_line{board_table.size()};
   int width_empty{7};
   int sqrt_board{static_cast<int>(std::sqrt(BOARD_SIZE))};

   ext::fstring first_line{max_size_in_line, ' '};
   if (!m_actions.empty()) {
      int x{m_actions.back().m_x};

      int reference_location{width_empty +
                             2 * (((x - 1) / sqrt_board) + (x - 1)) - 1};

      first_line[reference_location] = 'v';
      first_line.color(ext::cfg::bright_yellow);
      first_line.style(ext::stl::bold);
   }

   std::cout << first_line << "\n";

   // Imprime a segunda linha com os valores
   ext::fstring second_line{max_size_in_line, ' '};

   for (int reference{1}; reference != BOARD_SIZE + 1; ++reference) {
      int reference_location{
          width_empty + 2 * (((reference - 1) / sqrt_board) + (reference - 1)) -
          1};
      second_line[reference_location] = reference + '0';
   }

   second_line.color(ext::cfg::green);
   std::cout << second_line << "\n";

   /// Imprime a primeira borda:
   std::cout << board_table << "\n";

   for (int line{0}; line != BOARD_SIZE; ++line) {
      if (line != 0 && line % sqrt_board == 0) {
         std::cout << "    |-------+-------+-------|\n";
      }

      /// Verifica se tem jogada e se a jogada atual é essa linha:
      if (!m_actions.empty()) {
         int y{m_actions.back().m_y};

         if (y == line + 1) {
            ext::fstring reference{">"};
            reference.color(ext::cfg::bright_yellow);
            reference.style(ext::stl::bold);
            std::cout << reference;
         } else {
            std::cout << " ";
         }
      } else { /// Se não for, imprime um espaço em branco:
         std::cout << " ";
      }

      /// Imprime a letra correspondente:
      ext::fstring character{" "};
      character.push_back(line + 'A');
      character.color(ext::cfg::green);
      std::cout << character << " ";

      for (int number{0}; number != BOARD_SIZE; ++number) {
         if (number % sqrt_board == 0) {
            std::cout << "| ";
         }

         int current_value{m_current_board.m_values[line][number]};
         int original_value{m_board.m_values[line][number]};

         if (current_value == 0) {
            std::cout << "  ";
         } else if (current_value == original_value) {
            ext::fstring original_str {std::to_string(current_value)};
            original_str.style(ext::stl::bold);
            std::cout << original_str << " ";
         } else if (!m_actions.empty() && !checkUniqueValue(number + 1, line + 1) && special_color_ != ext::cfg::none) {
            ext::fstring special_val{std::to_string(current_value)};
            special_val.color(special_color_);
            std::cout << special_val << " ";
         } else if (current_value == -original_value) {
            ext::fstring correct_val{std::to_string(current_value)};
            correct_val.color(correct_color_);
            std::cout << correct_val << " ";
         } else {
            ext::fstring incorrect_val{std::to_string(current_value)};
            incorrect_val.color(wrong_color_);
            std::cout << incorrect_val << " ";
         }
      }

      std::cout << "|\n";
   }

   /// Imprime a segunda borda (ultima):
   std::cout << board_table << "\n";
}

int Sudoku::getBoardId() const { return m_board.m_id; }

int Sudoku::getChecks() const { return m_checks; }

std::vector<ActionGame> Sudoku::getActions() const { return m_actions; }

size_t Sudoku::getActionsCount() const {return m_actions.size(); }

std::array<int, BOARD_SIZE> Sudoku::getDigitsLeft() const {
   std::array<int, BOARD_SIZE> digits;
   digits.fill(BOARD_SIZE);

   for (int y{0}; y != BOARD_SIZE; ++y) {
      for (int x{0}; x != BOARD_SIZE; ++x) {
         int value{m_current_board.m_values[y][x]};
         if (value != 0) {
            --digits[value - 1];
         }
      }
   }

   return digits;
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

   ActionGame last_act{m_actions.empty() ? ActionGame() : m_actions.back()};

   m_actions.push_back(act);

   if (!checkUniqueValue(x_, y_)) {
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

   auto actions{m_actions};
   m_actions.clear();
   actions.pop_back();
   m_current_board = replaceNegatives(m_board);
   executeSaveActions(actions);

   return {true, ""};
}

std::pair<bool, std::string> Sudoku::check() {
   if (m_checks == 0) {
      return {false, "The number of checks has already been exhausted."};
   }

   --m_checks;

   return {true, ""};
}

void Sudoku::draw() const {
   drawWithColors(ext::cfg::bright_blue, ext::cfg::bright_blue, ext::cfg::bright_red);
}

void Sudoku::drawCheck() const {
   drawWithColors(ext::cfg::bright_green, ext::cfg::bright_red);
}

void Sudoku::drawOnlyWrong() const {
   drawWithColors(ext::cfg::bright_blue, ext::cfg::bright_red);
}

/// Check full
bool Sudoku::checkFull() const {
   for (int y{0}; y != BOARD_SIZE; ++y) {
      for (int x{0}; x != BOARD_SIZE; ++x) {
         int value{m_current_board.m_values[y][x]};
         if (value == 0) {
            return false;
         }
      }
   }

   return true;
}
/// Check winner
bool Sudoku::checkWinner() const {
   for (int y{0}; y != BOARD_SIZE; ++y) {
      for (int x{0}; x != BOARD_SIZE; ++x) {
         int value{m_current_board.m_values[y][x]};
         int correct_value{std::abs(m_board.m_values[y][x])};

         if (value != correct_value) {
            return false;
         }
      }
   }

   return true;
}