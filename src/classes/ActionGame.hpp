#ifndef ACTION_GAME_HPP_
#define ACTION_GAME_HPP_

enum Action {
   Insert,
   Remove,
};

struct ActionGame {
   short m_x;
   short m_y;
   short m_value{0};
   Action m_action;
};

#endif /// ACTION_GAME_HPP_