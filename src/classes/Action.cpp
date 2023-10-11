#include "Action.hpp"

   Action::Command Action::getCommand() const {
      return m_command;
   }
   short Action::getRow() const {
      return m_row;
   }
   short Action::getColumn() const {
      return m_col;
   }
   short Action::getValue() const {
      return m_value;
   }

   void Action::setCommand(Command command_) {
      m_command = command_;
   }
   void Action::setRow(short row_) {
      m_row = row_;
   }
   void Action::setColumn(short col_) {
      m_col = col_;
   }
   void Action::setValue(short value_) {
      m_value = value_;
   }