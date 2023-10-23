#ifndef GAME_CONTROLLER_HPP_
#define GAME_CONTROLLER_HPP_

class GameController {
   private:
   static GameController* m_instance;

   GameController(GameController const& other_) = delete;
   GameController& operator=(GameController const& other_) = delete;

   public:   
   GameController() = default;
   ~GameController() = default;

   static GameController& getInstance();
   static void destruct();
};

#endif /// GAME_CONTROLLER_HPP_