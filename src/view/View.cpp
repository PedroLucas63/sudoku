/**
 * @file View.cpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Implementation of the View class functions for rendering the game's
 * user interface.
 * @version 1.0
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "SudokuConfig.h"
#include "View.hpp"
#include "format/fstring.hpp"
#include <iostream>
#include <sstream>

/// @brief Renders the title of the Sudoku game on the console.
void renderTitle() {
   // Create a horizontal line of '=' characters.
   ext::fstring bars{WIDTH, '='};
   bars.color(ext::cfg::blue); // Set the text color to blue.
   bars.style(ext::stl::bold); // Apply bold style to the text.

   // Construct the title string with version information.
   std::ostringstream oss;
   oss << "Welcome to a terminal version of Sudoku, v" << sudoku_VERSION_MAJOR
       << "." << sudoku_VERSION_MINOR << "";
   ext::fstring title{oss.str()};
   title.align_center(WIDTH);
   title.color(ext::cfg::blue);
   title.style(ext::stl::bold);

   // Create the copyright notice.
   ext::fstring copyright{"Copyright (C) 2023, Pedro Lucas M. Nascimento"};
   copyright.align_center(WIDTH);
   copyright.color(ext::cfg::blue);
   copyright.style(ext::stl::bold);

   // Output the title and copyright on the console.
   std::cout << bars << "\n";
   std::cout << title << "\n";
   std::cout << copyright << "\n";
   std::cout << bars << "\n";
}

/// @brief Renders the helper information on the console.
/// @param default_checks_ The default number of checks per game.
void renderHelper(int default_checks_) {
   renderTitle(); // Render the title first.

   // Output usage information and game options.
   std::cout << "Usage: sudoku [-c <num>] [-h] <input_puzzle_file>\n";
   std::cout << "  Game options:\n";
   std::cout << "    -c <num> Number of checks per game. Default = "
             << default_checks_ << ".\n";
   std::cout << "    -h       Print this help text.\n";
}

/// @brief Renders the data selection menu on the console.
/// @param datas_ A vector of strings containing the available data options.
void renderSelectData(std::vector<std::string> const &datas_) {
   std::cout << "\n";
   renderTitle(); // Render the title.
   std::cout << "\n";

   // Create an information message for data selection.
   ext::fstring info{"Select one of the options below."};
   info.color(ext::cfg::green);
   info.style(ext::stl::italic);

   std::cout << info << "\n"; // Output the information message.

   // Output the available data options.
   for (size_t index{0}; index != datas_.size(); ++index) {
      std::cout << index << ". " << datas_[index] << "\n";
   }

   // Create a message to prompt user input.
   ext::fstring select{"Enter one of the listed game banks >> "};
   select.color(ext::cfg::blue);
   select.style(ext::stl::italic);

   std::cout << "\n" << select; // Output the prompt for user input.
}

/// @brief Renders the main menu on the console.
/// @param draw_function_ A function to draw the game screen or status.
/// @param save_ A boolean indicating whether to display "Save" or "Load" in the
/// menu.
/// @param warning_ A warning message to display, if any.
void renderMenu(const std::function<void()> &draw_function_, bool save_,
                std::string warning_) {
   std::cout << "\n";
   renderTitle(); // Render the title.
   std::cout << "\n";

   draw_function_(); // Call the draw function to display game screen or status.
   std::cout << "\n";

   if (!warning_.empty()) {
      ext::fstring message{warning_};
      message.color(ext::cfg::red);

      std::cout << message << "\n\n"; // Display a warning message, if provided.
   }

   // Create and output the main menu options.
   ext::fstring options{"1. Play  2. New Game  3. "};
   options +=
       save_ ? "Save"
             : "Load"; // Display "Save" or "Load" based on the save_ parameter.
   options += "  4. About  5. Quit";

   options.color(ext::cfg::blue);
   options.style(ext::stl::bold);

   ext::fstring request{"Select option [1,5] >> "};
   request.color(ext::cfg::blue);

   std::cout << options << "\n"; // Output the menu options.
   std::cout << request;         // Output the user prompt for menu selection.
}

/// @brief Renders the play menu on the console during gameplay.
/// @param draw_function_ A function to draw the game screen or status.
/// @param checks_left_ The number of checks left.
/// @param digits_left_ An array representing the remaining digits for each
/// number.
/// @param message_ A message to display to the player.
void renderPlay(const std::function<void()> &draw_function_, int checks_left_,
                std::array<int, BOARD_SIZE> digits_left_,
                std::string message_) {
   std::cout << "\n";
   renderTitle(); // Render the title.
   std::cout << "\n";

   draw_function_(); // Call the draw function to display the game screen or
                     // status.

   // Create and output the "Checks left" information.
   ext::fstring checks{"Checks left: [ " + std::to_string(checks_left_) + " ]"};
   checks.color(ext::cfg::yellow);
   checks.style(ext::stl::italic);

   std::cout << checks << "\n";

   // Create and output the "Digits left" information.
   ext::fstring digits{"Digits left: [ "};

   for (int index{0}; index != BOARD_SIZE; ++index) {
      if (digits_left_[index] <= 0) {
         digits += "  "; // Hide digits with count 0.
      } else {
         digits += std::to_string(index + 1) + " ";
      }
   }

   digits += "]";
   digits.color(ext::cfg::yellow);
   digits.style(ext::stl::italic);

   std::cout << digits << "\n";

   // Create and output the player message.
   ext::fstring message_left{"MSG: ["};
   message_left.color(ext::cfg::yellow);
   message_left.style(ext::stl::italic);

   ext::fstring message{message_};
   message.background(ext::cbg::yellow);
   message.style(ext::stl::italic);

   ext::fstring message_right{"]"};
   message_right.color(ext::cfg::yellow);
   message_right.style(ext::stl::italic);

   std::cout << message_left << message << message_right << "\n\n";

   // Output the commands and syntax information.
   ext::fstring commands{
       "Commands syntax:\n"
       "  'enter' (without typing anything)  -> go back to the previous menu.\n"
       "  'p' <row> <col> <number> + 'enter' -> place <number> on the board at "
       "location (<row>, <col>).\n"
       "  'r' <row> <col> + 'enter'          -> remove the number on the board "
       "at "
       "location (<row>, <col>).\n"
       "  'c' 'enter'                        -> check which moves are "
       "correct.\n"
       "  'u' + 'enter'                      -> undo the last play.\n"
       "  <col> and <number> must be in the range [1,9].\n"
       "  <row> must be in the range [A,I].\n"};

   commands.color(ext::cfg::green);
   commands.style(ext::stl::bold);

   std::cout << commands << "\n";

   // Create and output the prompt for user action.
   ext::fstring enter_command{"Enter the action >> "};
   enter_command.color(ext::cfg::blue);
   enter_command.style(ext::stl::italic);

   std::cout << enter_command;
}

/// @brief Renders the winner or loser screen on the console.
/// @param draw_function_ A function to draw the game screen or status.
/// @param checks_left_ The number of checks left.
/// @param digits_left_ An array representing the remaining digits for each
/// number.
/// @param winner_ A boolean indicating whether the player is a winner (true) or
/// loser (false).
void renderWinner(const std::function<void()> &draw_function_, int checks_left_,
                  std::array<int, BOARD_SIZE> digits_left_, bool winner_) {
   std::cout << "\n";
   renderTitle(); // Render the title.
   std::cout << "\n";

   draw_function_(); // Call the draw function to display the game screen or
                     // status.

   // Create and output the "Checks left" information.
   ext::fstring checks{"Checks left: [ " + std::to_string(checks_left_) + " ]"};
   checks.color(ext::cfg::yellow);
   checks.style(ext::stl::italic);

   std::cout << checks << "\n";

   // Create and output the "Digits left" information.
   ext::fstring digits{"Digits left: [ "};

   for (int index{0}; index != BOARD_SIZE; ++index) {
      if (digits_left_[index] <= 0) {
         digits += "  ";
      } else {
         digits += std::to_string(index + 1) + " ";
      }
   }

   digits += "]";
   digits.color(ext::cfg::yellow);
   digits.style(ext::stl::italic);

   std::cout << digits << "\n";

   // Create and output the player message.
   ext::fstring message_left{"MSG: ["};
   message_left.color(ext::cfg::yellow);
   message_left.style(ext::stl::italic);

   ext::fstring message;

   if (winner_) {
      message =
          "Congratulations, you solved the puzzle! Press enter to continue.";
   } else {
      message = "Sorry, you lost! Press enter to continue.";
   }

   message.background(ext::cbg::yellow);
   message.style(ext::stl::italic);

   ext::fstring message_right{"]"};
   message_right.color(ext::cfg::yellow);
   message_right.style(ext::stl::italic);

   std::cout << message_left << message << message_right << "\n\n";
}
/// @brief Renders the prompt to start a new game.
/// @param save_ A boolean indicating whether there is a game in progress (true)
/// or not (false).
void renderNewGame(bool save_) {
   // Create a prompt message asking if the player wants to start a new game.
   ext::fstring ask{"You have a game in progress. Are you sure you want to "
                    "load a new game [y/N]? "};
   ask.color(ext::cfg::blue);
   ask.style(ext::stl::italic);

   // Output the prompt only if there is a game in progress (save_ is true).
   if (save_) {
      std::cout << "\n" << ask;
   }
}

/// @brief Renders the menu to select a game save or enter a custom save.
/// @param saves_ A vector of pairs representing available game saves, where
/// each pair contains a name and save data.
void renderLoadSave(std::vector<std::pair<std::string, Save>> const &saves_) {
   // Output a newline and the title.
   std::cout << "\n";
   renderTitle();
   std::cout << "\n";

   // Create an information message indicating the options.
   ext::fstring info{"Select one of the options below or enter your own save."};
   info.color(ext::cfg::green);
   info.style(ext::stl::italic);

   // Output the information message.
   std::cout << info << "\n";

   // Iterate through the available game saves and display them with an index.
   for (size_t index{0}; index != saves_.size(); ++index) {
      std::cout << index << ". " << saves_[index].first << "\n";
   }

   // Output an option to inform the user about saves.
   std::cout << saves_.size() << ". Inform yourself\n";

   // Create a prompt to enter a selected game save.
   ext::fstring select{"Enter one of the listed game saves >> "};
   select.color(ext::cfg::blue);
   select.style(ext::stl::italic);

   // Output the prompt.
   std::cout << "\n" << select;
}

/// @brief Renders a prompt for the user to enter the name of a save file.
void renderRequestSave() {
   // Create a prompt message for the user to enter the save file name.
   ext::fstring select{"Enter the save file >> "};
   select.color(ext::cfg::blue);
   select.style(ext::stl::italic);

   // Output the prompt to the console.
   std::cout << "\n" << select;
}

/// @brief Renders a confirmation prompt for overwriting an existing file.
void renderOverwriteConfirm() {
   // Create a confirmation prompt for overwriting an existing file.
   ext::fstring select{"This file already exists, are you sure you want to "
                       "overwrite it [y/N]? "};
   select.color(ext::cfg::blue);
   select.style(ext::stl::italic);

   // Output the confirmation prompt to the console.
   std::cout << "\n" << select;
}

/// @brief Renders information about the Sudoku game and its rules.
void renderAbout() {
   // Output a line break, the title, and another line break.
   std::cout << "\n";
   renderTitle();
   std::cout << "\n";

   // Create a line of dashes with a specified width.
   ext::fstring bars{WIDTH, '-'};
   bars.color(ext::cfg::green);
   bars.style(ext::stl::italic);

   // Output the line of dashes.
   std::cout << bars << "\n";

   // Provide information about Sudoku rules and the game's goal.
   ext::fstring about{
       "The goal of Sudoku is to fill a 9x9 grid with numbers so that "
       "each row, column, and 3x3 section (nonet) contains all of the "
       "digits between 1 and 9.\n\nThe Sudoku rules are:\n1. Each row, column, "
       "and nonet can contain each number (typically 1 to 9) exactly once.\n2. "
       "The sum of all numbers in any nonet, row, or column must be equal to "
       "45."};

   // Split the 'about' text into lines that fit within the specified width.
   std::vector<ext::fstring<char>> lines;
   about.split_with_empty(lines, '\n');

   // Iterate through each line, format it, and output to the console.
   for (ext::fstring line : lines) {
      do {
         ext::fstring buffer{line.split_at(WIDTH, ' ')};

         line.color(ext::cfg::green);
         line.style(ext::stl::italic);

         // Output the formatted line.
         std::cout << line << "\n";

         line = buffer;
      } while (!line.empty());
   }

   // Output another line of dashes and an "end" message.
   std::cout << bars << "\n";

   ext::fstring end{"Press <enter> to go back."};
   end.color(ext::cfg::blue);
   end.style(ext::stl::italic);

   std::cout << "\n" << end;
}

/// @brief Renders a confirmation message when quitting the game.
/// @param save_ Indicates whether there is an ongoing game that can be saved.
void renderQuitting(bool save_) {
   ext::fstring ask{
       "You have a game in progress. Are you sure you want to leave [y/N]? "};
   ask.color(ext::cfg::blue);
   ask.style(ext::stl::italic);

   // If save_ is true, the game is in progress, and the confirmation message is
   // displayed.
   if (save_) {
      std::cout << "\n" << ask;
   }
}

/// @brief Renders a warning or error message at the end of a game.
/// @param warning_ The warning or error message to be displayed at the end of a
/// game.
void renderEnding(std::string warning_) {
   // Check if the warning message is empty; if so, return without rendering
   // anything.
   if (warning_.empty()) {
      return;
   }

   // Create an fstring to display the warning message with red text and bold
   // formatting.
   ext::fstring message{warning_};
   message.color(ext::cfg::red);
   message.style(ext::stl::bold);

   // Display the warning message to the user.
   std::cout << message << "\n";
}
