include(OpenGLConfigurations)

#--------------------------------------------------------------------
# Source and Header Files
#--------------------------------------------------------------------
set(LEVE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/levegl)
set(LEVE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

list(APPEND LEVE_HEADER_FILES
  ${LEVE_INCLUDE_DIR}/levegl.h
  ${LEVE_INCLUDE_DIR}/leapi.h

  ${LEVE_SOURCE_DIR}/lecore_context.h
)

list(APPEND LEVE_SOURCE_FILES
  # Modules
  ${LEVE_SOURCE_DIR}/lecore.c
  ${LEVE_SOURCE_DIR}/leshader.c
  ${LEVE_SOURCE_DIR}/leshapes.c
)

# Add platform-specific
if(PLATFORM STREQUAL "Desktop")
  list(APPEND LEVE_SOURCE_FILES ${LEVE_SOURCE_DIR}/platforms/leglfw.c)
endif()

#--------------------------------------------------------------------
# Library Setup
#--------------------------------------------------------------------
add_library(${PROJECT_NAME} ${LEVE_SOURCE_FILES} ${LEVE_HEADER_FILES})

# Link libraries
setup_opengl_target(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LEVE_LINK_DEPS})

# Sanitizers
add_sanitizers(${PROJECT_NAME})

#--------------------------------------------------------------------
# Include Directories
#--------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
  PRIVATE
    ${LEVE_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#--------------------------------------------------------------------
# Target Properties
#--------------------------------------------------------------------
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        PUBLIC_HEADER ${LEVE_INCLUDE_DIR}/levegl.h
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        C_EXTENSIONS OFF
        C_STANDARD 99
        C_STANDARD_REQUIRED ON
)

#--------------------------------------------------------------------
# Compile definitions
#--------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(${PROJECT_NAME} PUBLIC LE_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_definitions(${PROJECT_NAME} PUBLIC LE_RELEASE)
endif()

if(BUILD_SHARED_LIBS)
  message(STATUS "Building shared library")
  target_compile_definitions(${PROJECT_NAME} PUBLIC LEVEGL_SHARED_DEFINE)
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC LEVEGL_STATIC_DEFINE)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC "${PLATFORM_BACKEND}")
target_compile_definitions(${PROJECT_NAME} PUBLIC "${GRAPHICS}")

#--------------------------------------------------------------------
# Source Groups
#--------------------------------------------------------------------
GroupSourcesByFolder(${PROJECT_NAME})
