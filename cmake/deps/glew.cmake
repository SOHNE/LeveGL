if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

if(NOT DEFINED EMSCRIPTEN)
  string(TIMESTAMP BEFORE "%s")

  message(STATUS "== GLEW ================================================================= ")

  CPMAddPackage(
    NAME GLEW
    GITHUB_REPOSITORY Perlmint/glew-cmake
    GIT_TAG glew-cmake-2.2.0
    OPTIONS
      "BUILD_SHARED_LIBS OFF"
      "BUILD_UTILS OFF"
      "BUILD_32_BIT OFF"
      "BUILD_SINGLE_CONTEXT OFF"
      "GLEW_USE_STATIC_LIBS ON"
  )

  if(GLEW_ADDED)
    list(APPEND LEVE_INCLUDE_DIR ${GLEW_SOURCE_DIR}/include)

    list(APPEND LEVE_LINK_DEPS
      $<IF:$<TARGET_EXISTS:libglew_static>,libglew_static,
      $<IF:$<TARGET_EXISTS:GLEW::GLEW>,GLEW::GLEW,>>
    )

    string(TIMESTAMP AFTER "%s")
    math(EXPR DELTA_GLEW "${AFTER} - ${BEFORE}")
    message(STATUS "> GLEW in ${DELTA_GLEW}s")
  else()
    message(FATAL_ERROR "Failed to add GLEW package")
  endif()
endif()

