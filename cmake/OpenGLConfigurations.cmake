# Set OpenGL preference to GLVND
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

message(STATUS "========================================================================")
message(STATUS "> OpenGL")

#------------------------------------------------------------------
# Platform-specific configurations
#------------------------------------------------------------------
if(PLATFORM STREQUAL "Desktop")
    set(PLATFORM_BACKEND "PLATFORM_DESKTOP")

  find_package(OpenGL QUIET)

    if(APPLE)
        if(NOT CMAKE_SYSTEM STRLESS "Darwin-18.0.0")
            add_definitions(-DGL_SILENCE_DEPRECATION)
            message(AUTHOR_WARNING "OpenGL will no longer be supported on Apple systems starting with macOS 10.14 (Mojave)")
        endif()

        # macOS framework handling
        set(FRAMEWORKS Cocoa IOKit CoreFoundation CoreVideo OpenGL)
        foreach(framework IN LISTS FRAMEWORKS)
            find_library(FRAMEWORK_${framework} ${framework})
            list(APPEND LEVE_LINK_DEPS ${FRAMEWORK_${framework}})
        endforeach()

    elseif(WIN32)
        find_package(OpenGL QUIET)
        list(APPEND LEVE_LINK_DEPS
            winmm
            gdi32
            user32
            shell32
            ${OPENGL_LIBRARIES}
        )

    elseif(UNIX)
        find_library(pthread NAMES pthread)
        find_package(OpenGL QUIET)
        if ("${OPENGL_LIBRARIES}" STREQUAL "")
            set(OPENGL_LIBRARIES "GL")
        endif ()

        list(APPEND LEVE_LINK_DEPS m atomic pthread ${OPENGL_LIBRARIES})
    endif()

elseif(PLATFORM STREQUAL "Web")
    set(PLATFORM_BACKEND "PLATFORM_WEB")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")

    if(OPENGL_VERSION STREQUAL "Auto")
        set(OPENGL_VERSION "ES 2.0")
    endif()

    if(NOT OPENGL_VERSION MATCHES "^ES")
        message(FATAL_ERROR "Web platform only supports OpenGL ES versions")
    endif()
endif()

#------------------------------------------------------------------
# OpenGL Version Configuration
#------------------------------------------------------------------

set(OPENGL_CONFIGURATIONS
    "4.3:GRAPHICS_API_OPENGL_43"
    "3.3:GRAPHICS_API_OPENGL_33"
    "2.1:GRAPHICS_API_OPENGL_21"
    "1.1:GRAPHICS_API_OPENGL_11"
    "ES 2.0:GRAPHICS_API_OPENGL_ES2"
    "ES 3.0:GRAPHICS_API_OPENGL_ES3"
)

# TODO: Properly handles user choosed API
if(NOT OPENGL_VERSION STREQUAL "Auto")
    foreach(config IN LISTS OPENGL_CONFIGURATIONS)
        string(REPLACE ":" ";" parts ${config})
        list(GET parts 0 version)
        list(GET parts 1 graphics)

        if(OPENGL_VERSION STREQUAL "${version}")
            set(GRAPHICS ${graphics})
            break()
        endif()
    endforeach()
else()
    if(PLATFORM STREQUAL "Desktop")
        set(GRAPHICS "GRAPHICS_API_OPENGL_33")
    else()
        set(GRAPHICS "GRAPHICS_API_OPENGL_ES2")
    endif()
endif()

#------------------------------------------------------------------
# Target Setup Function
#------------------------------------------------------------------
function(setup_opengl_target target)
    target_include_directories(${target} PRIVATE
        ${OPENGL_INCLUDE_DIR}
    )

    if(PLATFORM STREQUAL "Web" AND EMSCRIPTEN)
        set_target_properties(${target} PROPERTIES
            LINK_FLAGS "-s USE_GLFW=3 -s FULL_ES2=1"
        )

        if(OPENGL_VERSION STREQUAL "ES 3.0")
            target_link_options(${target}
              PRIVATE
                "SHELL:-s FULL_ES3"
                "SHELL:-s MIN_WEBGL_VERSION=2"
                "SHELL:-s MAX_WEBGL_VERSION=2"
            )
        endif()
    endif()
endfunction()
