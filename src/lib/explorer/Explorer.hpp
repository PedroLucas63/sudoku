/**
 * @file Explorer.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Contains the Explorer class for advanced file and directory
 * management.
 * @version 1.0
 * @date 2023-10-22
 *
 * This file defines the "Explorer" class, which extends the functionality of
 * the C++ Standard Library's fs::directory_entry class to provide a wide range
 * of features for managing directories and files. The "Explorer" class allows
 * for creating, removing, renaming, listing items in directories, checking disk
 * space, generating directory trees, and much more. It is a powerful tool for
 * advanced file and directory management, making it easier to work with file
 * systems in C++.
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef EXPLORER_HPP_
#define EXPLORER_HPP_

#include "ExplorerFunctions.hpp"
#include "FileHandler.hpp"
#include "List.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

/**
 * @brief Namespace 'ext' for external utilities and extensions.
 */
namespace ext {
namespace fs = std::filesystem;

/**
 * @class Explorer
 * @brief Extends the functionality of the fs::directory_entry class for file
 * and directory handling.
 *
 * The Explorer class extends the functionality of the fs::directory_entry class
 * to provide additional features for working with directories and files, such
 * as creating, removing, renaming, and listing items in directories.
 */
class Explorer : public fs::directory_entry {
 private:
   /**
    * @brief Assigns the value of a path to the Explorer object.
    *
    * @param path_ The path to assign to the Explorer object.
    * @return A reference to the modified Explorer object.
    */
   Explorer &assign(fs::path const &path_) {
      static_cast<fs::directory_entry &>(*this) = fs::directory_entry(path_);
      return *this;
   }

 public:
   /**
    * @brief Default constructor for an Explorer object.
    */
   Explorer() {}

   /**
    * @brief Constructs an Explorer object from a directory path.
    *
    * @param path_ The directory path to assign to the Explorer object.
    * @throw std::invalid_argument if the path is a file path or if it doesn't
    * exist.
    */
   Explorer(fs::path const &path_) {
      if (fs::is_directory(path_) || !fs::exists(path_)) {
         assign(path_);
      } else {
         throw std::invalid_argument(
             "Cannot create an Explorer from a file path.");
      }
   }

   /**
    * @brief Assigns a directory path to the Explorer object.
    *
    * @param path_ The directory path to assign to the Explorer object.
    * @throw std::invalid_argument if the path is a file path or if it doesn't
    * exist.
    * @return A reference to the modified Explorer object.
    */
   Explorer &operator=(fs::path const &path_) {
      if (fs::is_directory(path_) || !fs::exists(path_)) {
         assign(path_);
      } else {
         throw std::invalid_argument(
             "Cannot create an Explorer from a file path.");
      }

      return *this;
   }

   /**
    * @brief Opens an Explorer object with a directory path.
    *
    * @param path_ The directory path to assign to the Explorer object.
    * @throw std::invalid_argument if the path is a file path or if it doesn't
    * exist.
    */
   void open(fs::path const &path_) {
      if (fs::is_directory(path_) || !fs::exists(path_)) {
         assign(path_);
      } else {
         throw std::invalid_argument(
             "Cannot create an Explorer from a file path.");
      }
   }

   /**
    * @brief Checks if the directory exists.
    *
    * @return True if the directory exists, false otherwise.
    */
   bool exists() const { return fs::exists(*this); }

   /**
    * @brief Creates the directory.
    *
    * @return True if the directory was successfully created, false if it
    * already exists.
    */
   bool create() const {
      if (exists()) {
         return false;
      }

      fs::create_directory(*this);
      return true;
   }

   /**
    * @brief Removes the directory and its contents.
    */
   void remove() { fs::remove_all(*this); }

   /**
    * @brief Renames the directory.
    *
    * @param new_folder_ The new directory path.
    * @return True if the directory was successfully renamed, false if the new
    * path already exists.
    */
   bool rename(Explorer const &new_folder_) {
      if (new_folder_.exists()) {
         return false;
      }

      fs::rename(*this, new_folder_);
      return true;
   }

   /**
    * @brief Copies the directory and its contents to a new location.
    *
    * @param destiny_ The destination directory path.
    * @return True if the directory was successfully copied, false if the
    * destination already exists.
    */
   bool copy(Explorer const &destiny_) {
      if (destiny_.exists()) {
         return false;
      }

      fs::copy(*this, destiny_);
      return true;
   }

   /**
    * @brief Gets the total capacity of the file system containing the
    * directory.
    *
    * @return The total capacity in bytes.
    */
   std::uintmax_t capacity() const {
      if (!this->exists()) {
         return 0;
      }

      return fs::space(*this).capacity;
   }

   /**
    * @brief Gets the amount of free space in the file system containing the
    * directory.
    *
    * @return The amount of free space in bytes.
    */
   std::uintmax_t free() const {
      if (!this->exists()) {
         return 0;
      }

      return fs::space(*this).free;
   }

   /**
    * @brief Gets the amount of available space in the file system containing
    * the directory.
    *
    * @return The amount of available space in bytes.
    */
   std::uintmax_t available() const {
      if (!this->exists()) {
         return 0;
      }

      return fs::space(*this).available;
   }

   /**
    * @brief Generates a tree-like string representation of the directory and
    * its contents.
    *
    * @param depth_ The depth of the tree.
    * @return A string containing the directory structure.
    */
   std::string tree(size_t depth_ = 0) const {
      if (!exists()) {
         return std::string();
      }

      std::ostringstream oss;

      oss << "\33[34m" << fs::path(*this).filename().generic_string()
          << "\33[0m\n";

      std::string bars;

      for (size_t quant{0}; quant != depth_ + 1; ++quant) {
         bars += "| ";
      }

      for (auto children : fs::directory_iterator(*this)) {
         oss << bars;

         if (fs::is_directory(children)) {
            Explorer e{children};
            oss << e.tree(depth_ + 1);
         } else {
            oss << fs::path(children).filename().generic_string() << "\n";
         }
      }

      return oss.str();
   }

   /**
    * @brief Generates a list of directory and file names in the directory.
    *
    * @return A string containing the list of directory and file names.
    */
   std::string list() {
      if (!exists()) {
         return std::string();
      }

      std::ostringstream oss;

      for (auto children : fs::directory_iterator(*this)) {
         if (fs::is_directory(children)) {
            oss << "\33[34m" << fs::path(children).filename().generic_string()
                << "\33[0m\n";
         } else {
            oss << fs::path(children).filename().generic_string() << "\n";
         }
      }

      return oss.str();
   }

   /**
    * @brief Generates a list of directory and file names along with their
    * permissions in the directory.
    *
    * @return A string containing the list of directory and file names with
    * permissions.
    */
   std::string listPermissions() {
      if (!exists()) {
         return std::string();
      }

      std::ostringstream oss;

      for (auto children : fs::directory_iterator(*this)) {
         oss << permissions(children) << " ";

         if (fs::is_directory(children)) {
            oss << "\33[34m" << fs::path(children).filename().generic_string()
                << "\33[0m\n";
         } else {
            oss << fs::path(children).filename().generic_string() << "\n";
         }
      }

      return oss.str();
   }

   /**
    * @brief Retrieves the list of all files and directories in the directory
    * and its subdirectories.
    *
    * @return A List object containing the items in the directory and its
    * subdirectories.
    */
   List getChildrens() const {
      List list;

      for (auto children : fs::recursive_directory_iterator(*this)) {
         list.pushBack(children);
      }

      return list;
   }

   /**
    * @brief Retrieves the list of immediate files and directories in the
    * directory.
    *
    * @return A List object containing the immediate items in the directory.
    */
   List getImediateChildrens() const {
      List list;

      for (auto children : fs::directory_iterator(*this)) {
         list.pushBack(children);
      }

      return list;
   }

   /**
    * @brief Creates a new directory within the current directory.
    *
    * @param folder_ The name of the new directory to create.
    * @return True if the directory was successfully created, false if it
    * already exists.
    */
   bool createDirectory(Explorer const &folder_) const {
      if (folder_.exists()) {
         return false;
      }

      fs::path root = fs::path(*this);
      fs::path dir = fs::path(folder_);
      fs::path folder = root.generic_string() + "/" + dir.generic_string();
      fs::create_directory(folder);
      return true;
   }

   /**
    * @brief Renames a directory within the current directory.
    *
    * @param folder_ The directory to rename.
    * @param new_folder_ The new name for the directory.
    * @return True if the directory was successfully renamed, false if the new
    * path already exists.
    */
   bool renameDirectory(Explorer &folder_, Explorer const &new_folder_) const {
      if (new_folder_.exists()) {
         return false;
      }

      folder_.rename(new_folder_);
      return true;
   }

   /**
    * @brief Copies a directory and its contents to a new location within the
    * current directory.
    *
    * @param folder_ The directory to copy.
    * @param destiny_ The destination directory for the copy.
    * @return True if the directory was successfully copied, false if the
    * destination already exists.
    */
   bool copyDirectory(Explorer const &folder_, Explorer const &destiny_) const {
      if (destiny_.exists()) {
         return false;
      }

      fs::copy(folder_, destiny_);
      return true;
   }

   /**
    * @brief Creates a new empty file within the current directory.
    *
    * @param file_ The name of the new file to create.
    * @return True if the file was successfully created, false if it already
    * exists.
    */
   bool createFile(FileHandler const &file_) const {
      if (file_.exists()) {
         return false;
      }

      fs::path root = fs::path(*this);
      fs::path file_path = root.generic_string() + "/" + file_.generic_string();

      std::ofstream file{file_path.generic_string(), std::ios::out};
      file.close();

      return true;
   }

   /**
    * @brief Copies a file to a new location within the current directory.
    *
    * @param file_ The file to copy.
    * @param destiny_ The destination file for the copy.
    * @return True if the file was successfully copied, false if the destination
    * already exists.
    */
   bool copyFile(FileHandler const &file_, FileHandler const &destiny_) const {
      if (destiny_.exists()) {
         return false;
      }

      fs::copy_file(file_, destiny_);
      return true;
   }
};
} // namespace ext

#endif /// EXPLORER_HPP_
