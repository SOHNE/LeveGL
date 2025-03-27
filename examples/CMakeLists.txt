# --------------------------------------------------------------------
# Project Setup
# --------------------------------------------------------------------
cmake_minimum_required(VERSION 3.26)
project(LeveGL_Example LANGUAGES C)

# --------------------------------------------------------------------
# Output Directories
# --------------------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# --------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------
include(../cmake/CPM.cmake)
include(../cmake/tools.cmake)

CPMAddPackage(NAME LeveGL SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
CPMAddPackage("gh:TheLartians/GroupSourcesByFolder.cmake@1.0")

# --------------------------------------------------------------------
# Find all examples files recursively
# --------------------------------------------------------------------
file(GLOB_RECURSE EXAMPLE_SOURCE_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

foreach(SOURCE_FILE ${EXAMPLE_SOURCE_FILES})
    # Get the filename without extension
    get_filename_component(EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)

    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE})

    # Link LeveGL
    target_link_libraries(${EXECUTABLE_NAME} PUBLIC LeveGL::LeveGL)

    # Platform-Specific Configuration
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
        set_property(TARGET ${EXECUTABLE_NAME} PROPERTY WIN32_EXECUTABLE true)
        target_link_options(${EXECUTABLE_NAME} PUBLIC -static -static-libgcc -static-libstdc++)
    endif()

    if(EMSCRIPTEN)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --emrun -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1 -s ALLOW_MEMORY_GROWTH")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
        set_target_properties(${EXECUTABLE_NAME} PROPERTIES SUFFIX ".html")
        # set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME "index")
    endif()

    # Group sources
    GroupSourcesByFolder(${EXECUTABLE_NAME})
endforeach()
