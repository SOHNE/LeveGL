# --------------------------------------------------------------------
# Project Setup
# --------------------------------------------------------------------
cmake_minimum_required(VERSION 3.26)
project(LeveGLUnitTests LANGUAGES C)

# --------------------------------------------------------------------
# Options
# --------------------------------------------------------------------
option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --------------------------------------------------------------------
# Include External CMake Scripts
# --------------------------------------------------------------------
include(../cmake/tools.cmake)
include(../cmake/CPM.cmake)

# --------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------
CPMAddPackage("gh:TheLartians/GroupSourcesByFolder.cmake@1.0")
CPMAddPackage(
  NAME Tau
  URL https://github.com/jasmcaus/tau/archive/refs/heads/dev.zip
  # URL_HASH 55f126525c4422b59b583c412dab38b3488a9e15e37e23d661d0612342662e24
  OPTIONS
    "TAU_BUILDINTERNALTESTS OFF"
)

if(TEST_INSTALLED_VERSION)
  find_package(LeveGL REQUIRED)
else()
  CPMAddPackage(NAME LeveGL SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

# --------------------------------------------------------------------
# Target Definition
# --------------------------------------------------------------------
set(UNIT_TESTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

add_executable(${PROJECT_NAME} ${UNIT_TESTS_SOURCES})

target_link_libraries(${PROJECT_NAME} Tau LeveGL::LeveGL)

# --------------------------------------------------------------------
# Compiler Options
# --------------------------------------------------------------------
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "GNU")
    target_compile_options(LeveGL PUBLIC -Wall -Wpedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(LeveGL PUBLIC /W4 /WX)
  endif()
endif()

# --------------------------------------------------------------------
# Testing Setup
# --------------------------------------------------------------------
enable_testing()

# --------------------------------------------------------------------
# Test Coverage
# --------------------------------------------------------------------
if(ENABLE_TEST_COVERAGE)
  target_compile_options(LeveGL PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(LeveGL PUBLIC -fprofile-arcs -ftest-coverage)
endif()

# --------------------------------------------------------------------
# Source Grouping
# --------------------------------------------------------------------
GroupSourcesByFolder(${PROJECT_NAME})
