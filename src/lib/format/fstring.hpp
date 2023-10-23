/**
 * @file fstring.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief A traditional std::string with lots of formatting features
 * @version 1.0
 * @date 2023-10-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef FSTRING_HPP_
#define FSTRING_HPP_

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "style.hpp"

/**
 * @brief Namespace 'ext' for external utilities and extensions.
 */
namespace ext {

/**
 * @brief A custom string class that extends std::basic_string.
 * @tparam CharType The character type used in the string (e.g., char, wchar_t).
 */
template <typename CharType = char>
class fstring : public std::basic_string<CharType> {
 public:
   using size_type = typename std::basic_string<CharType>::size_type;
   using difference_type =
       typename std::basic_string<CharType>::difference_type;

   /**
    * @brief Default constructor for fstring.
    */
   fstring() : std::basic_string<CharType>() {}

   /**
    * @brief Constructor to create an fstring with a specified character
    * repeated a number of times.
    * @param count_ The number of times to repeat the character.
    * @param char_ The character to repeat.
    */
   fstring(size_type count_, CharType char_)
       : std::basic_string<CharType>(count_, char_) {}

   /**
    * @brief Constructor to create an fstring from a substring of another
    * fstring.
    * @param rhs_ The source fstring.
    * @param pos_ The starting position of the substring.
    */
   fstring(fstring const &rhs_, size_type pos_)
       : std::basic_string<CharType>(rhs_, pos_) {}

   /**
    * @brief Constructor to create an fstring from a substring of another
    * fstring.
    * @param rhs_ The source fstring.
    * @param pos_ The starting position of the substring.
    * @param count_ The length of the substring.
    */
   fstring(fstring const &rhs_, size_type pos_, size_type count_)
       : std::basic_string<CharType>(rhs_, pos_, count_) {}

   /**
    * @brief Constructor to create an fstring from a character array with a
    * specified length.
    * @param rhs_ The character array.
    * @param count_ The length of the character array.
    */
   fstring(CharType const *rhs_, size_type count_)
       : std::basic_string<CharType>(rhs_, count_) {}

   /**
    * @brief Constructor to create an fstring from a null-terminated character
    * array.
    * @param rhs_ The null-terminated character array.
    */
   fstring(CharType const *rhs_) : std::basic_string<CharType>(rhs_) {}

   /**
    * @brief Constructor to create an fstring from a range of characters defined
    * by iterators.
    * @tparam It Iterator type for character range.
    * @param first_ The iterator to the beginning of the range.
    * @param last_ The iterator to the end of the range.
    */
   template <class It>
   fstring(It first_, It last_) : std::basic_string<CharType>(first_, last_) {}

   /**
    * @brief Constructor to create an fstring from a standard string.
    * @param rhs_ The source standard string.
    */
   fstring(std::basic_string<CharType> const &rhs_)
       : std::basic_string<CharType>(rhs_) {}

   /**
    * @brief Copy constructor for fstring.
    * @param rhs_ The source fstring to be copied.
    */
   fstring(fstring const &rhs_) : std::basic_string<CharType>(rhs_) {}

   /**
    * @brief Move constructor for fstring.
    * @param rhs_ The source fstring to be moved.
    */
   fstring(fstring const &&rhs_) : std::basic_string<CharType>(rhs_) {}

   /**
    * @brief Assignment operator for fstring.
    * @param rhs_ The source fstring to be assigned.
    * @return A reference to the modified fstring.
    */
   fstring &operator=(const fstring &rhs_) {
      if (this != &rhs_) {
         std::basic_string<CharType>::operator=(rhs_);
      }
      return *this;
   }

   /**
    * @brief Remove leading whitespace characters from the fstring.
    *
    * @param target_ A string containing characters to remove (defaults to "
    * \t\n\r\f\v").
    */
   void ltrim(std::basic_string<CharType> target_ = " \t\n\r\f\v") {
      this->erase(0, this->find_first_not_of(target_));
   }

   /**
    * @brief Remove trailing whitespace characters from the fstring.
    *
    * @param target_ A string containing characters to remove (defaults to "
    * \t\n\r\f\v").
    */
   void rtrim(std::basic_string<CharType> target_ = " \t\n\r\f\v") {
      this->erase(this->find_last_not_of(target_) + 1, this->size());
   }

   /**
    * @brief Remove leading and trailing whitespace characters from the fstring.
    *
    * @param target_ A string containing characters to remove (defaults to "
    * \t\n\r\f\v").
    */
   void trim(std::basic_string<CharType> target_ = " \t\n\r\f\v") {
      ltrim(target_);
      rtrim(target_);
   }

   /**
    * @brief Replaces the first occurrence of a character sequence with another.
    *
    * @param target_ The character sequence to be replaced.
    * @param replace_ The replacement character sequence.
    * @return An iterator to the first occurrence of the replacement character
    * sequence.
    */
   typename std::basic_string<CharType>::iterator
   replace_first(std::basic_string<CharType> const &target_,
                 std::basic_string<CharType> const &replace_) {
      size_t search = this->find(target_);

      if (search != std::basic_string<CharType>::npos) {
         this->replace(search, target_.length(), replace_);
         return (this->begin() + search);
      }

      return this->end();
   }

   /**
    * @brief Replaces all occurrences of a character sequence with another.
    *
    * @param target_ The character sequence to be replaced.
    * @param replace_ The replacement character sequence.
    * @return An iterator to the last occurrence of the replacement character
    * sequence.
    */
   typename std::basic_string<CharType>::iterator
   replace_all(std::basic_string<CharType> const &target_,
               std::basic_string<CharType> const &replace_) {
      fstring buffer{*this};

      auto it{this->end()};
      auto buffer_it{replace_first(target_, replace_)};

      while (*this != buffer) {
         buffer = *this;
         it = buffer_it;
         buffer_it = replace_first(target_, replace_);
      }

      return it;
   }

   /**
    * @brief Returns a substring from the specified indices.
    *
    * @param last_ The ending index of the substring.
    * @param first_ The starting index of the substring (default is 0).
    * @return A new fstring representing the specified substring.
    */
   fstring sub_fstring(size_type last_, size_type first_ = 0) const {
      std::ostringstream oss;

      for (size_type index{first_}; index != last_; ++index) {
         oss << (*this)[index];
      }

      return fstring(oss.str());
   }
   /**
    * @brief Splits the fstring into substrings based on a specified delimiter
    * and stores them in a container.
    *
    * @tparam Container The type of container to store the resulting substrings.
    * @param container_ The container to store the resulting substrings.
    * @param delimiter_ The character used as the delimiter (default is a
    * space).
    */
   template <class Container>
   void split(Container &container_, CharType const &delimiter_ = ' ') const {
      fstring buffer{*this};

      size_type pos{buffer.find_first_not_of(delimiter_)};

      while (pos != std::basic_string<CharType>::npos) {
         size_type pos_delimiter{buffer.find_first_of(delimiter_, pos)};
         if (pos_delimiter == std::basic_string<CharType>::npos) {
            pos_delimiter = buffer.length();
         }

         container_.push_back(buffer.sub_fstring(pos_delimiter, pos));
         buffer.erase(0, pos_delimiter);
         pos = buffer.find_first_not_of(delimiter_);
      }
   }

   /**
    * @brief Splits the fstring into substrings based on a specified delimiter,
    * allowing empty substrings, and stores them in a container.
    *
    * @tparam Container The type of container to store the resulting substrings.
    * @param container_ The container to store the resulting substrings.
    * @param delimiter_ The character used as the delimiter (default is a
    * space).
    */
   template <class Container>
   void split_with_empty(Container &container_,
                         CharType const &delimiter_ = ' ') const {
      fstring buffer{*this};

      bool additional_split{false};
      if (buffer.find_last_of(delimiter_) == buffer.size() - 1) {
         additional_split = true;
      };

      size_type pos{buffer.find_first_of(delimiter_)};

      while (pos != std::basic_string<CharType>::npos) {
         container_.push_back(buffer.sub_fstring(pos));
         buffer.erase(0, pos + 1);
         pos = buffer.find_first_of(delimiter_);
      }

      if (!buffer.empty()) {
         container_.push_back(buffer);
      }

      if (additional_split) {
         container_.push_back(fstring());
      }
   }

   /**
    * @brief Checks if the fstring contains a specified character sequence.
    *
    * @param target_ The character sequence to search for.
    * @return True if the character sequence is found, false otherwise.
    */
   bool contains(std::basic_string<CharType> const &target_) {
      return std::search(this->begin(), this->end(), target_.begin(),
                         target_.end()) != this->end();
   }

   /**
    * @brief Appends a specified value multiple times to the fstring.
    *
    * @tparam Type The type of value to append.
    * @param count_ The number of times to append the value.
    * @param value_ The value to be appended.
    */
   template <typename Type> void append(size_type count_, Type value_) {
      std::ostringstream oss;
      for (size_type times{0}; times != count_; ++times) {
         oss << value_;
      }

      *this += oss.str();
   }

   /**
    * @brief Returns the fstring enclosed in double quotes.
    *
    * @return An fstring enclosed in double quotes.
    */
   fstring quoted() const { return '"' + *this + '"'; }
   /**
    * @brief Splits the fstring into two parts at the specified size, using a
    * given separator.
    *
    * @param size_ The size at which the fstring should be split.
    * @param separator_ The character used as the separator (default is a
    * space).
    * @return The remaining part of the fstring after the split.
    */
   fstring split_at(size_type size_, CharType const &separator_ = ' ') {
      if (size_ == 0) {
         return fstring();
      }

      std::vector<fstring<CharType>> splitted;
      this->split_with_empty(splitted, separator_);

      fstring new_fstring;
      fstring remainder;

      bool catch_stop{false};

      for (fstring<CharType> _w : splitted) {
         if (!catch_stop) {
            if (new_fstring.size() + _w.size() + 1 <= size_) {
               new_fstring += _w + separator_;
            } else if (new_fstring.size() + _w.size() <= size_) {
               new_fstring += _w;
            } else {
               catch_stop = true;
            }
         }

         if (new_fstring.empty() && catch_stop) {
            catch_stop = true;
            new_fstring = _w.sub_fstring(size_);
            remainder += _w.sub_fstring(_w.size(), size_);
         } else if (catch_stop) {
            catch_stop = true;
            remainder += (!remainder.empty() ? separator_ : '\0') + _w;
         }
      }

      *this = new_fstring;
      return remainder;
   }

   /**
    * @brief Aligns the fstring to the left within a specified size by adding
    * separator characters on the right.
    *
    * @param new_size_ The new size to which the fstring should be aligned.
    * @param separator_ The character used to pad the fstring (default is a
    * space).
    */
   void align_left(size_type new_size_, CharType const &separator_ = ' ') {
      if (new_size_ < this->size()) {
         return;
      }

      this->append(new_size_ - this->size(), separator_);
   }
   /**
    * @brief Aligns the fstring to the center within a specified size by adding
    * separator characters on both sides.
    *
    * @param new_size_ The new size to which the fstring should be aligned.
    * @param separator_ The character used to pad the fstring (default is a
    * space).
    */
   void align_center(size_type new_size_, CharType const &separator_ = ' ') {
      if (new_size_ < this->size()) {
         return;
      }

      fstring add_space;
      add_space.append((new_size_ - this->size()) / 2, separator_);

      std::ostringstream oss;
      oss << add_space << *this << add_space;

      if (oss.str().size() < new_size_) {
         oss << separator_;
      }

      *this = oss.str();
   }

   /**
    * @brief Aligns the fstring to the right within a specified size by adding
    * separator characters on the left.
    *
    * @param new_size_ The new size to which the fstring should be aligned.
    * @param separator_ The character used to pad the fstring (default is a
    * space).
    */
   void align_right(size_type new_size_, CharType const &separator_ = ' ') {
      if (new_size_ < this->size()) {
         return;
      }

      fstring add_space;
      add_space.append(new_size_ - this->size(), separator_);

      std::ostringstream oss;
      oss << add_space << *this;

      *this = oss.str();
   }

   /**
    * @brief Aligns the fstring using a justified style within a specified size
    * by distributing separator characters.
    *
    * @param new_size_ The new size to which the fstring should be aligned.
    * @param separator_ The character used to pad the fstring (default is a
    * space).
    */
   void align_justify(size_type new_size_, CharType const &separator_ = ' ') {
      if (new_size_ < this->size()) {
         return;
      }

      std::vector<fstring<CharType>> splitted;
      split_with_empty(splitted, separator_);

      size_type words_with_space{splitted.size() - 1};

      if (words_with_space <= 0) {
         align_left(new_size_, separator_);
         return;
      }

      size_type spaces{(new_size_ - this->size()) + words_with_space};
      size_type spaces_per_word{spaces / words_with_space};
      size_type spaces_truncated{spaces % words_with_space};

      std::ostringstream oss;
      for (auto &word : splitted) {
         oss << word;

         if (&word != &splitted.back()) {
            oss << fstring(spaces_per_word, separator_);
         }

         if (spaces_truncated != 0) {
            oss << separator_;
            --spaces_truncated;
         }
      }

      *this = oss.str();
   }
   /**
    * @brief Sets the text color for the fstring using ANSI color codes.
    *
    * @param code_ The ANSI color code to apply (default is cfg::none).
    */
   void color(short code_ = cfg::none) {
      bool search{
          std::any_of(std::begin(cfg::list), std::end(cfg::list),
                      [code_](short color_) { return code_ == color_; })};

      if (!search || code_ == cfg::none) {
         return;
      }

      std::ostringstream oss;

      oss << "\33[" << code_ << "m" << *this << "\33[" << stl::regular << "m";
      *this = oss.str();
   }

   /**
    * @brief Sets the background color for the fstring using ANSI color codes.
    *
    * @param code_ The ANSI background color code to apply (default is
    * cbg::none).
    */
   void background(short code_ = cbg::none) {
      bool search{std::any_of(
          std::begin(cbg::list), std::end(cbg::list),
          [code_](short background_) { return code_ == background_; })};

      if (!search || code_ == cbg::none) {
         return;
      }

      std::ostringstream oss;

      oss << "\33[" << code_ << "m" << *this << "\33[" << stl::regular << "m";
      *this = oss.str();
   }

   /**
    * @brief Sets the text style for the fstring using ANSI style codes.
    *
    * @param code_ The ANSI style code to apply (default is stl::none).
    */
   void style(short code_ = stl::none) {
      bool search{
          std::any_of(std::begin(stl::list), std::end(stl::list),
                      [code_](short style_) { return code_ == style_; })};

      if (!search || code_ == stl::none) {
         return;
      }

      std::ostringstream oss;

      oss << "\33[" << code_ << "m" << *this << "\33[" << stl::regular << "m";
      *this = oss.str();
   }
};
} // namespace ext

#endif /// FSTRING_HPP_