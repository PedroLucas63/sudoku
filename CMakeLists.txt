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
src/view/View.cpp
src/classes/Sudoku.cpp
src/classes/Save.cpp)

add_executable(convert src/convert.cpp
src/classes/Sudoku.cpp
src/functions/Validate.cpp)

add_executable(reading src/reading.cpp
src/classes/Sudoku.cpp)

add_executable(savefile src/savefile.cpp
src/classes/Sudoku.cpp
src/classes/Save.cpp)

add_executable(loadfile src/loadfile.cpp
src/classes/Sudoku.cpp
src/classes/Save.cpp
src/functions/Validate.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
