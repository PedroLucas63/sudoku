#include "GameController.hpp"
#include "Validate.hpp"
#include "View.hpp"
#include "cli/Cli.hpp"
#include "explorer/Explorer.hpp"
#include <iostream>
#include <string>

/// Static member representing the unique instance of the GameController class.
GameController *GameController::m_instance = nullptr;

/// @brief Get the unique instance of the GameController class.
/// If an instance does not exist, it is created.
/// @return A reference to the unique instance of the GameController class.
GameController &GameController::getInstance() {
   if (m_instance == nullptr) {
      m_instance = new GameController;
   }

   return *m_instance;
}

/// @brief Destroys the unique instance of the GameController class, if it
/// exists.
void GameController::destruct() {
   // Check if the unique instance exists
   if (m_instance != nullptr) {
      // Delete the unique instance
      delete m_instance;
   }

   // Set the unique instance pointer to nullptr
   m_instance = nullptr;
}

/// @brief Initialize the GameController with command-line arguments.
/// @param argc_ The number of command-line arguments.
/// @param argv_ An array of command-line argument strings.
void GameController::initialize(int argc_, char *argv_[]) {
   ext::CLI cli;

   // Parse command-line arguments for checks and help.
   cli.addArgumentInteger('c', "checks", DEFAULT_CHECKS);
   cli.addArgument('h', "help");
   cli.parse(argc_, argv_);

   // Set the 'm_help' flag based on the presence of the help option.
   m_help = cli.receiveSimple("-h");

   // Get the number of checks from the command line and set it with a minimum
   // check value.
   int checks{cli.receiveInteger("-c")};
   m_game_checks = checks >= MINIMUM_CHECKS ? checks : DEFAULT_CHECKS;

   // Receive a buffer of command-line arguments.
   std::vector<std::string> buffers{cli.receiveBuffer()};
   if (!buffers.empty()) {
      ext::FileHandler input{buffers.front()};
      // Check if the provided buffer exists and has the correct extension.
      if (input.exists() && input.extension() == DATA_EXTENSION) {
         // Add the buffer to the input data vector.
         m_input_datas.push_back(buffers.front());
      }
   }

   // Set the initial state of the GameController to 'Start'.
   m_state = Start;
}

/// @brief Process the game logic based on the current game state.
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
   case Playing:
      getOptionPlay();
      break;
   case CheckWinner:
      pressEnter();
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

/// @brief Update the game state based on the current state.
void GameController::update() {
   switch (m_state) {
   case Start:
      m_state = Welcome;
      break;
   case Welcome:
      selectStateAfterWelcome();
      break;
   case Helper:
      m_state = Ending;
      break;
   case SearchDatas:
      selectStateAfterSearchData();
      break;
   case SelectData:
      selectStateAfterSelectData();
      break;
   case ReadData:
      selectStateAfterReadData();
      break;
   case SearchSaves:
      selectStateAfterSearchSaves();
      break;
   case ReadSaves:
      m_state = LoadGame;
      break;
   case LoadGame:
      m_state = Menu;
      break;
   case Menu:
      selectStateAfterMenu();
      break;
   case Playing:
      selectStateAfterPlaying();
      break;
   case CheckWinner:
      m_state = Menu;
      break;
   case NewGame:
      m_state = Menu;
      break;
   case SaveGame:
      selectStateAfterSaveGame();
      break;
   case OverwriteSave:
      m_state = Menu;
      break;
   case SelectSave:
      selectStateAfterLoadSave();
      break;
   case ReadUserSave:
      selectStateAfterReadSave();
      break;
   case About:
      m_state = Menu;
      break;
   case Quitting:
      selectStateAfterQuitting();
      break;
   default:
      break;
   }
}

/// @brief Render the game based on the current state.
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
   case Playing:
      renderPlay([&]() { printBoard(); }, m_current_game.getChecks(),
                 m_current_game.getDigitsLeft(), m_play_message);
      break;
   case CheckWinner:
      renderWinner([&]() { m_current_game.drawOnlyWrong(); },
                   m_current_game.getChecks(), m_current_game.getDigitsLeft(),
                   m_current_game.checkWinner());
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

/// @brief Check if the game should exit.
/// @return True if the game should exit, false otherwise.
bool GameController::exit() const { return m_state == Ending; }

/*******************************************************************
 * @brief Process functions
 *****************************************************************/

/// @brief Search for available data files in the DATA_FOLDER directory.
void GameController::searchDatas() {
   ext::Explorer data{DATA_FOLDER};

   // Check if the DATA_FOLDER exists.
   if (!data.exists()) {
      return;
   }

   ext::List files{data.getImediateChildrens()};

   for (size_t index{0}; index != files.getFilesSize(); ++index) {
      ext::FileHandler file{files.atFiles(index)};
      // Check if the file has the DATA_EXTENSION.
      if (file.extension().generic_string() == DATA_EXTENSION) {
         m_input_datas.push_back(file.generic_string());
      }
   }
}

/// @brief Select a data file based on user input.
void GameController::selectData() {
   try {
      std::string buffer;
      std::getline(std::cin, buffer);
      m_select_data = std::stoi(buffer);

      // Check if the selected data index is valid.
      if (m_select_data >= 0 && m_select_data < m_input_datas.size()) {
         // Update the currently selected data based on user input.
         m_input_datas.front() = m_input_datas[m_select_data];
      }
   } catch (...) {
      // Handle exceptions by setting an invalid selection.
      m_select_data = -1;
   }
}

/// @brief Read and process a data file.
void GameController::readData() {
   // Read the data from the selected file and create a Bank object.
   Bank bank{readBankFromFile(m_input_datas[m_select_data])};

   // Check if the read data is valid and can be used.
   if (validateBank(bank)) {
      // Update the game's bank with the valid data.
      m_bank = bank;
   }
}

/// @brief Search for available save files in the SAVE_FOLDER directory.
void GameController::searchSaves() {
   ext::Explorer save{SAVE_FOLDER};

   // Check if the SAVE_FOLDER exists.
   if (!save.exists()) {
      // Create the SAVE_FOLDER directory if it doesn't exist.
      ext::Explorer(".").createDirectory(save);
      return;
   }

   ext::List files{save.getImediateChildrens()};

   for (size_t index{0}; index != files.getFilesSize(); ++index) {
      ext::FileHandler file{files.atFiles(index)};

      // Check if the file has the SAVE_EXTENSION.
      if (file.extension().generic_string() == SAVE_EXTENSION) {
         m_input_saves.push_back(file.generic_string());
      }
   }
}

/// @brief Read and process save files that match the current bank's ID and
/// version.
void GameController::readSaves() {
   for (std::string local_save : m_input_saves) {
      // Read a save file and create a Save object.
      Save save{readGameFromFile(local_save)};

      // Check if the save file matches the current bank's ID and version.
      if (save.m_bank_id == m_bank.m_id &&
          save.m_bank_version == m_bank.m_version) {
         // Validate the save data.
         if (validateSave(save)) {
            // Add the valid save data to the game's list of saves.
            std::pair<std::string, Save> save_pair{local_save, save};
            m_saves.push_back(save_pair);
         }
      }
   }
}

/// @brief Load a game from the current bank's boards and initialize it.
void GameController::loadGame() {
   // Set the selected game to the first game in the bank.
   m_select_game = 0;

   // Create a new Sudoku game using the board from the bank and the specified
   // checks.
   m_current_game = Sudoku{m_bank.m_boards[m_select_game], m_game_checks};
}
/// @brief Get the user's menu option selection and update the menu option.
void GameController::getOptionMenu() {
   // Clear the menu warning message.
   m_menu_warning.clear();

   // Read the user's menu option selection.
   std::string option;
   std::getline(std::cin, option);

   try {
      // Convert the input option to an integer.
      int value = std::stoi(option);

      switch (value) {
      case 1:
         m_return_menu = false;
         m_menu_option = MenuPlay;
         break;
      case 2:
         m_menu_option = MenuNewGame;
         break;
      case 3:
         defineSaveOrLoad();
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
      // Handle exceptions by setting the menu option to MenuNone.
      m_menu_option = MenuNone;
   }
}

/// @brief Determine whether to save or load a game based on the game's actions
/// count.
void GameController::defineSaveOrLoad() {
   if (m_current_game.getActionsCount()) {
      // If the game has actions, set the menu option to Save Game.
      m_menu_option = MenuSaveGame;
   } else {
      // If the game has no actions, set the menu option to Load Game.
      m_menu_option = MenuLoadGame;
   }
}

/// @brief Get the user's input commands for playing the game.
void GameController::getOptionPlay() {
   // Check if the game board is already full.
   if (m_current_game.checkFull()) {
      return;
   }

   // Initialize variables.
   m_print_check = false;
   ext::fstring full_command;
   std::getline(std::cin, full_command);

   // Trim leading and trailing whitespace from the input command.
   full_command.trim();

   if (full_command.empty()) {
      m_return_menu = true;
   } else {
      // Split the input command into individual commands.
      std::vector<std::string> commands;
      full_command.split(commands, ' ');

      // Get the first command from the input.
      std::string command{commands.front()};

      if (command == "p") {
         // Insert a number into the game board.
         insertNumber(commands);
      } else if (command == "r") {
         // Remove a number from the game board.
         removeNumber(commands);
      } else if (command == "c") {
         // Check the game board for correctness.
         checkBoard();
      } else if (command == "u") {
         // Undo the last move on the game board.
         undoBoard();
      } else {
         // Handle unrecognized functions.
         m_play_message = "No functions recognized.";
      }
   }
}

/// @brief Insert a number into the game board based on user input.
/// @param commands_ A vector of input commands containing row, column, and
/// number.
void GameController::insertNumber(std::vector<std::string> commands_) {
   if (commands_.size() >= 4) {
      std::string row{commands_[1]};
      std::string column{commands_[2]};
      std::string number{commands_[3]};

      // Calculate the row integer from the row character.
      int row_int{std::toupper(row.front()) - 'A' + 1};

      try {
         int column_int{std::stoi(column)};
         int number_int{std::stoi(number)};

         // Attempt to insert the number into the game board.
         std::pair<bool, std::string> result{
             m_current_game.insert(number_int, column_int, row_int)};

         // Set the play message based on the insertion result.
         m_play_message = result.second;
      } catch (...) {
         // Handle exceptions by indicating an incorrect entry format.
         m_play_message = "One of the entries is not in the requested format.";
      }
   } else {
      // Handle cases where there are insufficient entries.
      m_play_message = "Insufficient number of entries.";
   }
}

/// @brief Remove a number from the game board based on user input.
/// @param commands_ A vector of input commands containing row and column.
void GameController::removeNumber(std::vector<std::string> commands_) {
   if (commands_.size() >= 3) {
      std::string row{commands_[1]};
      std::string column{commands_[2]};

      // Calculate the row integer from the row character.
      int row_int{std::toupper(row.front()) - 'A' + 1};

      try {
         int column_int{std::stoi(column)};

         // Attempt to remove the number from the game board.
         std::pair<bool, std::string> result{
             m_current_game.remove(column_int, row_int)};

         // Set the play message based on the removal result.
         m_play_message = result.second;
      } catch (...) {
         // Handle exceptions by indicating an incorrect entry format.
         m_play_message = "One of the entries is not in the requested format.";
      }
   } else {
      // Handle cases where there are insufficient entries.
      m_play_message = "Insufficient number of entries.";
   }
}

/// @brief Check the current game board for correctness and update play
/// messages.
void GameController::checkBoard() {
   // Check the current game board and get the result as a pair.
   std::pair<bool, std::string> result{m_current_game.check()};

   // Set whether to print the check result based on the first value in the
   // pair.
   m_print_check = result.first;

   // Set the play message based on the second value in the pair.
   m_play_message = result.second;
}

/// @brief Undo the last move on the current game board and update the play
/// message.
void GameController::undoBoard() {
   // Undo the last move on the current game board and get the result as a pair.
   std::pair<bool, std::string> result{m_current_game.undo()};

   // Set the play message based on the result.
   m_play_message = result.second;
}

/// @brief Start a new game, allowing the user to confirm or cancel.
void GameController::newGame() {
   // Check if the current game has actions.
   if (m_current_game.getActionsCount()) {
      std::string buffer;
      std::getline(std::cin, buffer);

      // Check if the user's input is empty.
      if (buffer.empty()) {
         m_new_game = false;
      } else {
         // Get the first character from the input.
         char option = buffer.front();

         // Check if the input confirms the new game (using CONFIRM_KEY).
         if (std::tolower(option) == CONFIRM_KEY) {
            generateNewGame();
         } else {
            m_new_game = false;
         }
      }
   } else {
      // If there are no actions in the current game, generate a new game
      // directly.
      generateNewGame();
   }
}

/// @brief Generate a new game by selecting the next board in the bank.
void GameController::generateNewGame() {
   // Select the next game board in the bank (looping back to the first if
   // necessary).
   m_select_game = (m_select_game + 1) % m_bank.m_boards.size();

   // Create a new Sudoku game using the selected board and the specified
   // checks.
   m_current_game = Sudoku{m_bank.m_boards[m_select_game], m_game_checks};

   // Reset the new game flag and clear the play message.
   m_new_game = true;
   m_play_message.clear();
}

/// @brief Initiate the process of saving the current game with a user-specified
/// name.
void GameController::saveGame() {
   std::string buffer;
   std::getline(std::cin, buffer);

   // Check if the user's input is empty.
   if (buffer.empty()) {
      m_saving = true;
      return;
   }

   // Create a save file name by appending the user's input to the save folder
   // and extension.
   std::string save_name{SAVE_FOLDER + '/' + buffer + SAVE_EXTENSION};

   // Create a file handler for the save file.
   ext::FileHandler save_file{save_name};

   // Set the saving string to the save file name.
   m_saving_str = save_name;

   // Check if the save file already exists.
   if (save_file.exists()) {
      m_saving = false;
   } else {
      // If the save file does not exist, save the game data to the file.
      saveGameInFile();
      m_saving = true;
   }
}

/// @brief Confirm the overwrite of an existing save file, if the user chooses
/// to do so.
void GameController::confirmOverwrite() {
   std::string buffer;
   std::getline(std::cin, buffer);

   // Check if the user's input is not empty.
   if (!buffer.empty()) {
      // Get the first character from the input.
      char option = buffer.front();

      // Check if the input confirms the overwrite (using CONFIRM_KEY).
      if (std::tolower(option) == CONFIRM_KEY) {
         // If confirmed, save the game data to the file.
         saveGameInFile();
      }
   }
}

/// @brief Save the current game data to a file using the provided save
/// information.
void GameController::saveGameInFile() {
   // Create a Save object to store game data.
   Save save;

   // Set the save object properties based on the current game state.
   save.m_bank_id = m_bank.m_id;
   save.m_bank_version = m_bank.m_version;
   save.m_board_id = m_current_game.getBoardId();
   save.m_checks = m_current_game.getChecks();
   save.m_actions = m_current_game.getActions();

   // Save the game data to a file using the provided save information and file
   // name.
   saveGameToFile(save, m_saving_str);
}

/// @brief Select and load a previously saved game based on user input.
void GameController::selectSave() {
   try {
      std::string buffer;
      std::getline(std::cin, buffer);
      m_select_save = std::stoi(buffer);

      // Check if the selected save index is within valid bounds.
      if (m_select_save >= 0 && m_select_save < m_saves.size()) {
         // Load the selected save game data and update the current game state.
         Save save{m_saves[m_select_save].second};
         m_select_game = save.m_board_id;
         m_current_game = Sudoku{m_bank.m_boards[save.m_board_id],
                                 save.m_checks, save.m_actions};
         m_play_message.clear();
      } else if (m_select_save != m_saves.size()) {
         // If the selection is not within valid bounds, set the selection to
         // -1.
         m_select_save = -1;
      }
   } catch (...) {
      // Handle exceptions by setting the selection to -1.
      m_select_save = -1;
   }
}

/// @brief Load a game from a locally specified save file.
void GameController::enterLocalSave() {
   std::string buffer;
   std::getline(std::cin, buffer);

   // Create a file handler for the specified save file.
   ext::FileHandler save_file{buffer};

   // Check if the save file exists and has the correct extension.
   if (save_file.exists() &&
       save_file.extension().generic_string() == SAVE_EXTENSION) {
      // Read the save data from the save file.
      Save save{readGameFromFile(save_file.generic_string())};

      // Check if the save data matches the current bank's ID and version.
      if (save.m_bank_id == m_bank.m_id &&
          save.m_bank_version == m_bank.m_version) {

         // Validate the save data.
         if (validateSave(save)) {
            // Load the selected save game data and update the current game
            // state.
            m_select_game = save.m_board_id;
            m_current_game = Sudoku{m_bank.m_boards[save.m_board_id],
                                    save.m_checks, save.m_actions};
            m_select_save = static_cast<size_t>(m_saves.size());
            m_play_message.clear();
            return;
         }
      }
   }

   // If loading the game fails or the file is invalid, set the selection to -1.
   m_select_save = -1;
}

/// @brief Wait for the user to press the Enter key.
void GameController::pressEnter() {
   std::string buffer;
   std::getline(std::cin, buffer);
}

/// @brief Confirm the intention to quit the game, if there are no unsaved
/// changes.
void GameController::confirmQuitting() {
   // Check if there are unsaved changes in the current game.
   if (m_current_game.getActionsCount()) {
      std::string buffer;
      std::getline(std::cin, buffer);

      // Check if the user's input is not empty.
      if (buffer.empty()) {
         m_quitting = false;
      } else {
         // Get the first character from the input.
         char option = buffer.front();

         // Check if the input confirms the intention to quit (using
         // CONFIRM_KEY).
         if (std::tolower(option) == CONFIRM_KEY) {
            m_quitting = true;
         } else {
            m_quitting = false;
         }
      }
   } else {
      // If there are no unsaved changes, confirm the intention to quit.
      m_quitting = true;
   }
}

/*******************************************************************
 * @brief Update functions
 *****************************************************************/

/// @brief Determine the next game state after the welcome screen.
void GameController::selectStateAfterWelcome() {
   // Check if the help option is enabled.
   if (m_help) {
      // If help is requested, transition to the Helper state.
      m_state = Helper;
   } else {
      // If help is not requested, check if there are available input data
      // files.
      m_state = m_input_datas.empty() ? SearchDatas : ReadData;
   }
}

/// @brief Determine the next game state after searching for available data
/// files.
void GameController::selectStateAfterSearchData() {
   // Check if there are no available input data files.
   if (m_input_datas.empty()) {
      // If no data files are found, transition to the Ending state with a
      // warning message.
      m_state = Ending;
      m_ending_warning = "No game banks are available to run.";
   } else {
      // If data files are found, transition to the SelectData state.
      m_state = SelectData;
   }
}

/// @brief Determine the next game state after selecting a data bank.
void GameController::selectStateAfterSelectData() {
   // Check if the selected data bank index is invalid.
   if (m_select_data < 0 || m_select_data >= m_input_datas.size()) {
      // If an invalid selection is made, transition to the Ending state with a
      // warning message.
      m_state = Ending;
      m_ending_warning = "No game bank was selected for execution.";
   } else {
      // If a valid data bank is selected, transition to the ReadData state.
      m_state = ReadData;
   }
}

/// @brief Determine the next game state after reading data from the selected
/// bank.
void GameController::selectStateAfterReadData() {
   // Check if the selected bank has no game boards or is empty.
   if (m_bank.m_boards.empty()) {
      // If the bank is empty or corrupt, transition to the Ending state with a
      // warning message.
      m_state = Ending;
      m_ending_warning = "The selected game bank is corrupt or empty.";
   } else {
      // If the bank has valid game boards, transition to the SearchSaves state.
      m_state = SearchSaves;
   }
}

/// @brief Determine the next game state after searching for available save
/// files.
void GameController::selectStateAfterSearchSaves() {
   // Check if there are no available save files.
   if (m_input_saves.empty()) {
      // If no save files are found, transition to the LoadGame state.
      m_state = LoadGame;
   } else {
      // If save files are found, transition to the ReadSaves state.
      m_state = ReadSaves;
   }
}

/// @brief Determine the next game state after processing the main menu option.
void GameController::selectStateAfterMenu() {
   // Check the selected menu option and determine the next game state
   // accordingly.
   switch (m_menu_option) {
   case MenuPlay:
      m_state = Playing;
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

/// @brief Determine the next game state after playing a game.
void GameController::selectStateAfterPlaying() {
   // Check game conditions and player input to determine the next game state.
   if (m_current_game.checkFull()) {
      // If the game is complete, transition to the CheckWinner state.
      m_state = CheckWinner;
   } else if (m_return_menu) {
      // If the player wants to return to the menu, transition to the Menu
      // state.
      m_state = Menu;
   }
}

/// @brief Determine the next game state after attempting to load a saved game.
void GameController::selectStateAfterLoadSave() {
   // Check the result of the load save operation and determine the next game
   // state.
   if (m_select_save == m_saves.size()) {
      // If the player chooses to enter the name of a local save, transition to
      // ReadUserSave state.
      m_state = ReadUserSave;
   } else if (m_select_save != -1) {
      // If a valid save is selected, transition to the Menu state.
      m_state = Menu;
   } else {
      // If the selected save does not exist or is corrupt, show a warning and
      // transition to the Menu state.
      m_menu_warning = "The selected save does not exist or is corrupt.";
      m_state = Menu;
   }
}

/// @brief Determine the next game state after reading a saved game.
void GameController::selectStateAfterReadSave() {
   // Check the result of reading the saved game and determine the next game
   // state.
   if (m_select_save != -1) {
      // If a valid save is read, transition to the Menu state.
      m_state = Menu;
   } else {
      // If the selected save does not exist or is corrupt, show a warning and
      // transition to the Menu state.
      m_menu_warning = "The selected save does not exist or is corrupt.";
      m_state = Menu;
   }
}

/// @brief Determine the next game state after saving a game.
void GameController::selectStateAfterSaveGame() {
   // Check whether the game is currently in the process of saving.
   if (m_saving) {
      // If the game is still saving, stay in the Menu state.
      m_state = Menu;
   } else {
      // If the save operation is complete, transition to the OverwriteSave
      // state.
      m_state = OverwriteSave;
   }
}

/// @brief Determine the next game state after the player confirms quitting.
void GameController::selectStateAfterQuitting() {
   // Check if the player has confirmed quitting the game.
   if (m_quitting) {
      // If the player confirms quitting, transition to the Ending state.
      m_state = Ending;
   } else {
      // If the player chooses not to quit, return to the Menu state.
      m_state = Menu;
   }
}

/*******************************************************************
 * @brief Draw functions
 *****************************************************************/

/// @brief Print the Sudoku board, optionally showing the check results.
void GameController::printBoard() const {
   // Check if the game is configured to display check results.
   if (m_print_check) {
      // If displaying check results, draw the Sudoku board with checks.
      m_current_game.drawCheck();
   } else {
      // If not displaying check results, draw the standard Sudoku board.
      m_current_game.draw();
   }
}
