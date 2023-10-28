/**
 * @file Cli.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Command-Line Interface (CLI) Utility for Argument Parsing
 * @version 1.0
 * @date 2023-10-22
 *
 * This C++ project provides a flexible and extensible Command-Line Interface
 * (CLI) utility for parsing command-line arguments. It allows developers to
 * define various types of command-line arguments, such as simple flags, text,
 * integers, floating-point numbers, file paths, and lists. The utility handles
 * argument validation and stores parsed values in an organized manner.
 *
 * Features:
 * - Define and manage command-line arguments with short and long references.
 * - Support for simple flags, text, integers, floats, input and output file
 * paths, and lists.
 * - Validate and store argument values while handling incorrect input
 * gracefully.
 * - Easily access parsed values and non-matching arguments.
 *
 * Usage:
 * 1. Include the CLI utility in your C++ project.
 * 2. Define the desired command-line arguments and their types.
 * 3. Call the `parse` function to process command-line arguments.
 * 4. Retrieve argument values using specific access functions.
 *
 * Example:
 * ```
 * // Create and configure the CLI object with various argument types.
 * CLI cli;
 * cli.addArgument('h', "help");        // Simple flag for help.
 * cli.addArgumentText('n', "name");    // Text argument for a name.
 * cli.addArgumentInteger('c', "count");// Integer argument for a count.
 *
 * // Parse command-line arguments.
 * int argc = ...;
 * char** argv = ...;
 * cli.parse(argc, argv);
 *
 * // Access parsed argument values.
 * if (cli.receiveSimple("help")) {
 *   // Handle the help flag.
 * }
 * std::string name = cli.receiveText("name");
 * int count = cli.receiveInteger("count");
 * ```
 *
 * This CLI utility simplifies the process of handling command-line arguments in
 * C++ applications.
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CLI_HPP_
#define CLI_HPP_

#include "Argument.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Namespace 'ext' for external utilities and extensions.
 */
namespace ext {
/**
 * @brief The CLI class for parsing command-line arguments.
 */
class CLI {
 private:
   std::vector<std::unique_ptr<Argument>>
       m_arguments; /**< Vector of unique pointers to Argument objects. */
   std::map<Argument *, bool>
       m_values_simple; /**< Map to store simple argument values. */
   std::map<Argument *, std::string>
       m_values_text; /**< Map to store text argument values. */
   std::map<Argument *, int>
       m_values_int; /**< Map to store integer argument values. */
   std::map<Argument *, float>
       m_values_float; /**< Map to store floating-point argument values. */
   std::map<ArgumentInFile *, std::string>
       m_values_infile; /**< Map to store input file argument values. */
   std::map<ArgumentOutFile *, std::string>
       m_values_outfile; /**< Map to store output file argument values. */
   std::map<ArgumentList *, std::vector<std::string>>
       m_values_list; /**< Map to store list argument values. */
   std::vector<std::string>
       m_buffer; /**< Buffer to store non-matching arguments. */

   /**
    * @brief Helper function to check if an argument exists.
    * @param argument_ The argument to check.
    * @return A pointer to the matching Argument object, or nullptr if not
    * found.
    */
   Argument *checkArgument(std::string const &argument_) const {
      for (auto &arg : m_arguments) {
         if (arg->checkReferences(argument_)) {
            return arg.get();
         }
      }

      return nullptr;
   }

   /**
    * @brief Helper function to parse and store text argument values.
    * @param arg_ Pointer to the ArgumentText object.
    * @param next_ The next command-line argument.
    * @return True if the argument is successfully parsed and stored, otherwise
    * false.
    */
   bool getText(ArgumentText *arg_, char *next_) {
      if (arg_->validateData(next_)) {
         m_values_text[arg_] = next_;
         return true;
      }

      return false;
   }

   /**
    * @brief Helper function to parse and store integer argument values.
    * @param arg_ Pointer to the ArgumentInteger object.
    * @param next_ The next command-line argument.
    * @return True if the argument is successfully parsed and stored, otherwise
    * false.
    */
   bool getInteger(ArgumentInteger *arg_, char *next_) {
      if (arg_->validateData(next_)) {
         int converted{std::stoi(next_)};
         m_values_int[arg_] = converted;
         return true;
      }

      return false;
   }

   /**
    * @brief Helper function to parse and store floating-point argument values.
    * @param arg_ Pointer to the ArgumentFloating object.
    * @param next_ The next command-line argument.
    * @return True if the argument is successfully parsed and stored, otherwise
    * false.
    */
   bool getFloating(ArgumentFloating *arg_, char *next_) {
      if (arg_->validateData(next_)) {
         float converted{std::stof(next_)};
         m_values_float[arg_] = converted;
         return true;
      }

      return false;
   }

   /**
    * @brief Helper function to parse and store input file argument values.
    * @param arg_ Pointer to the ArgumentInFile object.
    * @param next_ The next command-line argument.
    * @return True if the argument is successfully parsed and stored, otherwise
    * false.
    */
   bool getInFile(ArgumentInFile *arg_, char *next_) {
      if (arg_->validateData(next_)) {
         m_values_infile[arg_] = next_;
         return true;
      }

      return false;
   }

   /**
    * @brief Helper function to parse and store output file argument values.
    * @param arg_ Pointer to the ArgumentOutFile object.
    * @param next_ The next command-line argument.
    * @return True if the argument is successfully parsed and stored, otherwise
    * false.
    */
   bool getOutFile(ArgumentOutFile *arg_, char *next_) {
      if (arg_->validateData(next_)) {
         m_values_outfile[arg_] = next_;
         return true;
      }

      return false;
   }

   /**
    * @brief Helper function to parse and store list argument values.
    * @param arg_ Pointer to the ArgumentList object.
    * @param next_ The next command-line argument.
    * @return True if the argument is successfully parsed and stored, otherwise
    * false.
    */
   bool getlist(ArgumentList *arg_, char *next_) {
      if (arg_->validateData(next_)) {
         m_values_list[arg_].push_back(next_);
         return true;
      }

      return false;
   }

   /**
    * @brief Helper function to parse and store simple argument values.
    * @param arg_ Pointer to the Argument object.
    * @return Always returns true for simple arguments.
    */
   bool getSimple(Argument *arg_) {
      m_values_simple[arg_] = true;
      return true;
   }

 public:
   /**
    * @brief Default constructor for the CLI class.
    */
   CLI() {}

   /**
    * @brief Add a simple argument without a value.
    * @param ref_ The short reference character for the argument.
    * @param c_ref_ The long reference string for the argument (optional).
    */
   void addArgument(char ref_, std::string c_ref_ = std::string(), bool default_ = false) {
      m_arguments.push_back(std::make_unique<Argument>(ref_, c_ref_));
      m_values_simple[m_arguments.back().get()] = default_;
   }

   /**
    * @brief Add a text argument with a value.
    * @param ref_ The short reference character for the argument.
    * @param c_ref_ The long reference string for the argument (optional).
    */
   void addArgumentText(char ref_, std::string c_ref_ = std::string(), std::string default_ = std::string()) {
      m_arguments.push_back(std::make_unique<ArgumentText>(ref_, c_ref_));
      m_values_text[m_arguments.back().get()] = default_;
   }

   /**
    * @brief Add an integer argument with a value.
    * @param ref_ The short reference character for the argument.
    * @param c_ref_ The long reference string for the argument (optional).
    */
   void addArgumentInteger(char ref_, std::string c_ref_ = std::string(), int default_ = int()) {
      m_arguments.push_back(std::make_unique<ArgumentInteger>(ref_, c_ref_));
      m_values_int[m_arguments.back().get()] = default_;
   }

   /**
    * @brief Add a floating-point argument with a value.
    * @param ref_ The short reference character for the argument.
    * @param c_ref_ The long reference string for the argument (optional).
    */
   void addArgumentFloating(char ref_, std::string c_ref_ = std::string(), float default_ = float()) {
      m_arguments.push_back(std::make_unique<ArgumentFloating>(ref_, c_ref_));
      m_values_float[m_arguments.back().get()] = default_;
   }

   /**
    * @brief Add an input file argument with a value.
    * @param ref_ The short reference character for the argument.
    * @param c_ref_ The long reference string for the argument (optional).
    * @param extensions_ A list of valid file extensions for this argument
    * (optional).
    */
   void addArgumentInFile(
       char ref_, std::string c_ref_ = std::string(),
       std::initializer_list<std::string> const &extensions_ = {}, std::string default_ = std::string()) {
      m_arguments.push_back(
          std::make_unique<ArgumentInFile>(ref_, c_ref_, extensions_));
      m_values_infile[static_cast<ArgumentInFile *>(m_arguments.back().get())] =
          default_;
   }

   /**
    * @brief Add an output file argument with a value.
    * @param ref_ The short reference character for the argument.
    * @param c_ref_ The long reference string for the argument (optional).
    * @param extensions_ A list of valid file extensions for this argument
    * (optional).
    */
   void addArgumentOutFile(
       char ref_, std::string c_ref_ = std::string(),
       std::initializer_list<std::string> const &extensions_ = {}, std::string default_ = std::string()) {
      m_arguments.push_back(
          std::make_unique<ArgumentOutFile>(ref_, c_ref_, extensions_));
      m_values_outfile[static_cast<ArgumentOutFile *>(
          m_arguments.back().get())] = default_;
   }

   /**
    * @brief Add a list argument with multiple values.
    * @param ref_ The short reference character for the argument.
    * @param c_ref_ The long reference string for the argument (optional).
    * @param options_ A list of valid options for this argument (optional).
    */
   void
   addArgumentList(char ref_, std::string c_ref_ = std::string(),
                   std::initializer_list<std::string> const &options_ = {}) {
      m_arguments.push_back(
          std::make_unique<ArgumentList>(ref_, c_ref_, options_));
      m_values_list[static_cast<ArgumentList *>(m_arguments.back().get())] = {};
   }

   /**
    * @brief Parse command-line arguments and store their values.
    *
    * This function processes the command-line arguments provided to the program
    * and stores their values according to the predefined argument types
    * (simple, text, integer, float, file, list). It also captures non-matching
    * arguments in the buffer.
    *
    * @param argc_ The number of command-line arguments.
    * @param argv_ An array of C-style strings representing command-line
    * arguments.
    */
   void parse(int argc_, char *argv_[]) {
      for (int index{1}; index < argc_; ++index) {
         Argument *argument{checkArgument(argv_[index])};

         if (argument == nullptr) {
            m_buffer.push_back(argv_[index]);
         } else if (ArgumentText *textArg =
                        dynamic_cast<ArgumentText *>(argument)) {
            if (index + 1 < argc_) {
               if (getText(textArg, argv_[index + 1])) {
                  ++index;
               }
            }
         } else if (ArgumentInteger *intArg =
                        dynamic_cast<ArgumentInteger *>(argument)) {
            if (index + 1 < argc_) {
               if (getInteger(intArg, argv_[index + 1])) {
                  ++index;
               }
            }
         } else if (ArgumentFloating *floatArg =
                        dynamic_cast<ArgumentFloating *>(argument)) {
            if (index + 1 < argc_) {
               if (getFloating(floatArg, argv_[index + 1])) {
                  ++index;
               }
            }
         } else if (ArgumentInFile *inputFileArg =
                        dynamic_cast<ArgumentInFile *>(argument)) {
            if (index + 1 < argc_) {
               if (getInFile(inputFileArg, argv_[index + 1])) {
                  ++index;
               }
            }
         } else if (ArgumentOutFile *outputFileArg =
                        dynamic_cast<ArgumentOutFile *>(argument)) {
            if (index + 1 < argc_) {
               if (getOutFile(outputFileArg, argv_[index + 1])) {
                  ++index;
               }
            }
         } else if (ArgumentList *listArg =
                        dynamic_cast<ArgumentList *>(argument)) {
            while (index + 1 < argc_) {
               if (getlist(listArg, argv_[index + 1])) {
                  ++index;
               } else {
                  break;
               }
            }
         } else {
            getSimple(argument);
         }
      }
   }

   /**
    * @brief Check if a simple argument is received.
    * @param argument_ The reference character or string of the argument.
    * @return True if the simple argument is received, otherwise false.
    */
   bool receiveSimple(std::string argument_) const {
      for (auto [arg, value] : m_values_simple) {
         if (arg->checkReferences(argument_)) {
            return value;
         }
      }

      return false;
   }

   /**
    * @brief Get the value of a text argument.
    * @param argument_ The reference character or string of the argument.
    * @return The value of the text argument as a string.
    */
   std::string receiveText(std::string argument_) const {
      for (auto [arg, value] : m_values_text) {
         if (arg->checkReferences(argument_)) {
            return value;
         }
      }

      return std::string();
   }

   /**
    * @brief Get the value of an integer argument.
    * @param argument_ The reference character or string of the argument.
    * @return The value of the integer argument as an integer.
    */
   int receiveInteger(std::string argument_) const {
      for (auto [arg, value] : m_values_int) {
         if (arg->checkReferences(argument_)) {
            return value;
         }
      }

      return int();
   }

   /**
    * @brief Get the value of a floating-point argument.
    * @param argument_ The reference character or string of the argument.
    * @return The value of the floating-point argument as a float.
    */
   float receiveFloating(std::string argument_) const {
      for (auto [arg, value] : m_values_float) {
         if (arg->checkReferences(argument_)) {
            return value;
         }
      }

      return float();
   }

   /**
    * @brief Get the value of an input file argument.
    * @param argument_ The reference character or string of the argument.
    * @return The value of the input file argument as a string.
    */
   std::string receiveInFile(std::string argument_) const {
      for (auto [arg, value] : m_values_infile) {
         if (arg->checkReferences(argument_)) {
            return value;
         }
      }

      return std::string();
   }

   /**
    * @brief Get the value of an output file argument.
    * @param argument_ The reference character or string of the argument.
    * @return The value of the output file argument as a string.
    */
   std::string receiveOutFile(std::string argument_) const {
      for (auto [arg, value] : m_values_outfile) {
         if (arg->checkReferences(argument_)) {
            return value;
         }
      }

      return std::string();
   }

   /**
    * @brief Get the values of a list argument.
    * @param argument_ The reference character or string of the argument.
    * @return A vector of strings containing the list argument values.
    */
   std::vector<std::string> receiveList(std::string argument_) const {
      for (auto [arg, value] : m_values_list) {
         if (arg->checkReferences(argument_)) {
            return value;
         }
      }

      return {};
   }

   /**
    * @brief Retrieve the non-matching command-line arguments stored in the
    * buffer.
    * @return A vector of strings containing non-matching arguments.
    */
   std::vector<std::string> receiveBuffer() const { return m_buffer; }
};
} // namespace ext

#endif /// CLI_HPP_