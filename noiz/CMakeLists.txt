project(noiz-lib)

add_library(${PROJECT_NAME} INTERFACE)
add_library(noiz::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS include FILES
  include/noiz/detail/generator.hpp
  include/noiz/detail/grid2.hpp

  include/noiz/cell2.hpp
  include/noiz/index2.hpp
  include/noiz/seed.hpp
  include/noiz/vec2.hpp
)

target_include_directories(${PROJECT_NAME} INTERFACE
  include
)
