cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(
  LeveGL
  VERSION 25.0.1
  DESCRIPTION "A simple rendering engine"
  LANGUAGES C
  HOMEPAGE_URL "https://github.com/SOHNE/LeveGL"
)

message(STATUS "=========================================================================")
message(STATUS " Copyright (c)         2024-2025         SOHNE, Leandro Peres (@zschzen) ")
message(STATUS "=========================================================================")
message(STATUS "                                                                         ")
message(STATUS " ooooo                                          .oooooo.    ooooo        ")
message(STATUS " `888'                                         d8P'  `Y8b   `888'        ")
message(STATUS "  888          .ooooo.  oooo    ooo  .ooooo.  888            888         ")
message(STATUS "  888         d88' `88b  `88.  .8'  d88' `88b 888            888         ")
message(STATUS "  888         888ooo888   `88..8'   888ooo888 888     ooooo  888         ")
message(STATUS "  888       o 888    .o    `888'    888    .o `88.    .88'   888       o ")
message(STATUS " o888ooooood8 `Y8bod8P'     `8'     `Y8bod8P'  `Y8bood8P'   o888ooooood8 ")
message(STATUS " v${PROJECT_VERSION}                                                     ")
message(STATUS "                                                                         ")
message(STATUS "=========================================================================")
message(STATUS " https://sohne.github.io/LeveGL                     zlib/libpng licensed ")

# Determine if LuGL is the main project
set(LEVE_IS_MAIN ${PROJECT_IS_TOP_LEVEL})

# --------------------------------------------------------------------
# Paths
# --------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# --------------------------------------------------------------------
# Options
# --------------------------------------------------------------------
include(CMakeOptions.cmake)

# --------------------------------------------------------------------
# Output Directories
# --------------------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# --------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------
message(STATUS "========================================================================= ")
message(STATUS "> CPM.cmake")

include(cmake/CPM.cmake)
CPMUsePackageLock(package-lock.cmake)

CPMAddPackage("gh:cpm-cmake/CPMLicenses.cmake@0.0.7")
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.13.0")
CPMAddPackage("gh:TheLartians/GroupSourcesByFolder.cmake@1.0")

include(cmake/tools.cmake)

set(LEVE_INCLUDE_DEPS_DIR "")
set(LEVE_LINK_DEPS "")

include(cmake/deps/glew.cmake)
include(cmake/deps/glfw.cmake)

list(APPEND LEVE_INCLUDE_DEPS_DIR "${PROJECT_SOURCE_DIR}/external")

# --------------------------------------------------------------------
# Global Properties
# --------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# --------------------------------------------------------------------
# Subdirectories
# --------------------------------------------------------------------
add_subdirectory(src levegl)

# --------------------------------------------------------------------
# Installation Configuration
# --------------------------------------------------------------------
string(TOLOWER ${PROJECT_NAME}/leversion.h VERSION_HEADER_LOCATION)
string(TOLOWER ${PROJECT_NAME}/leexport.h EXPORT_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  EXPORT_HEADER "${EXPORT_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES "Ccache 1.2.5;GLEW glew-cmake-2.2.0;GLFW 3.4"
)

unset(VERSION_HEADER_LOCATION)
unset(EXPORT_HEADER_LOCATION)

# --------------------------------------------------------------------
# License collector
# --------------------------------------------------------------------
# Usage: --build build --target write-licenses
cpm_licenses_create_disclaimer_target(
  write-licenses "${CMAKE_CURRENT_BINARY_DIR}/third_party.txt" "${CPM_PACKAGES}"
)

message(STATUS "========================================================================= ")

