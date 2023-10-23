/**
 * @file Argument.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Contains classes for managing command-line arguments and their
 * validations.
 * @version 1.0
 * @date 2023-10-22
 *
 *  This file defines a set of classes for managing and validating command-line
 * arguments. The base class "Argument" provides a framework for managing
 * references and offers a common interface for data validation. Subclasses
 * extend the base class to handle specific data types and validation rules.
 *
 * The "Argument" class can handle references, while its subclasses
 * "ArgumentInteger," "ArgumentFloating," "ArgumentText," "ArgumentIFile,"
 * "ArgumentOFile," and "ArgumentList" provide specific validation rules for
 * integer values, floating-point values, text, input files, output files, and
 * lists of options.
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ARGUMENT_HPP_
#define ARGUMENT_HPP_

#include "Reference.hpp"
#include <filesystem>
#include <string>
#include <vector>

/**
 * @brief Namespace 'ext' for external utilities and extensions.
 */
namespace ext {
/**
 * @class Argument
 * @brief Base class for managing command-line arguments and references.
 */
class Argument {
 protected:
   Reference m_reference;                /**< The single-character reference. */
   CompostReference m_compost_reference; /**< The string reference. */

 public:
   /**
    * @brief Constructs an Argument object with default references.
    * @param ref_ The single-character reference.
    * @param c_ref_ The string reference.
    */
   Argument(Reference const &ref_ = Reference(),
            CompostReference const &c_ref_ = CompostReference()) {
      m_reference = ref_;
      m_compost_reference = c_ref_;
   }

   /**
    * @brief Sets the single-character reference.
    * @param ref_ The single-character reference to set.
    */
   void setReference(Reference const &ref_) { m_reference = ref_; }

   /**
    * @brief Sets the string reference.
    * @param c_ref_ The string reference to set.
    */
   void setCompostReference(CompostReference const &c_ref_) {
      m_compost_reference = c_ref_;
   }

   std::string getReference() const {
      return m_reference.getReference();
   }
   
   /**
    * @brief Checks if a given string reference matches either the
    * single-character or string reference.
    * @param ref_ The string reference to check.
    * @return True if the reference matches, false otherwise.
    */
   bool checkReferences(std::string const &ref_) const {
      return m_reference.checkReference(ref_) ||
             m_compost_reference.checkReference(ref_);
   }

   /**
    * @brief Validates data for a specific Argument type.
    * @param val_ The data to validate.
    * @return True if the data is valid, false otherwise.
    */
   virtual bool validateData(std::string const &val_) const { return true; }
};

/**
 * @class ArgumentInteger
 * @brief A subclass of Argument for validating integer arguments.
 */
class ArgumentInteger : public Argument {
 public:
   /**
    * @brief Constructs an ArgumentInteger object.
    * @param ref_ A Reference object.
    * @param c_ref_ A CompostReference object.
    */
   ArgumentInteger(Reference const &ref_ = Reference(),
                   CompostReference const &c_ref_ = CompostReference())
       : Argument(ref_, c_ref_) {}

   /**
    * @brief Validates whether the input string represents a valid integer.
    * @param val_ The string to be validated.
    * @return True if the string represents a valid integer, false otherwise.
    */
   bool validateData(std::string const &val_) const {
      try {
         size_t pos;
         int data{std::stoi(val_, &pos)};

         if (pos == val_.size()) {
            return true;
         }

         return false;
      } catch (...) {
         return false;
      }
   }
};

/**
 * @class ArgumentFloating
 * @brief A subclass of Argument for validating floating-point arguments.
 */
class ArgumentFloating : public Argument {
 public:
   /**
    * @brief Constructs an ArgumentFloating object.
    * @param ref_ A Reference object.
    * @param c_ref_ A CompostReference object.
    */
   ArgumentFloating(Reference const &ref_ = Reference(),
                    CompostReference const &c_ref_ = CompostReference())
       : Argument(ref_, c_ref_) {}

   /**
    * @brief Validates whether the input string represents a valid
    * floating-point number.
    * @param val_ The string to be validated.
    * @return True if the string represents a valid floating-point number, false
    * otherwise.
    */
   bool validateData(std::string const &val_) const {
      try {
         size_t pos;
         float data{std::stof(val_, &pos)};

         if (pos == val_.size()) {
            return true;
         }

         return false;
      } catch (...) {
         return false;
      }
   }
};

/**
 * @class ArgumentText
 * @brief A subclass of Argument for validating text arguments.
 */
class ArgumentText : public Argument {
 public:
   /**
    * @brief Constructs an ArgumentText object.
    * @param ref_ A Reference object.
    * @param c_ref_ A CompostReference object.
    */
   ArgumentText(Reference const &ref_ = Reference(),
                CompostReference const &c_ref_ = CompostReference())
       : Argument(ref_, c_ref_) {}

   /**
    * @brief Validates whether the input string is not empty.
    * @param val_ The string to be validated.
    * @return True if the string is not empty, false if it is empty.
    */
   bool validateData(std::string const &val_) const { return !val_.empty(); }
};

/**
 * @class ArgumentInFile
 * @brief A subclass of Argument for validating input file arguments with
 * specific extensions.
 */
class ArgumentInFile : public Argument {
 protected:
   std::vector<std::string>
       m_extensions; ///< The list of valid file extensions.

   /**
    * @brief Validates whether the given file extension is in the list of valid
    * extensions.
    * @param extension_ The file extension to be validated.
    * @return True if the extension is valid, false if it is not.
    */
   bool validateExtension(std::string const &extension_) const {
      if (m_extensions.empty()) {
         return true;
      }

      for (std::string ext : m_extensions) {
         if (ext == extension_) {
            return true;
         }
      }

      return false;
   }

 public:
   /**
    * @brief Constructs an ArgumentInFile object.
    * @param ref_ A Reference object.
    * @param c_ref_ A CompostReference object.
    * @param extensions_ An initializer list of valid file extensions.
    */
   ArgumentInFile(Reference const &ref_ = Reference(),
                  CompostReference const &c_ref_ = CompostReference(),
                  std::initializer_list<std::string> const &extensions_ = {})
       : Argument(ref_, c_ref_) {
      for (std::string ext : extensions_) {
         m_extensions.push_back(ext);
      }
   }

   /**
    * @brief Adds a valid file extension to the list of extensions.
    * @param extension_ The file extension to be added.
    */
   void addExtension(std::string const &extension_) {
      if (!extension_.empty()) {
         m_extensions.push_back(extension_);
      }
   }

   /**
    * @brief Validates whether the input string represents an existing file with
    * a valid extension.
    * @param val_ The string to be validated as a file path.
    * @return True if the file exists and has a valid extension, false
    * otherwise.
    */
   bool validateData(std::string const &val_) const {
      std::filesystem::path file{val_};

      return std::filesystem::exists(file) &&
             validateExtension(file.extension().generic_string());
   }
};

/**
 * @class ArgumentOutFile
 * @brief A subclass of Argument for validating output file arguments with
 * specific extensions.
 */
class ArgumentOutFile : public Argument {
 protected:
   std::vector<std::string>
       m_extensions; ///< The list of valid file extensions.

   /**
    * @brief Validates whether the given file extension is in the list of valid
    * extensions.
    * @param extension_ The file extension to be validated.
    * @return True if the extension is valid, false if it is not.
    */
   bool validateExtension(std::string const &extension_) const {
      if (m_extensions.empty()) {
         return true;
      }
      
      for (std::string ext : m_extensions) {
         if (ext == extension_) {
            return true;
         }
      }

      return false;
   }

 public:
   /**
    * @brief Constructs an ArgumentOutFile object.
    * @param ref_ A Reference object.
    * @param c_ref_ A CompostReference object.
    * @param extensions_ An initializer list of valid file extensions.
    */
   ArgumentOutFile(Reference const &ref_ = Reference(),
                   CompostReference const &c_ref_ = CompostReference(),
                   std::initializer_list<std::string> const &extensions_ = {})
       : Argument(ref_, c_ref_) {
      for (std::string ext : extensions_) {
         m_extensions.push_back(ext);
      }
   }

   /**
    * @brief Adds a valid file extension to the list of extensions.
    * @param extension_ The file extension to be added.
    */
   void addExtension(std::string const &extension_) {
      if (!extension_.empty()) {
         m_extensions.push_back(extension_);
      }
   }

   /**
    * @brief Validates whether the input string represents a valid file
    * extension.
    * @param val_ The string to be validated as a file extension.
    * @return True if the extension is valid, false otherwise.
    */
   bool validateData(std::string const &val_) const {
      std::filesystem::path file{val_};

      return validateExtension(file.extension().generic_string());
   }
};

/**
 * @class ArgumentList
 * @brief A subclass of Argument for validating list arguments with specific
 * options.
 */
class ArgumentList : public Argument {
 protected:
   std::vector<std::string> m_options; ///< The list of valid options.

   /**
    * @brief Validates whether the given option is in the list of valid options.
    * @param option_ The option to be validated.
    * @return True if the option is valid, false if it is not.
    */
   bool validateOption(std::string const &option_) const {
      for (std::string opt : m_options) {
         if (opt == option_) {
            return true;
         }
      }

      return false;
   }

 public:
   /**
    * @brief Constructs an ArgumentList object.
    * @param ref_ A Reference object.
    * @param c_ref_ A CompostReference object.
    * @param options_ An initializer list of valid options.
    */
   ArgumentList(Reference const &ref_ = Reference(),
                CompostReference const &c_ref_ = CompostReference(),
                std::initializer_list<std::string> const &options_ = {})
       : Argument(ref_, c_ref_) {
      for (std::string opt : options_) {
         m_options.push_back(opt);
      }
   }

   /**
    * @brief Adds a valid option to the list of options.
    * @param option_ The option to be added.
    */
   void addOption(std::string const &option_) {
      if (!option_.empty()) {
         m_options.push_back(option_);
      }
   }

   /**
    * @brief Validates whether the input string represents a valid option.
    * @param val_ The string to be validated as an option.
    * @return True if the option is valid, false otherwise.
    */
   bool validateData(std::string const &val_) const {
      return validateOption(val_);
   }
};
} // namespace ext

#endif /// ARGUMENT_HPP_