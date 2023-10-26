#ifndef VALIDATE_HPP_
#define VALIDATE_HPP_

#include "Sudoku.hpp"
#include "Save.hpp"

bool checkLines(Board const &board_);
bool checkColumns(Board const &board_);
bool checkBoard(Board const &board_);
bool validateBank(Bank const& bank_);

bool validateSave(Save const& save_);


#endif /// VALIDATE_HPP_
