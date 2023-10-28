/**
 * @file Actions.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Defines actions related to Sudoku games.
 * @version 1.0
 * @date 2023-10-28
 *
 * This file defines an enum and a struct related to actions in Sudoku games.
 * The enum "Action" represents two possible actions: Insert (placing a value on
 * the Sudoku board) and Remove (removing a value from the Sudoku board). The
 * struct "ActionGame" describes an action in a Sudoku game, including its
 * coordinates (m_x and m_y), the value to be inserted or removed (m_value), and
 * the type of action (m_action).
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ACTIONS_HPP_
#define ACTIONS_HPP_

enum Action {
   Insert, /**< Represents the action of inserting a value onto the Sudoku
              board. */
   Remove, /**< Represents the action of removing a value from the Sudoku board.
            */
};

struct ActionGame {
   short m_x; /**< The x-coordinate (row) where the action is performed on the
                 Sudoku board. */
   short m_y; /**< The y-coordinate (column) where the action is performed on
                 the Sudoku board. */
   short m_value{0}; /**< The value to be inserted or removed (default is 0). */
   Action m_action;  /**< The type of action, either Insert or Remove. */
};

#endif /// ACTIONS_HPP_
