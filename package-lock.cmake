# CPM Package Lock
# This file should be committed to version control

# PackageProject.cmake
CPMDeclarePackage(PackageProject.cmake
  VERSION 1.13.0
  GITHUB_REPOSITORY TheLartians/PackageProject.cmake
  SYSTEM YES
  EXCLUDE_FROM_ALL YES
)
# GroupSourcesByFolder.cmake
CPMDeclarePackage(GroupSourcesByFolder.cmake
  VERSION 1.0
  GITHUB_REPOSITORY TheLartians/GroupSourcesByFolder.cmake
  SYSTEM YES
  EXCLUDE_FROM_ALL YES
)
# GLEW (unversioned)
# CPMDeclarePackage(GLEW
#  NAME GLEW
#  GIT_TAG glew-cmake-2.2.0
#  GITHUB_REPOSITORY Perlmint/glew-cmake
#  OPTIONS
#    "BUILD_SHARED_LIBS OFF"
#    "BUILD_UTILS OFF"
#    "BUILD_32_BIT OFF"
#    "BUILD_SINGLE_CONTEXT OFF"
#    "GLEW_USE_STATIC_LIBS ON"
#)
# GLFW
CPMDeclarePackage(GLFW
  NAME GLFW
  GIT_TAG 3.4
  GITHUB_REPOSITORY glfw/glfw
  OPTIONS
    "GLFW_BUILD_DOCS OFF"
    "GLFW_BUILD_TESTS OFF"
    "GLFW_BUILD_EXAMPLES OFF"
    "GLFW_INSTALL ON"
)
