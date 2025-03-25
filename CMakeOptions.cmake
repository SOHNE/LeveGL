include(CMakeDependentOption)
include(EnumOption)

#--------------------------------------------------------------------
# Define Enum Options
#--------------------------------------------------------------------
enum_option(PLATFORM "Desktop;Web" "Select the target platform for the build.")
enum_option(OPENGL_VERSION "Auto;4.3;3.3;2.1;1.1;ES 2.0;ES 3.0" "Specify an OpenGL version, or use Auto to let it be determined automatically.")

#--------------------------------------------------------------------
# Build Options
#--------------------------------------------------------------------
option(BUILD_SHARED_LIBS "Build LeveGL as a shared library" OFF)

option(USE_CCACHE "Enable compiler cache that can drastically improve build times" ${LEVE_IS_MAIN})
option(CCACHE_OPTIONS "Compiler cache options" "CCACHE_CPP2=true;CCACHE_SLOPPINESS=clang_index_store")

option(LOG_SUPPORT "Enable LeveGL logging system" ON)

#--------------------------------------------------------------------
# Sanitize Options
#--------------------------------------------------------------------
option(SANITIZE_ADDRESS "Enable Address sanitizer" OFF)
option(SANITIZE_MEMORY "Enable Memory sanitizer" OFF)
option(SANITIZE_THREAD "Enable Thread sanitizer" OFF)
option(SANITIZE_UNDEFINED "Enable Undefined Behavior sanitizer" OFF)
option(SANITIZE_LINK_STATIC "Link sanitizers statically (for gcc)" OFF)
