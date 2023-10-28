/**
 * @file view.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Contains functions for rendering the user interface of the Sudoku
 * game.
 * @version 1.0
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 *
 * @details
 * This header file defines functions responsible for rendering various
 * components of the Sudoku game's user interface. These functions are used to
 * display the game's title, menus, game board, winner screen, and more. The
 * user interface is an essential part of the game's interaction with the
 * player.
 */

#ifndef VIEW_HPP_
#define VIEW_HPP_

#include "Save.hpp"
#include "Sudoku.hpp"
#include <array>
#include <functional>
#include <string>
#include <vector>

/**
 * @brief The width of the game interface.
 *
 * This constant defines the width of the game interface, typically used for
 * rendering purposes. It specifies the number of characters that can be
 * displayed in a single line when rendering various components of the Sudoku
 * game's user interface.
 */
constexpr size_t const WIDTH{60};

/**
 * @brief Render the title of the Sudoku game.
 */
void renderTitle();

/**
 * @brief Render a helper message with the default number of checks.
 *
 * @param default_checks_ The default number of checks available to the user.
 */
void renderHelper(int default_checks_);

/**
 * @brief Render the data selection interface.
 *
 * @param datas_ A vector of strings representing data options.
 */
void renderSelectData(std::vector<std::string> const &datas_);

/**
 * @brief Render the main menu of the game.
 *
 * @param draw_function_ A function to draw the current game state.
 * @param save_ Indicates whether the game can be saved.
 * @param warning_ A warning message to display (if any).
 */
void renderMenu(const std::function<void()> &draw_function_, bool save_,
                std::string warning_);

/**
 * @brief Render the game interface.
 *
 * @param draw_function_ A function to draw the current game state.
 * @param checks_left_ The number of checks remaining.
 * @param digits_left_ An array representing the remaining digits for each
 * number.
 * @param message_ A message to display (e.g., for user instructions).
 */
void renderPlay(const std::function<void()> &draw_function_, int checks_left_,
                std::array<int, BOARD_SIZE> digits_left_, std::string message_);

/**
 * @brief Render the winner screen.
 *
 * @param draw_function_ A function to draw the winning game state.
 * @param checks_left_ The number of checks remaining.
 * @param digits_left_ An array representing the remaining digits for each
 * number.
 * @param winner_ Indicates if the player has won.
 */
void renderWinner(const std::function<void()> &draw_function_, int checks_left_,
                  std::array<int, BOARD_SIZE> digits_left_, bool winner_);

/**
 * @brief Render the new game selection menu.
 *
 * @param save_ Indicates whether the game can be saved.
 */
void renderNewGame(bool save_);

/**
 * @brief Render the load/save interface.
 *
 * @param saves_ A vector of pairs containing save file names and their
 * corresponding Save data.
 */
void renderLoadSave(std::vector<std::pair<std::string, Save>> const &saves_);

/**
 * @brief Render a request to save the current game.
 */
void renderRequestSave();

/**
 * @brief Render a confirmation message for overwriting a saved game.
 */
void renderOverwriteConfirm();

/**
 * @brief Render information about the game.
 */
void renderAbout();

/**
 * @brief Render a message when the player is quitting the game.
 *
 * @param save_ Indicates whether the game can be saved before quitting.
 */
void renderQuitting(bool save_);

/**
 * @brief Render an ending message, possibly with a warning.
 *
 * @param warning_ A warning message to display (if any).
 */
void renderEnding(std::string warning_);

#endif /// VIEW_HPP_
