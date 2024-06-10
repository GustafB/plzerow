cmake_minimum_required(VERSION 3.14)

project(plzerow VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(plzerow
    src/main.cpp
    src/filehandler.cpp
    src/lexeme.cpp
    src/lexer.cpp
    src/parser.cpp
)

target_include_directories(plzerow PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(plzerow PRIVATE -Wall -Wextra -Wpedantic)


