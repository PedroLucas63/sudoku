/**
 * @file Reference.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Contains the classes Reference and CompostReference for managing
 * references.
 * @version 1.0
 * @date 2023-10-22
 *
 * This file defines the "Reference" and "CompostReference" classes, which
 * provide mechanisms for managing references represented as strings. The
 * "Reference" class represents a basic reference with a single character, while
 * the "CompostReference" class extends it to handle references as strings.
 *
 * The classes offer methods for setting, retrieving, and checking references,
 * making them suitable for various applications where references need to be
 * managed.
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef REFERENCE_HPP_
#define REFERENCE_HPP_

#include <string>

/**
 * @brief Namespace 'ext' for external utilities and extensions.
 */
namespace ext {
/**
 * @class Reference
 * @brief Represents a basic reference with a single character.
 */
class Reference {
 protected:
   std::string m_literal; /**< The literal representation of the reference. */

 public:
   /**
    * @brief Default constructor for a Reference object.
    */
   Reference() {}

   /**
    * @brief Constructs a Reference object with a single character literal.
    * @param literal_ The character representing the reference.
    */
   Reference(char literal_) { m_literal = literal_; }

   /**
    * @brief Sets the reference to a single character literal.
    * @param literal_ The character representing the reference.
    * @return The modified reference string.
    */
   void setReference(char literal_) { m_literal = literal_; }

   /**
    * @brief Gets the reference string with a leading hyphen.
    * @return The reference string.
    */
   virtual std::string getReference() const {
      if (!m_literal.empty()) {
         return "-" + m_literal;
      }
      return std::string();
   }

   /**
    * @brief Checks if a reference string matches the stored reference.
    * @param ref_ The reference string to compare with the stored reference.
    * @return True if the reference matches, false otherwise.
    */
   bool checkReference(std::string ref_) const {
      if (!m_literal.empty()) {
         return ref_ == getReference();
      }
      return false;
   }
};

/**
 * @class CompostReference
 * @brief Represents a reference as a string, extending the Reference class.
 */
class CompostReference : public Reference {
 public:
   /**
    * @brief Default constructor for a CompostReference object.
    */
   CompostReference() {}

   /**
    * @brief Constructs a CompostReference object with a string literal.
    * @param literal_ The string representing the reference.
    */
   CompostReference(std::string literal_) { m_literal = literal_; }

   /**
    * @brief Sets the reference to a string literal.
    * @param literal_ The string representing the reference.
    */
   void setReference(std::string literal_) { m_literal = literal_; }

   /**
    * @brief Gets the reference string with a leading double hyphen.
    * @return The reference string.
    */
   virtual std::string getReference() const {
      if (!m_literal.empty()) {
         return "--" + m_literal;
      }
      return std::string();
   }
};
} // namespace ext

#endif /// REFERENCE_HPP_
