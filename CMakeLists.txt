cmake_minimum_required(VERSION 3.0.0)
project(sudoku VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

configure_file(config/SudokuConfig.h.in SudokuConfig.h)
include_directories(sudoku PUBLIC "${PROJECT_BINARY_DIR}"
"src/controller"
"src/lib"
"src/view"
"src/model"
"src/classes"
"src/functions")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(sudoku src/main.cpp
src/controller/GameController.cpp
src/view/View.cpp)

add_executable(convert src/convert_data.cpp)
add_executable(test_draw src/test.cpp
src/classes/Sudoku.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
