# Configure window system
message(STATUS "================ Window System: ${WINDOW_SYSTEM} ================")
if(WINDOW_SYSTEM STREQUAL "GLFW")
    include(deps/glfw)
elseif(WINDOW_SYSTEM STREQUAL "SDL2")
    include(deps/sdl2)
else()
    message(FATAL_ERROR "No valid window system provided")
endif()

# Convert window system to lowercase for filename consistency
string(TOLOWER ${WINDOW_SYSTEM} WINDOW_SYSTEM_LOWER)

# Configure platform
message(STATUS "================ Platform: ${PLATFORM} ================")

# Convert platform to lowercase for filename consistency
string(TOLOWER ${PLATFORM} PLATFORM_LOWER)

# Determine platform file based on dynamic naming pattern
set(PLATFORM_FILE "le${PLATFORM_LOWER}_${WINDOW_SYSTEM_LOWER}.c")
set(PLATFORM_FILE_PATH "${LEVE_SOURCE_DIR}/platforms/${PLATFORM_FILE}")

# Verify that the platform/window system combination is supported
if(NOT EXISTS "${PLATFORM_FILE_PATH}")
    message(FATAL_ERROR "Unsupported platform-window combination: ${PLATFORM}-${WINDOW_SYSTEM}")
endif()

# Add platform file to sources
list(APPEND LEVE_SOURCE_FILES "${PLATFORM_FILE_PATH}")
