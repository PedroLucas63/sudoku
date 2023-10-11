cmake_minimum_required(VERSION 3.0.0)
project(sudoku VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

configure_file(config/SudokuConfig.h.in SudokuConfig.h)
include_directories(sudoku PUBLIC "${PROJECT_BINARY_DIR}"
"src/classes"
"src/libs"
"src/views")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall")

add_executable(sudoku src/main.cpp
   src/classes/Action.cpp
   src/classes/GameController.cpp
   src/classes/SaveGame.cpp
   src/classes/SudokuGame.cpp
   src/views/View.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
