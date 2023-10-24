#ifndef ACTIONS_HPP_
#define ACTIONS_HPP_

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

#endif /// ACTIONS_HPP_