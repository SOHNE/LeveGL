if(NOT DEFINED EMSCRIPTEN)
  string(TIMESTAMP BEFORE "%s")

  message(STATUS "== GLFW ================================================================= ")

  CPMAddPackage(
    NAME GLFW
    GITHUB_REPOSITORY glfw/glfw
    GIT_TAG 3.4
    OPTIONS
      "GLFW_BUILD_DOCS OFF"
      "GLFW_BUILD_TESTS OFF"
      "GLFW_BUILD_EXAMPLES OFF"
      "GLFW_INSTALL ON"
  )

  if(GLFW_ADDED)
    list(APPEND LEVE_INCLUDE_DIR ${GLFW_SOURCE_DIR}/include)

    list(APPEND LEVE_LINK_DEPS glfw)

    string(TIMESTAMP AFTER "%s")
    math(EXPR DELTA_GLFW "${AFTER} - ${BEFORE}")
    message(STATUS "> GLFW in ${DELTA_GLFW}s")
  else()
    message(FATAL_ERROR "Failed to add GLFW package")
  endif()
endif()
