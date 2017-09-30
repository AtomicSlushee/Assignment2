# Authors: Ken Steele, Andrew Byron and Eric Lichtenstein
# Assignment 2

# prevent in-source builds
cmake_minimum_required( VERSION 2.6 )
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed. Create and use a 'build' folder.")
endif()

# start of project
project( dpgen )
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
add_subdirectory( src )

