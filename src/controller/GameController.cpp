#include "GameController.hpp"
#include "Validate.hpp"
#include "View.hpp"
#include "cli/Cli.hpp"
#include "explorer/Explorer.hpp"
#include <iostream>
#include <string>

GameController *GameController::m_instance = nullptr;

GameController &GameController::getInstance() {
   if (m_instance == nullptr) {
      m_instance = new GameController;
   }

   return *m_instance;
}

void GameController::destruct() {
   if (m_instance != nullptr) {
      delete m_instance;
   }

   m_instance = nullptr;
}

void GameController::initialize(int argc_, char *argv_[]) {
   ext::CLI cli;
   cli.addArgumentInteger('c', "checks");
   cli.addArgument('h', "help");

   cli.parse(argc_, argv_);

   m_help = cli.receiveSimple("-h");

   int checks{cli.receiveInteger("-c")};
   /// TODO: Verificar se foi definido (Mudança no CLI para tupla <Reference*,
   /// bool, type>)
   m_game_checks = checks >= MINIMUM_CHECKS ? checks : DEFAULT_CHECKS;

   std::vector<std::string> buffers{cli.receiveBuffer()};
   if (!buffers.empty()) {
      ext::FileHandler input{buffers.front()};
      if (input.exists() && input.extension() == DATA_EXTENSION) {
         m_input_datas.push_back(buffers.front());
      }
   }
}

void GameController::process() {
   switch (m_state) {
   case SearchDatas:
      searchDatas();
      break;
   case SelectData:
      selectData();
      break;
   case ReadData:
      readData();
      break;
   case SearchSaves:
      searchSaves();
      break;
   case ReadSaves:
      readSaves();
      break;
   case LoadGame:
      loadGame();
      break;
   case Menu:
      getOptionMenu();
      break;
   case NewGame:
      newGame();
      break;
   case OverwriteSave:
      confirmOverwrite();
      break;
   case SaveGame:
      saveGame();
      break;
   case SelectSave:
      selectSave();
      break;
   case ReadUserSave:
      enterLocalSave();
      break;
   case About:
      pressEnter();
      break;
   case Quitting:
      confirmQuitting();
      break;
   default:
      break;
   }
}

void GameController::update() {
   switch (m_state) {
   case Start:
      m_state = Welcome;
      break;
   case Welcome:
      selectNextWelcomeState();
      break;
   case Helper:
      m_state = Ending;
      break;
   case SearchDatas:
      selectNextSearchDataState();
      break;
   case SelectData:
      selectNextSelectDataState();
      break;
   case ReadData:
      selectNextReadDataState();
      break;
   case SearchSaves:
      selectNextSearchSavesState();
      break;
   case ReadSaves:
      m_state = LoadGame;
      break;
   case LoadGame:
      m_state = Menu;
      break;
   case Menu:
      selectNextMenuState();
      break;
   case NewGame:
      m_state = Menu;
      break;
   case SaveGame:
      selectNextSaveGameState();
      break;
   case OverwriteSave:
      m_state = Menu;
      break;
   case SelectSave:
      selectNextLoadSaveState();
      break;
   case ReadUserSave:
      selectNextReadSaveState();
      break;
   case About:
      m_state = Menu;
      break;
   case Quitting:
      selectNextQuittingState();
      break;
   default:
      break;
   }
}

void GameController::render() {
   switch (m_state) {
   case Helper:
      renderHelper(DEFAULT_CHECKS);
      break;
   case SelectData:
      renderSelectData(m_input_datas);
      break;
   case Menu:
      renderMenu([&]() { m_current_game.draw(); },
                 m_current_game.getActionsCount(), m_menu_warning);
      break;
   case NewGame:
      renderNewGame(m_current_game.getActionsCount());
      break;
   case SaveGame:
      renderRequestSave();
      break;
   case OverwriteSave:
      renderOverwriteConfirm();
      break;
   case SelectSave:
      renderLoadSave(m_saves);
      break;
   case ReadUserSave:
      renderRequestSave();
      break;
   case About:
      renderAbout();
      break;
   case Quitting:
      renderQuitting(m_current_game.getActionsCount());
      break;
   case Ending:
      renderEnding(m_ending_warning);
      break;
   default:
      break;
   }
}

bool GameController::exit() const { return m_state == Ending; }

//*********************************
//* Process functions
//*********************************
void GameController::selectData() {
   try {
      std::string buffer;
      std::getline(std::cin, buffer);
      m_select_data = std::stoi(buffer);

      if (m_select_data >= 0 && m_select_data < m_input_datas.size()) {
         m_input_datas.front() = m_input_datas[m_select_data];
      }
   } catch (...) {
      m_select_data = -1;
   }
}

void GameController::searchDatas() {
   ext::Explorer data{"./data"};

   if (!data.exists()) {
      return;
   }

   ext::List files{data.getImediateChildrens()};

   for (size_t index{0}; index != files.getFilesSize(); ++index) {
      ext::FileHandler file{files.atFiles(index)};
      if (file.extension().generic_string() == DATA_EXTENSION) {
         m_input_datas.push_back(file.generic_string());
      }
   }
}

void GameController::readData() {
   Bank bank{readBankFromFile(m_input_datas[m_select_data])};

   if (validateBank(bank)) {
      m_bank = bank;
   }
}

void GameController::searchSaves() {
   ext::Explorer save{SAVE_FOLDER};

   if (!save.exists()) {
      ext::Explorer(".").createDirectory(save);
      return;
   }

   ext::List files{save.getImediateChildrens()};

   for (size_t index{0}; index != files.getFilesSize(); ++index) {
      ext::FileHandler file{files.atFiles(index)};
      
      if (file.extension().generic_string() == SAVE_EXTENSION) {
         m_input_saves.push_back(file.generic_string());
      }
   }
}

void GameController::readSaves() {
   for (std::string local_save : m_input_saves) {
      Save save{readGameFromFile(local_save)};

      if (save.m_bank_id == m_bank.m_id &&
          save.m_bank_version == m_bank.m_version) {
         if (validateSave(save)) {
            std::pair<std::string, Save> save_pair{local_save, save};
            m_saves.push_back(save_pair);
         }
      }
   }
}

void GameController::loadGame() {
   m_select_game = 0;
   m_current_game = Sudoku{m_bank.m_boards[m_select_game], m_game_checks};
}

void GameController::getOptionMenu() {
   m_menu_warning.clear();

   std::string option;
   std::getline(std::cin, option);

   try {
      int value = std::stoi(option);

      switch (value) {
      case 1:
         m_menu_option = MenuPlay;
         break;
      case 2:
         m_menu_option = MenuNewGame;
         break;
      case 3:
         if (m_current_game.getActionsCount()) {
            m_menu_option = MenuSaveGame;
         } else {
            m_menu_option = MenuLoadGame;
         }
         break;
      case 4:
         m_menu_option = MenuAbout;
         break;
      case 5:
         m_menu_option = MenuQuit;
         break;
      default:
         m_menu_option = MenuNone;
         break;
      }
   } catch (...) {
      m_menu_option = MenuNone;
   }
}

void GameController::newGame() {
   if (m_current_game.getActionsCount()) {
      std::string buffer;
      std::getline(std::cin, buffer);

      if (buffer.empty()) {
         m_new_game = false;
      } else {
         char option = buffer.front();

         if (std::tolower(option) == CONFIRM_KEY) {
            m_select_game = (m_select_game + 1) % m_bank.m_boards.size();
            m_current_game =
                Sudoku{m_bank.m_boards[m_select_game], m_game_checks};
            m_new_game = true;
         } else {
            m_new_game = false;
         }
      }
   } else {
      m_select_game = (m_select_game + 1) % m_bank.m_boards.size();
      m_current_game = Sudoku{m_bank.m_boards[m_select_game], m_game_checks};
      m_new_game = true;
   }
}

void GameController::saveGame() {
   std::string buffer;
   std::getline(std::cin, buffer);

   if (buffer.empty()) {
      m_saving = true;
   }

   std::string save_name{SAVE_FOLDER + '/' + buffer + SAVE_EXTENSION};

   ext::FileHandler save_file{save_name};

   if (save_file.exists()) {
      m_saving = false;
      m_saving_str = save_name;
   } else {
      Save save;
      save.m_bank_id = m_bank.m_id;
      save.m_bank_version = m_bank.m_version;
      save.m_board_id = m_current_game.getBoardId();
      save.m_checks = m_current_game.getChecks();
      save.m_actions = m_current_game.getActions();

      saveGameToFile(save, save_file);
      m_saving = true;
   }
}

void GameController::confirmOverwrite() {
   std::string buffer;
   std::getline(std::cin, buffer);

   if (!buffer.empty()) {
      char option = buffer.front();

      if (std::tolower(option) == CONFIRM_KEY) {
         Save save;
         save.m_bank_id = m_bank.m_id;
         save.m_bank_version = m_bank.m_version;
         save.m_board_id = m_current_game.getBoardId();
         save.m_checks = m_current_game.getChecks();
         save.m_actions = m_current_game.getActions();

         saveGameToFile(save, m_saving_str);
      }
   }
}

void GameController::selectSave() {
   try {
      std::string buffer;
      std::getline(std::cin, buffer);
      m_select_save = std::stoi(buffer);

      if (m_select_save >= 0 && m_select_save < m_saves.size()) {
         Save save{m_saves[m_select_save].second};
         m_select_game = save.m_board_id;
         m_current_game = Sudoku{m_bank.m_boards[save.m_board_id],
                                 save.m_checks, save.m_actions};
      } else if (m_select_save != m_saves.size()) {
         m_select_save = -1;
      }
   } catch (...) {
      m_select_save = -1;
   }
}

void GameController::enterLocalSave() {
   std::string buffer;
   std::getline(std::cin, buffer);

   ext::FileHandler save_file{buffer};

   if (save_file.exists() &&
       save_file.extension().generic_string() == SAVE_EXTENSION) {
      Save save{readGameFromFile(save_file.generic_string())};
      if (save.m_bank_id == m_bank.m_id &&
          save.m_bank_version == m_bank.m_version) {
         if (validateSave(save)) {
            m_select_game = save.m_board_id;
            m_current_game = Sudoku{m_bank.m_boards[save.m_board_id],
                                    save.m_checks, save.m_actions};
         }
      }
   }

   m_select_save = -1;
}

void GameController::pressEnter() {
   std::string buffer;
   std::getline(std::cin, buffer);
}

void GameController::confirmQuitting() {
   if (m_current_game.getActionsCount()) {
      std::string buffer;
      std::getline(std::cin, buffer);

      if (buffer.empty()) {
         m_quitting = false;
      } else {
         char option = buffer.front();

         if (std::tolower(option) == CONFIRM_KEY) {
            m_quitting = true;
         } else {
            m_quitting = false;
         }
      }
   } else {
      m_quitting = true;
   }
}

//*********************************
//* Update functions
//*********************************

void GameController::selectNextWelcomeState() {
   if (m_help) {
      m_state = Helper;
   } else {
      m_state = m_input_datas.empty() ? SearchDatas : ReadData;
   }
}

void GameController::selectNextSearchDataState() {
   if (m_input_datas.empty()) {
      m_state = Ending;
      m_ending_warning = "No game banks are available to run.";
   } else {
      m_state = SelectData;
   }
}

void GameController::selectNextSelectDataState() {
   if (m_select_data < 0 || m_select_data >= m_input_datas.size()) {
      m_state = Ending;
      m_ending_warning = "No game bank was selected for execution.";
   } else {
      m_state = ReadData;
   }
}

void GameController::selectNextReadDataState() {
   if (m_bank.m_boards.empty()) {
      m_state = Ending;
      m_ending_warning = "The selected game bank is corrupt or empty.";
   } else {
      m_state = SearchSaves;
   }
}

void GameController::selectNextSearchSavesState() {
   if (m_input_saves.empty()) {
      m_state = LoadGame;
   } else {
      m_state = ReadSaves;
   }
}

void GameController::selectNextMenuState() {
   switch (m_menu_option) {
   case MenuPlay:
      break;
   case MenuNewGame:
      m_state = NewGame;
      break;
   case MenuLoadGame:
      m_state = SelectSave;
      break;
   case MenuSaveGame:
      m_state = SaveGame;
      break;
   case MenuAbout:
      m_state = About;
      break;
   case MenuQuit:
      m_state = Quitting;
      break;
   default:
      break;
   }
}

void GameController::selectNextLoadSaveState() {
   if (m_select_save == m_saves.size()) {
      m_state = ReadUserSave;
   } else if (m_select_save != -1) {
      m_state = Menu; /// TODO: PLAYING
   } else {
      m_menu_warning = "The selected save does not exist or is corrupt.";
      m_state = Menu;
   }
}

void GameController::selectNextReadSaveState() {
   if (m_select_save != -1) {
      m_state = Menu; /// TODO: PLAYING
   } else {
      m_menu_warning = "The selected save does not exist or is corrupt.";
      m_state = Menu;
   }
}

void GameController::selectNextSaveGameState() {
   if (m_saving) {
      m_state = Menu;
   } else {
      m_state = OverwriteSave;
   }
}

void GameController::selectNextQuittingState() {
   if (m_quitting) {
      m_state = Ending;
   } else {
      m_state = Menu;
   }
}
