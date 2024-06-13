include(${CMAKE_SOURCE_DIR}/env.cmake)

cmake_minimum_required(VERSION 3.14)

project(plzerow VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 8.1.1 # You can specify the version you need
)

FetchContent_MakeAvailable(fmt)

add_executable(plzerow
    src/main.cpp
    src/inputhandler.cpp
    src/token.cpp
    src/lexer.cpp
    src/parser.cpp
    src/virtual_machine.cpp
    src/chunk.cpp
    src/value.cpp
    src/debugger.cpp
    src/compiler.cpp
)

target_include_directories(plzerow PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(plzerow PRIVATE -Wall -Wextra -Wpedantic -Wno-switch)

target_link_libraries(plzerow PRIVATE fmt::fmt)
