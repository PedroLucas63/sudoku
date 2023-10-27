#include "SudokuConfig.h"
#include "View.hpp"
#include "format/fstring.hpp"
#include <iostream>
#include <sstream>

void renderTitle() {
   ext::fstring bars{WIDTH, '='};
   bars.color(ext::cfg::blue);
   bars.style(ext::stl::bold);

   std::ostringstream oss;
   oss << "Welcome to a terminal version of Sudoku, v" << sudoku_VERSION_MAJOR
       << "." << sudoku_VERSION_MINOR << "";
   ext::fstring title{oss.str()};
   title.align_center(WIDTH);
   title.color(ext::cfg::blue);
   title.style(ext::stl::bold);

   ext::fstring copyright{"Copyright (C) 2023, Pedro Lucas M. Nascimento"};
   copyright.align_center(WIDTH);
   copyright.color(ext::cfg::blue);
   copyright.style(ext::stl::bold);

   std::cout << bars << "\n";
   std::cout << title << "\n";
   std::cout << copyright << "\n";
   std::cout << bars << "\n";
}

void renderHelper(int default_checks_) {
   renderTitle();
   std::cout << "Usage: sudoku [-c <num>] [-h] <input_puzzle_file>\n";
   std::cout << "  Game options:\n";
   std::cout << "    -c <num> Number of checks per game. Default = "
             << default_checks_ << ".\n";
   std::cout << "    -h       Print this help text.\n";
}

void renderSelectData(std::vector<std::string> const& datas_) {
   std::cout << "\n";
   renderTitle();
   std::cout << "\n";

   ext::fstring info {"Select one of the options below."};
   info.color(ext::cfg::green);
   info.style(ext::stl::italic);

   std::cout << info << "\n";

   for (size_t index{0}; index != datas_.size(); ++index) {
      std::cout << index << ". " << datas_[index] << "\n";
   }

   ext::fstring select {"Enter one of the listed game banks >> "};
   select.color(ext::cfg::blue);
   select.style(ext::stl::italic);

   std::cout << "\n" << select;
}

void renderEnding(std::string warning_) {
   if (warning_.empty()) {
      return;
   }

   ext::fstring message{warning_};
   message.color(ext::cfg::red);
   message.style(ext::stl::bold);

   std::cout << message << "\n";
}

void renderMenu(const std::function<void()> &draw_function_, bool save_, std::string warning_) {
   std::cout << "\n";
   renderTitle();
   std::cout << "\n";

   draw_function_();
   std::cout << "\n";

   if (!warning_.empty()) {
      ext::fstring message{warning_};
      message.color(ext::cfg::red);

      std::cout << message << "\n\n";
   }

   ext::fstring options{"1. Play  2. New Game  3. "};

   options += save_ ? "Save" : "Load";

   options += "  4. About  5. Quit";

   options.color(ext::cfg::blue);
   options.style(ext::stl::bold);

   ext::fstring request{"Select option [1,5] >> "};
   request.color(ext::cfg::blue);

   std::cout << options << "\n";
   std::cout << request;
}

void renderNewGame(bool save_) {
      ext::fstring ask{
       "You have a game in progress. Are you sure you want to load a new game [y/N]? "};
   ask.color(ext::cfg::blue);
   ask.style(ext::stl::italic);

   if (save_) {
      std::cout << "\n" << ask;
   }
}

void renderLoadSave(std::vector<std::pair<std::string, Save>> const& saves_) {
   std::cout << "\n";
   renderTitle();
   std::cout << "\n";

   ext::fstring info {"Select one of the options below or enter your own save."};
   info.color(ext::cfg::green);
   info.style(ext::stl::italic);

   std::cout << info << "\n";

   for (size_t index{0}; index != saves_.size(); ++index) {
      std::cout << index << ". " << saves_[index].first << "\n";
   }

   std::cout << saves_.size() << ". Inform yourself\n";

   ext::fstring select {"Enter one of the listed game saves >> "};
   select.color(ext::cfg::blue);
   select.style(ext::stl::italic);
   
   std::cout << "\n" << select;
}

void renderRequestSave() {
   ext::fstring select {"Enter the save file >> "};
   select.color(ext::cfg::blue);
   select.style(ext::stl::italic);
   
   std::cout << "\n" << select;
}

void renderOverwriteConfirm() {
   ext::fstring select {"This file already exists, are you sure you want to overwrite it [y/N]? "};
   select.color(ext::cfg::blue);
   select.style(ext::stl::italic);
   
   std::cout << "\n" << select;
}

void renderAbout() {
   std::cout << "\n";
   renderTitle();
   std::cout << "\n";

   ext::fstring bars{WIDTH, '-'};
   bars.color(ext::cfg::green);
   bars.style(ext::stl::italic);

   std::cout << bars << "\n";

   ext::fstring about{
       "The goal of Sudoku is to fill a 9x9 grid with numbers so that "
       "each row, column and 3x3 section (nonet) contain all of the "
       "digits between 1 and 9.\n\nThe Sudoku rules are:\n1. Each row, column, "
       "and nonet can contain each number (typically 1 to 9) exactly once.\n2. "
       "The sum of all numbers in any nonet, row, or column must be equal to "
       "45."};

   std::vector<ext::fstring<char>> lines;
   about.split_with_empty(lines, '\n');

   for (ext::fstring line : lines) {
      do {
         ext::fstring buffer { line.split_at(WIDTH, ' ') };

         line.color(ext::cfg::green);
         line.style(ext::stl::italic);

         std::cout << line << "\n";

         line = buffer;
      } while (!line.empty());
   }

   std::cout << bars << "\n";

   ext::fstring end {"Press <enter> to go back."};
   end.color(ext::cfg::blue);
   end.style(ext::stl::italic);

   std::cout << "\n" << end;
}

void renderQuitting(bool save_) {
   ext::fstring ask{
       "You have a game in progress. Are you sure you want to leave [y/N]? "};
   ask.color(ext::cfg::blue);
   ask.style(ext::stl::italic);

   if (save_) {
      std::cout << "\n" << ask;
   }
}
