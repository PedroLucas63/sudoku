#ifndef ACTION_HPP_
#define ACTION_HPP_

class Action {
   public:
   enum Command {
      Place = 0,
      Remove = 1,
   };

   Action() = default;
   Action(Command command_, short row_, short col_, short value_ = -1) : m_command(command_), m_row(row_), m_col(col_), m_value(value_) {}
   ~Action() = default;

   Command getCommand() const;
   short getRow() const;
   short getColumn() const;
   short getValue() const;

   void setCommand(Command command_);
   void setRow(short row_);
   void setColumn(short col_);
   void setValue(short value_);
   
   private:
   Command m_command;
   short m_row, m_col;
   short m_value;
};

#endif /// ACTION_HPP_