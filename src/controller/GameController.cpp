#include "GameController.hpp"
#include "cli/Cli.hpp"
#include "View.hpp"
#include "explorer/Explorer.hpp"
#include <iostream>
#include <string>

GameController* GameController::m_instance = nullptr;

GameController& GameController::getInstance() {
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

void GameController::initialize(int argc_, char* argv_[]) {
   ext::CLI cli;
   cli.addArgumentInteger('c', "checks");
   cli.addArgument('h', "help");

   cli.parse(argc_, argv_);

   m_help = cli.receiveSimple("-h");

   int checks { cli.receiveInteger("-c") };
   m_game_checks = checks >= MINIMUM_CHECKS ? checks : DEFAULT_CHECKS;

   std::vector<std::string> buffers { cli.receiveBuffer() };
   if (!buffers.empty()) {
      m_input_datas.push_back(buffers.front());
   }
}

void GameController::process() {
   switch (m_state) {
   case SearchDatas:
      searchDatas();
      break;
   case SelectData:
      try{
         std::string buffer;
         std::cin >> buffer;
         m_select_data = std::stoi(buffer);

         if (m_select_data >= 0 && m_select_data < m_input_datas.size()) {
            m_input_datas.front() = m_input_datas[m_select_data];
         }
      } catch (...) {
         m_select_data = -1;
      }
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
      if (m_help) {
         m_state = Helper;
      } else {
         m_state = m_input_datas.empty() ? SearchDatas : ReadData;
         break;
      }
      break;
   case Helper:
      m_state = Ending;
      break;
   case SearchDatas:
      m_state = m_input_datas.empty() ? Ending : SelectData;
      break;
   case SelectData:
      if (m_select_data < 0 || m_select_data >= m_input_datas.size()) {
         m_state = Ending;
      } else {
         m_state = ReadData;
      }
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
   default:
      break;
   }
}

bool GameController::exit() const {
   return m_state == Ending;
}

void GameController::searchDatas() {
   ext::Explorer data {"./data"};
   ext::List files {data.getImediateChildrens()};

   std::string data_extension { ".bak" };
   for (size_t index { 0 }; index != files.getFilesSize(); ++index) {
      ext::FileHandler file {files.atFiles(index)};
      if (file.extension() == data_extension) {
         m_input_datas.push_back(file.generic_string());
      }
   }
}
