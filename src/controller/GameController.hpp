#ifndef GAME_CONTROLLER_HPP_
#define GAME_CONTROLLER_HPP_

#include <string>
#include <vector>
#include "Sudoku.hpp"
#include "Save.hpp"

constexpr int const MINIMUM_CHECKS { 0 };
constexpr int const DEFAULT_CHECKS { 3 };
std::string const DATA_FOLDER { "./data" };
std::string const DATA_EXTENSION { ".sdk" };
std::string const SAVE_FOLDER { "./save" };
std::string const SAVE_EXTENSION { ".sav" };
constexpr char const CONFIRM_KEY {'y'};

class GameController {
   private:
   enum States {
      Start,
      Welcome,
      Helper,
      SearchDatas,
      SelectData,
      ReadData,
      SearchSaves,
      ReadSaves,
      LoadGame,
      Menu,
      Playing,
      CheckWinner,
      NewGame,
      SaveGame,
      OverwriteSave,
      SelectSave,
      ReadUserSave,
      About,
      Quitting,
      Ending,
   };

   enum MenuOption {
      MenuPlay,
      MenuNewGame,
      MenuLoadGame,
      MenuSaveGame,
      MenuAbout,
      MenuQuit,
      MenuNone,
   };

   static GameController* m_instance;

   States m_state;
   MenuOption m_menu_option;
   bool m_quitting;
   bool m_new_game;
   bool m_saving;
   bool m_help;
   std::vector<std::string> m_input_datas;
   std::vector<std::string> m_input_saves;
   std::string m_saving_str;
   std::string m_menu_warning;
   std::string m_ending_warning;

   int m_select_data;
   int m_select_game;
   int m_select_save;

   Bank m_bank;
   int m_game_checks;
   Sudoku m_current_game;
   std::vector<std::pair<std::string, Save>> m_saves;

   std::string m_play_message;
   bool m_print_check;
   bool m_return_menu;

   GameController(GameController const& other_) = delete;
   GameController& operator=(GameController const& other_) = delete;

   void selectData();
   void searchDatas();
   void readData();
   void searchSaves();
   void readSaves();
   void loadGame();
   void getOptionMenu();
   void getOptionPlay();
   void newGame();
   void saveGame();
   void confirmOverwrite();
   void selectSave();
   void enterLocalSave();
   void pressEnter();
   void confirmQuitting();

   void selectNextWelcomeState();
   void selectNextSearchDataState();
   void selectNextSelectDataState();
   void selectNextReadDataState();
   void selectNextSearchSavesState();
   void selectNextMenuState();
   void selectNextPlayingState();
   void selectNextLoadSaveState();
   void selectNextReadSaveState();
   void selectNextSaveGameState();
   void selectNextQuittingState();

   public:   
   GameController() = default;
   ~GameController() = default;

   static GameController& getInstance();
   static void destruct();

   void initialize(int argc_, char* argv_[]);

   void process();
   void update();
   void render();

   bool exit() const;
};

#endif /// GAME_CONTROLLER_HPP_