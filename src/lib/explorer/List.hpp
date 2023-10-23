/**
 * @file List.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Contains a class for managing lists of files and folders.
 * @version 0.5
 * @date 2023-10-22
 *
 * This file defines the "List" class, which provides a mechanism for managing
 * lists of files and folders. It allows for adding, removing, and accessing
 * file and folder paths and provides various utility functions for efficient
 * list management.
 *
 * The "List" class is particularly useful for handling and organizing file
 * system paths, and it supports dynamic resizing to accommodate growing lists.
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef EXPLORER_LIST_HPP_
#define EXPLORER_LIST_HPP_

#include <filesystem>
#include <stdexcept>

/**
 * @brief Namespace 'ext' for external utilities and extensions.
 */
namespace ext {
namespace fs = std::filesystem;

/**
 * @class List
 * @brief Manages lists of files and folders.
 *
 * The List class provides functionality for managing lists of file and folder
 * paths. It supports adding, removing, and accessing file and folder paths, and
 * provides utility functions for efficient list management.
 */
class List {
 private:
   fs::path *m_files;         ///< An array to store file paths.
   size_t m_files_capacity;   ///< The capacity of the list of files.
   size_t m_files_size;       ///< The current size of the list of files.
   fs::path *m_folders;       ///< An array to store folder paths.
   size_t m_folders_capacity; ///< The capacity of the list of folders.
   size_t m_folders_size;     ///< The current size of the list of folders.

   /**
    * @brief Appends a file path to the list of files.
    *
    * If the list of files is full, it will automatically increase its capacity.
    *
    * @param file_ The file path to add to the list.
    */
   void pushBackFiles(fs::path const &file_) {
      if (fullFiles()) {
         reserveFiles(m_files_capacity == 0 ? 1 : 2 * m_files_capacity);
      }

      m_files[m_files_size++] = file_;
   }

   /**
    * @brief Appends a folder path to the list of folders.
    *
    * If the list of folders is full, it will automatically increase its
    * capacity.
    *
    * @param folder_ The folder path to add to the list.
    */
   void pushBackFolders(fs::path const &folder_) {
      if (fullFolders()) {
         reserveFolders(m_folders_capacity == 0 ? 1 : 2 * m_folders_capacity);
      }

      m_folders[m_folders_size++] = folder_;
   }

 public:
   /**
    * @brief Default constructor for the List class.
    */
   List()
       : m_files_capacity(0), m_files_size(0), m_folders_capacity(0),
         m_folders_size(0) {
      m_files = new fs::path[m_files_capacity];
      m_folders = new fs::path[m_folders_capacity];
   };

   /**
    * @brief Copy constructor for the List class.
    *
    * @param other_ The List object to copy.
    */
   List(List const &other_)
       : m_files_capacity(other_.getFilesCapacity()),
         m_files_size(other_.getFilesSize()),
         m_folders_capacity(other_.getFoldersCapacity()),
         m_folders_size(other_.getFoldersSize()) {
      m_files = new fs::path[m_files_capacity];

      for (size_t index{0}; index != m_files_size; ++index) {
         m_files[index] = other_.atFiles(index);
      }

      m_folders = new fs::path[m_folders_capacity];

      for (size_t index{0}; index != m_folders_size; ++index) {
         m_folders[index] = other_.atFolders(index);
      }
   }

   /**
    * @brief Copy assignment operator for the List class.
    *
    * @param rhs_ The List object to assign to this object.
    * @return A reference to the modified List object.
    */
   List &operator=(List const &rhs_) {
      m_files_capacity = rhs_.getFilesCapacity();
      m_files_size = rhs_.getFilesSize();
      m_folders_capacity = rhs_.getFoldersCapacity();
      m_folders_size = rhs_.getFoldersSize();
      delete[] m_files;
      m_files = new fs::path[m_files_capacity];

      for (size_t index{0}; index != m_files_size; ++index) {
         m_files[index] = rhs_.atFiles(index);
      }

      delete[] m_folders;
      m_folders = new fs::path[m_folders_capacity];

      for (size_t index{0}; index != m_folders_size; ++index) {
         m_folders[index] = rhs_.atFolders(index);
      }

      return *this;
   }

   /**
    * @brief Destructor for the List class.
    */
   ~List() {
      delete[] m_files;
      delete[] m_folders;
   }

   /**
    * @brief Get the file path at a specific index in the list of files.
    *
    * @param index_ The index of the file path to retrieve.
    * @return A reference to the file path.
    * @throw std::out_of_range if the index is out of bounds.
    */
   fs::path &atFiles(size_t index_) const {
      if (index_ >= m_files_size) {
         throw std::out_of_range("The index is greater than the size.");
      }

      return m_files[index_];
   }

   /**
    * @brief Get the folder path at a specific index in the list of folders.
    *
    * @param index_ The index of the folder path to retrieve.
    * @return A reference to the folder path.
    * @throw std::out_of_range if the index is out of bounds.
    */
   fs::path &atFolders(size_t index_) const {
      if (index_ >= m_folders_size) {
         throw std::out_of_range("The index is greater than the size.");
      }

      return m_folders[index_];
   }

   /**
    * @brief Get the capacity of the list of files.
    *
    * @return The capacity of the list of files.
    */
   size_t getFilesCapacity() const { return m_files_capacity; }

   /**
    * @brief Get the current size of the list of files.
    *
    * @return The current size of the list of files.
    */
   size_t getFilesSize() const { return m_files_size; }

   /**
    * @brief Get the capacity of the list of folders.
    *
    * @return The capacity of the list of folders.
    */
   size_t getFoldersCapacity() const { return m_folders_capacity; }

   /**
    * @brief Get the current size of the list of folders.
    *
    * @return The current size of the list of folders.
    */
   size_t getFoldersSize() const { return m_folders_size; }

   /**
    * @brief Check if the list of files is full.
    *
    * @return true if the list of files is full; false otherwise.
    */
   bool fullFiles() const { return m_files_capacity == m_files_size; }

   /**
    * @brief Check if the list of files is empty.
    *
    * @return true if the list of files is empty; false otherwise.
    */
   bool emptyFiles() const { return m_files_size == 0; }

   /**
    * @brief Check if the list of folders is full.
    *
    * @return true if the list of folders is full; false otherwise.
    */
   bool fullFolders() const { return m_folders_capacity == m_folders_size; }

   /**
    * @brief Check if the list of folders is empty.
    *
    * @return true if the list of folders is empty; false otherwise.
    */
   bool emptyFolders() const { return m_folders_size == 0; }

   /**
    * @brief Get a pointer to the beginning of the list of files.
    *
    * @return A pointer to the beginning of the list of files.
    */
   fs::path *beginFiles() { return m_files; }

   /**
    * @brief Get a pointer to the end of the list of files.
    *
    * @return A pointer to the end of the list of files.
    */
   fs::path *endFiles() { return m_files + m_files_size; }

   /**
    * @brief Get a pointer to the beginning of the list of folders.
    *
    * @return A pointer to the beginning of the list of folders.
    */
   fs::path *beginFolders() { return m_folders; }

   /**
    * @brief Get a pointer to the end of the list of folders.
    *
    * @return A pointer to the end of the list of folders.
    */
   fs::path *endFolders() { return m_folders + m_folders_size; }

   /**
    * @brief Reserve capacity for the list of files.
    *
    * If the new capacity is greater than the current capacity, it will allocate
    * memory and copy the existing elements.
    *
    * @param new_capacity_ The new capacity to reserve.
    */
   void reserveFiles(size_t new_capacity_) {
      if (new_capacity_ <= m_files_capacity) {
         return;
      }

      fs::path *temp = new fs::path[new_capacity_];

      for (size_t index{0}; index != m_files_size; ++index) {
         temp[index] = m_files[index];
      }

      m_files_capacity = new_capacity_;
      delete[] m_files;
      m_files = temp;
   }

   /**
    * @brief Reserve capacity for the list of folders.
    *
    * If the new capacity is greater than the current capacity, it will allocate
    * memory and copy the existing elements.
    *
    * @param new_capacity_ The new capacity to reserve.
    */
   void reserveFolders(size_t new_capacity_) {
      if (new_capacity_ <= m_folders_capacity) {
         return;
      }

      fs::path *temp = new fs::path[new_capacity_];

      for (size_t index{0}; index != m_folders_size; ++index) {
         temp[index] = m_folders[index];
      }

      m_folders_capacity = new_capacity_;
      delete[] m_folders;
      m_folders = temp;
   }

   /**
    * @brief Add a file or folder path to the list, depending on the type of
    * path.
    *
    * If the path is a regular file, it will be added to the list of files. If
    * it's a directory, it will be added to the list of folders.
    *
    * @param path_ The file or folder path to add to the list.
    */
   void pushBack(fs::path const &path_) {
      if (fs::is_regular_file(path_)) {
         pushBackFiles(path_);
      } else if (fs::is_directory(path_)) {
         pushBackFolders(path_);
      }
   }

   /**
    * @brief Remove the last file path from the list of files.
    */
   void popBackFiles() {
      if (!emptyFiles()) {
         --m_files_size;
      }
   }

   /**
    * @brief Remove the last folder path from the list of folders.
    */
   void popBackFolders() {
      if (!emptyFolders()) {
         --m_folders_size;
      }
   }
};
} // namespace ext

#endif /// EXPLORER_LIST_HPP_
