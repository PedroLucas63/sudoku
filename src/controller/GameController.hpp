#ifndef GAME_CONTROLLER_HPP_
#define GAME_CONTROLLER_HPP_

#include <string>
#include <vector>

constexpr int const MINIMUM_CHECKS { 0 };
constexpr int const DEFAULT_CHECKS { 3 };

class GameController {
   private:
   enum States {
      Start,
      Welcome,
      Helper,
      SearchDatas,
      ReadData,
      SearchSaves,
      ReadSaves,
      Ending,
   };

   static GameController* m_instance;
   States m_state;
   bool m_help;
   int m_game_checks;
   std::string m_input_data;

   GameController(GameController const& other_) = delete;
   GameController& operator=(GameController const& other_) = delete;

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