#include "GameController.hpp"

int main(int argc, char *argv[]) {
   GameController &game{GameController::getInstance()};
   game.initialize(argc, argv);

   while (!game.exit()) {
      game.process();
      game.update();
      game.render();
   }

   game.destruct();
   return 0;
}