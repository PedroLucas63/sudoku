#include "GameController.hpp"
#include "cli/Cli.hpp"
#include "View.hpp"

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
   m_input_data = buffers.size() != 0 ? buffers.front() : std::string();
}

void GameController::process() {
   switch (m_state) {
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
         m_state = m_input_data == std::string() ? SearchDatas : ReadData;
         break;
      }
      break;
   case Helper:
      m_state = Ending;
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
   default:
      break;
   }
}

bool GameController::exit() const {
   return m_state == Ending;
}