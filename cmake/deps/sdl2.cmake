if(NOT DEFINED EMSCRIPTEN)
  # SDL2
  string(TIMESTAMP BEFORE "%s")

  message(STATUS "== SDL ================================================================= ")

  CPMAddPackage(
      NAME SDL2
      GITHUB_REPOSITORY libsdl-org/SDL
      GIT_TAG release-2.32.4
      OPTIONS
      "SDL2_DISABLE_INSTALL ON"
      "SDL_SHARED OFF"
      "SDL_STATIC ON"
      "SDL_STATIC_PIC ON"
      "SDL_WERROR OFF"
  )

  if(SDL2_ADDED)
    list(APPEND LEVE_LINK_DEPS
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
      $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )

    string(TIMESTAMP AFTER "%s")
    math(EXPR DELTASDL "${AFTER} - ${BEFORE}")
    message(STATUS "SDL2 TIME: ${DELTASDL}s")
  else()
    message(FATAL_ERROR "Failed to add SDL2 package")
  endif()

endif()

