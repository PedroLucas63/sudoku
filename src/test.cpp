#include "classes/Sudoku.hpp"
#include <iostream>

int main() {
    Board board;
    board.m_id = 1;
    // Inicialize os valores com os dados fornecidos
    int values[BOARD_SIZE][BOARD_SIZE] = {
        {3, -4, -9, -7, 2, -6, -8, -5, 1},
        {-8, -7, -1, -3, 4, -5, -9, -2, -6},
        {-2, 5, -6, 8, -9, -1, 4, -7, -3},
        {-1, 3, -2, -4, 7, -9, 6, -8, 5},
        {-7, 6, -4, -5, -3, 8, 2, -1, 9},
        {5, -9, 8, 1, -6, -2, -7, -3, -4},
        {-6, -8, -3, -2, -5, -4, -1, 9, -7},
        {-9, 1, 5, -6, -8, 7, -3, -4, -2},
        {-4, -2, 7, 9, -1, -3, 5, -6, -8}
    };

    // Copie os valores iniciais para a estrutura Board
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board.m_values[i][j] = values[i][j];
        }
    }

   Sudoku game{board, 3};

   auto res = game.insert(4, 8, 1);
   game.draw();

    std::cout << "MSG: " << res.second << "\n";


    res = game.insert(7, 7, 1);
   game.draw();

    std::cout << "MSG: " << res.second << "\n";


       res = game.insert(7, 7, 1);
   game.draw();

    std::cout << "MSG: " << res.second << "\n";


    res = game.undo();
   game.draw();

    std::cout << "MSG: " << res.second << "\n";


       res = game.undo();
   game.draw();

    std::cout << "MSG: " << res.second << "\n";

       res = game.undo();
   game.draw();

    std::cout << "MSG: " << res.second << "\n";


          res = game.insert(7, 9, 1);
   game.draw();
    std::cout << "MSG: " << res.second << "\n";


             res = game.insert(8, 6, 7);
   game.draw();

    std::cout << "MSG: " << res.second << "\n";

}