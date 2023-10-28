/**
 * @file Validate.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Declarations for validation functions related to Sudoku and save data
 * structures.
 * @version 1.0
 * @date 2023-10-28
 *
 * This file provides declarations for various validation functions related to
 * Sudoku and save data structures. These functions are responsible for checking
 * the validity of Sudoku boards, banks, and save files.
 *
 * @copyright Copyright (c) 2023
 */

#ifndef VALIDATE_HPP_
#define VALIDATE_HPP_

#include "Save.hpp"
#include "Sudoku.hpp"

/**
 * @brief Check if the rows in the Sudoku board are valid.
 *
 * This function checks if the rows in the Sudoku board are valid, meaning they
 * contain all numbers from 1 to 9 without any repetition.
 *
 * @param board_ The Sudoku board to be checked.
 * @return True if all rows are valid, false otherwise.
 */
bool checkLines(Board const &board_);

/**
 * @brief Check if the columns in the Sudoku board are valid.
 *
 * This function checks if the columns in the Sudoku board are valid, meaning
 * they contain all numbers from 1 to 9 without any repetition.
 *
 * @param board_ The Sudoku board to be checked.
 * @return True if all columns are valid, false otherwise.
 */
bool checkColumns(Board const &board_);

/**
 * @brief Check if the Sudoku board is valid.
 *
 * This function checks if the Sudoku board is valid by ensuring that both rows
 * and columns contain all numbers from 1 to 9 without any repetition.
 *
 * @param board_ The Sudoku board to be checked.
 * @return True if the Sudoku board is valid, false otherwise.
 */
bool checkBoard(Board const &board_);

/**
 * @brief Validate the structure of a Bank.
 *
 * This function checks if the structure of a Bank object is valid, ensuring it
 * contains the required elements and each Sudoku board within it is also valid.
 *
 * @param bank_ The Bank object to be validated.
 * @return True if the Bank structure is valid, false otherwise.
 */
bool validateBank(Bank const &bank_);

/**
 * @brief Validate the structure of a Save.
 *
 * This function checks if the structure of a Save object is valid, ensuring it
 * contains the required elements.
 *
 * @param save_ The Save object to be validated.
 * @return True if the Save structure is valid, false otherwise.
 */
bool validateSave(Save const &save_);

#endif /// VALIDATE_HPP_
