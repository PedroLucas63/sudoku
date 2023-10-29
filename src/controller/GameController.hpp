/**
 * @file GameController.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Contains the declaration of the GameController class.
 * @version 1.0
 * @date 2023-10-29
 *
 *
 * This file contains the declaration of the GameController class, which
 * controls the logic of the Sudoku game. The GameController class manages game
 * states, player interactions, and game board operations.
 *
 * @copyright Copyright (c) 2023
 */

#ifndef GAME_CONTROLLER_HPP_
#define GAME_CONTROLLER_HPP_

#include "Save.hpp"
#include "Sudoku.hpp"
#include <string>
#include <vector>

/**
 * @brief The minimum number of checks allowed.
 */
constexpr int const MINIMUM_CHECKS{0};

/**
 * @brief The default number of checks.
 */
constexpr int const DEFAULT_CHECKS{3};

/**
 * @brief Default data folder.
 */
std::string const DATA_FOLDER{"./data"};

/**
 * @brief Data file extension.
 */
std::string const DATA_EXTENSION{".sdk"};

/**
 * @brief Default save folder.
 */
std::string const SAVE_FOLDER{"./save"};

/**
 * @brief Save file extension.
 */
std::string const SAVE_EXTENSION{".sav"};

/**
 * @brief Default confirmation character.
 */
constexpr char const CONFIRM_KEY{'y'};

/**
 * @class GameController
 * @brief Controls the logic of the Sudoku game.
 */
class GameController {
 private:
   /**
    * @enum States
    * @brief Possible game states.
    */
   enum States {
      Start,         ///< The game is in the starting state.
      Welcome,       ///< The game is in the welcome state.
      Helper,        ///< The game is in the helper state.
      SearchDatas,   ///< The game is searching for data.
      SelectData,    ///< The game is in the process of selecting data.
      ReadData,      ///< The game is reading data.
      SearchSaves,   ///< The game is searching for save files.
      ReadSaves,     ///< The game is reading save files.
      LoadGame,      ///< The game is loading a saved game.
      Menu,          ///< The game is in the menu state.
      Playing,       ///< The player is actively playing the game.
      CheckWinner,   ///< The game is checking for a winner.
      NewGame,       ///< The game is starting a new game.
      SaveGame,      ///< The game is in the process of saving the game.
      OverwriteSave, ///< The game is confirming an overwrite action for save
                     ///< files.
      SelectSave,    ///< The game is selecting a save file.
      ReadUserSave,  ///< The game is reading user save data.
      About,         ///< The game is in the about state.
      Quitting,      ///< The game is in the process of quitting.
      Ending,        ///< The game is in the ending state.
   };

   /**
    * @enum MenuOption
    * @brief Available menu options.
    */
   enum MenuOption {
      MenuPlay,     ///< Play option in the menu.
      MenuNewGame,  ///< New Game option in the menu.
      MenuLoadGame, ///< Load Game option in the menu.
      MenuSaveGame, ///< Save Game option in the menu.
      MenuAbout,    ///< About option in the menu.
      MenuQuit,     ///< Quit option in the menu.
      MenuNone,     ///< No menu option selected.
   };

   static GameController *m_instance; ///< Pointer to the unique instance of the
                                      ///< GameController class.

   States m_state;           ///< The current state of the game.
   MenuOption m_menu_option; ///< The selected menu option.

   bool m_help;     ///< Flag indicating if the help is enabled.
   bool m_new_game; ///< Flag indicating if a new game is in progress.
   bool m_saving; ///< Flag indicating if the game is in the process of saving.
   bool m_quitting;    ///< Flag indicating if the game is in the process of
                       ///< quitting.
   bool m_return_menu; ///< Flag indicating if the game is returning to the main
                       ///< menu.
   bool m_print_check; ///< Flag indicating if checks should be printed.

   std::vector<std::string> m_input_datas; ///< A list of input data.
   std::vector<std::string> m_input_saves; ///< A list of input save data.
   std::string m_saving_str;               ///< A string used for saving data.
   std::string m_menu_warning; ///< Warning message displayed in the menu.
   std::string
       m_ending_warning; ///< Warning message displayed at the end of the game.

   int m_select_data; ///< The selected data index.
   int m_select_game; ///< The selected game index.
   int m_select_save; ///< The selected save index.

   Bank m_bank;           ///< An instance of the Bank class.
   int m_game_checks;     ///< The number of game checks performed.
   Sudoku m_current_game; ///< The current game state.
   std::vector<std::pair<std::string, Save>> m_saves; ///< A list of save data.
   std::string m_play_message; ///< Message displayed during gameplay.

   /**
    * @brief Private copy constructor, preventing copying of GameController
    * instances.
    */
   GameController(GameController const &other_) = delete;

   /**
    * @brief Private copy assignment operator, preventing assignment of
    * GameController instances.
    */
   GameController &operator=(GameController const &other_) = delete;

   /*******************************************************************
    * @brief Process functions
    *****************************************************************/
   /**
    * @brief Search for available data.
    */
   void searchDatas();

   /**
    * @brief Select data for the game.
    */
   void selectData();

   /**
    * @brief Read selected data.
    */
   void readData();

   /**
    * @brief Search for available save data.
    */
   void searchSaves();

   /**
    * @brief Read available save data.
    */
   void readSaves();

   /**
    * @brief Load a saved game.
    */
   void loadGame();

   /**
    * @brief Get and process the user's menu option.
    */
   void getOptionMenu();

   /**
    * @brief Determine whether to save or load a game.
    */
   void defineSaveOrLoad();

   /**
    * @brief Get and process the user's gameplay option.
    */
   void getOptionPlay();

   /**
    * @brief Insert numbers into the game board based on user input.
    * @param commands_ A vector of strings containing the user's commands.
    */
   void insertNumber(std::vector<std::string> commands_);

   /**
    * @brief Remove numbers from the game board based on user input.
    * @param commands_ A vector of strings containing the user's commands.
    */
   void removeNumber(std::vector<std::string> commands_);

   /**
    * @brief Check the current state of the game board.
    */
   void checkBoard();

   /**
    * @brief Undo the last move on the game board.
    */
   void undoBoard();

   /**
    * @brief Start a new game.
    */
   void newGame();

   /**
    * @brief Generate a new game board.
    */
   void generateNewGame();

   /**
    * @brief Save the current game state.
    */
   void saveGame();

   /**
    * @brief Confirm the overwrite of an existing save file.
    */
   void confirmOverwrite();

   /**
    * @brief Save the game state to a file.
    */
   void saveGameInFile();

   /**
    * @brief Select a save file.
    */
   void selectSave();

   /**
    * @brief Enter local save data.
    */
   void enterLocalSave();

   /**
    * @brief Process the user's "press enter" action.
    */
   void pressEnter();

   /**
    * @brief Confirm quitting the game.
    */
   void confirmQuitting();

   /*******************************************************************
    * @brief Update functions
    *****************************************************************/
   /**
    * @brief Update the game state after the welcome state.
    */
   void selectStateAfterWelcome();

   /**
    * @brief Update the game state after searching for data.
    */
   void selectStateAfterSearchData();

   /**
    * @brief Update the game state after selecting data.
    */
   void selectStateAfterSelectData();

   /**
    * @brief Update the game state after reading data.
    */
   void selectStateAfterReadData();

   /**
    * @brief Update the game state after searching for save data.
    */
   void selectStateAfterSearchSaves();

   /**
    * @brief Update the game state after the main menu.
    */
   void selectStateAfterMenu();

   /**
    * @brief Update the game state during gameplay.
    */
   void selectStateAfterPlaying();

   /**
    * @brief Update the game state after loading a saved game.
    */
   void selectStateAfterLoadSave();

   /**
    * @brief Update the game state after reading save data.
    */
   void selectStateAfterReadSave();

   /**
    * @brief Update the game state after saving the game.
    */
   void selectStateAfterSaveGame();

   /**
    * @brief Update the game state after initiating the quitting process.
    */
   void selectStateAfterQuitting();

   /*******************************************************************
    * @brief Draw functions
    *****************************************************************/
   /**
    * @brief Print the current game board.
    */
   void printBoard() const;

 public:
   /**
    * @brief Default constructor.
    */
   GameController() = default;

   /**
    * @brief Default destructor.
    */
   ~GameController() = default;

   /**
    * @brief Gets the unique instance of the GameController class.
    * @return Reference to the instance of the GameController class.
    */
   static GameController &getInstance();

   /**
    * @brief Destroys the unique instance of the GameController class.
    */
   static void destruct();

   /**
    * @brief Initializes the game with command line arguments.
    * @param argc_ The number of command line arguments.
    * @param argv_ An array of strings containing the command line arguments.
    */
   void initialize(int argc_, char *argv_[]);

   /**
    * @brief Executes the game processing loop.
    */
   void process();

   /**
    * @brief Updates the game state.
    */
   void update();

   /**
    * @brief Renders the game interface.
    */
   void render();

   /**
    * @brief Checks if the game should exit.
    * @return true if the game should exit, false otherwise.
    */
   bool exit() const;
};

#endif /// GAME_CONTROLLER_HPP_
