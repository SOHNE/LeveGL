include(OpenGLConfigurations)

#--------------------------------------------------------------------
# Source and Header Files
#--------------------------------------------------------------------
set(LEVE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/levegl)
set(LEVE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

list(APPEND LEVE_PUBLIC_HEADER_FILES
  ${LEVE_INCLUDE_DIR}/leapi.h
  ${LEVE_INCLUDE_DIR}/legl.h
  ${LEVE_INCLUDE_DIR}/leutils.h
  ${LEVE_INCLUDE_DIR}/levegl.h
)

list(APPEND LEVE_PRIVATE_HEADER_FILES
  ${LEVE_SOURCE_DIR}/lecore_context.h
)

list(APPEND LEVE_SOURCE_FILES
  # Modules
  ${LEVE_SOURCE_DIR}/lecore.c
  ${LEVE_SOURCE_DIR}/leshader.c
  ${LEVE_SOURCE_DIR}/leshapes.c
  ${LEVE_SOURCE_DIR}/leutils.c
)

# Add platform-specific
if(PLATFORM STREQUAL "Desktop")
    set(platform_file "leglfw.c")
elseif(PLATFORM STREQUAL "Web")
    set(platform_file "leweb.c")
else()
    message(FATAL_ERROR "Unsupported platform: ${PLATFORM}")
endif()

list(APPEND LEVE_SOURCE_FILES "${LEVE_SOURCE_DIR}/platforms/${platform_file}")


#--------------------------------------------------------------------
# Library Setup
#--------------------------------------------------------------------
add_library(${PROJECT_NAME}
    # Public headers
    ${LEVE_PUBLIC_HEADER_FILES}

    # Source files
    ${LEVE_SOURCE_FILES}

    # Private headers
    ${LEVE_PRIVATE_HEADER_FILES}
)

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
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${LEVE_INCLUDE_DEPS_DIR}"
)

#--------------------------------------------------------------------
# Target Properties
#--------------------------------------------------------------------
set_target_properties(${PROJECT_NAME}
PROPERTIES
        PUBLIC_HEADER "${LEVE_PUBLIC_HEADER_FILES}"
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        C_EXTENSIONS OFF
        C_STANDARD 99
        C_STANDARD_REQUIRED ON
)

#--------------------------------------------------------------------
# Compile definitions
#--------------------------------------------------------------------
target_compile_definitions(${PROJECT_NAME} PUBLIC
    $<$<CONFIG:Debug>:LE_DEBUG>
    $<$<CONFIG:Release>:LE_RELEASE>
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    $<$<BOOL:${BUILD_SHARED_LIBS}>:LE_SHARED>
    $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:LE_STATIC>
)

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<BOOL:${LOG_SUPPORT}>:LOG_SUPPORT>)

target_compile_definitions(${PROJECT_NAME} PUBLIC "${PLATFORM_BACKEND}")
target_compile_definitions(${PROJECT_NAME} PUBLIC "${GRAPHICS}")

#--------------------------------------------------------------------
# Source Groups
#--------------------------------------------------------------------
GroupSourcesByFolder(${PROJECT_NAME})
