#include "GameController.hpp"

int main(int argc, char* argv[]) {
    GameController& game {GameController::getInstance()};
    game.destruct();
}