/**
 * @file ExplorerFunctions.hpp
 * @author Pedro Lucas (pedrolucas.jsrn@gmail.com)
 * @brief Contains functions for file permissions manipulation.
 * @version 1.0
 * @date 2023-10-22
 *
 * This file defines the "permissions" function, which allows you to obtain a
 * file's permissions as a string in the format "rwxrwxrwx," where 'r'
 * represents read permission, 'w' represents write permission, and 'x'
 * represents execute permission. This function is useful for analyzing and
 * displaying file permissions.
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef EXPLORER_FUNCTIONS_HPP_
#define EXPLORER_FUNCTIONS_HPP_

#include <filesystem>
#include <string>

/**
 * @brief Namespace 'ext' for external utilities and extensions.
 */
namespace ext {
namespace fs = std::filesystem;

/**
 * @brief Get the file's permissions as a string.
 *
 * This function retrieves the permissions of a file and returns them as a
 * string in the form "rwxrwxrwx", where 'r' represents read permission, 'w'
 * represents write permission, and 'x' represents execute permission.
 *
 * @param path_ The path to the file whose permissions need to be retrieved.
 * @return A string representing the file's permissions in the format
 * "rwxrwxrwx".
 */
std::string permissions(fs::path const &path_);

} // namespace ext

#endif /// EXPLORER_FUNCTIONS_HPP_
