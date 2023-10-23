#ifndef GAME_CONTROLLER_HPP_
#define GAME_CONTROLLER_HPP_

#include <string>
#include <vector>

constexpr int const MINIMUM_CHECKS { 0 };
constexpr int const DEFAULT_CHECKS { 3 };
std::string const DATA_FOLDER { "./data" };
std::string const DATA_EXTENSION { ".bak" };
std::string const SAVE_FOLDER { "./save" };
std::string const SAVE_EXTENSION { ".sav" };

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
      Ending,
   };

   static GameController* m_instance;
   States m_state;
   bool m_help;
   int m_game_checks;
   std::vector<std::string> m_input_datas;
   int m_select_data;

   GameController(GameController const& other_) = delete;
   GameController& operator=(GameController const& other_) = delete;

   void searchDatas();

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