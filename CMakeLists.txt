cmake_minimum_required(VERSION 3.23)

project(noiz)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(is_project_root OFF)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(is_project_root ON)
endif()

option(NOIZ_BUILD_TESTS "Build noiz tests" ${is_project_root})
option(NOIZ_BUILD_EXAMPLES "Build noiz examples" ${is_project_root})

add_library(noiz-compile-options INTERFACE)
add_library(noiz::noiz-compile-options ALIAS noiz-compile-options)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(noiz-compile-options INTERFACE
    -Wall -Wextra -Wpedantic -Wconversion -Wunused -Werror=return-type $<$<NOT:$<CONFIG:Debug>>:-Werror>
  )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
  target_compile_options(noiz-compile-options INTERFACE
    $<$<NOT:$<CONFIG:Debug>>:/WX>
  )
endif()

add_subdirectory(noiz)

if(NOIZ_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(NOIZ_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
