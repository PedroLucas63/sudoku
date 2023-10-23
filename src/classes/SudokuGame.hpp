#ifndef SUDOKU_GAME_HPP_
#define SUDOKU_GAME_HPP_

#include "SaveGame.hpp"
#include "SudokuBoard.hpp"
#include "Validate.hpp"
#include <string>

class SudokuGame {
 private:
   SudokuBoard m_original_game;
   SudokuBoard m_current_game;
   SaveGame m_save;

 public:
   SudokuGame(SudokuBoard const &game_, int id_bank_, int checks_)
       : m_original_game{game_}, m_current_game{game_}, m_save{id_bank_,
                                                               game_.m_id,
                                                               checks_} {}

   std::pair<bool, std::string> insert(int val_, int x_, int y_) {
      if (val_ <= 0 || val_ > BOARD_SIZE) {
         return {false, "The value passed for insertion is outside the range."};
      } else if (x_ <= 0 || x_ > BOARD_SIZE) {
         return {false, "The passed X coordinate is outside the board limits."};
      } else if (y_ <= 0 || y_ > BOARD_SIZE) {
         return {false, "The passed Y coordinate is outside the board limits."};
      } else if (m_original_game.m_board[y_][x_] > 0) {
         return {false, "The passed coordinate cannot be changed."};
      }

      m_current_game.m_board[y_][x_] = val_;

      ActionGame act;
      act.m_x = x_;
      act.m_y = y_;
      act.m_value = val_;
      act.m_action = Insert;
      m_save.pushAction(act);

      /// TODO: Verificar com os números exibidos
      /// m_current_game ->  Colocar 0 no lugar dos negativos...
      /// Função do validate adaptado para apenas informar se tiver 2 números de
      /// mesmo grau alinhados...
      if (!checkBoard(m_current_game)) {
         return {true, "The move is valid, but it would be better if you paid "
                       "more attention."};
      }

      return {true, ""};
   }

   std::pair<bool, std::string> remove(int x_, int y_) {

      if (x_ <= 0 || x_ > BOARD_SIZE) {
         return {false, "The passed X coordinate is outside the board limits."};
      } else if (y_ <= 0 || y_ > BOARD_SIZE) {
         return {false, "The passed Y coordinate is outside the board limits."};
      } else if (m_original_game.m_board[y_][x_] > 0) {
         return {false, "The passed coordinate cannot be changed."};
      } else if (m_current_game.m_board[y_][x_] == 0) {
         return {false, "The passed coordinate has no value."};
      }

      m_current_game.m_board[y_][x_] = 0;

      ActionGame act;
      act.m_x = x_;
      act.m_y = y_;
      act.m_action = Remove;
      m_save.pushAction(act);

      return {true, ""};
   }

   std::pair<bool, std::string> undo() {
      if (m_save.actionsEmpty()) {
         return {false, "There are no moves to be undone."};
      }

      /// Não é necessário o boolean...
      m_save.popAction();

      /// Reconstruir Board com base nas ações restantes...

      return {true, ""};
   }

   std::pair<bool, std::string> check() {
      if (m_save.getChecks() == 0) {
         return {false, "The number of returns has already been exhausted."};
      }

      /// Habilita o Draw with Check...

      return {true, ""};
   }

   void draw() {
    /// Desenha a tabela...
    /// Se Draw With Check estiver ativado então é mostrado se os valores estão certos ou errados...
    /// No fim sempre determina Draw With Check como desativado.
   }
};

#endif /// SUDOKU_GAME_HPP_