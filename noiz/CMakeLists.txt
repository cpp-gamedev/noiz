project(noiz-lib)

add_library(${PROJECT_NAME} INTERFACE)
add_library(noiz::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS include FILES

#does the order of these files matter?
  include/noiz/detail/generator.hpp
  include/noiz/seed.hpp

  include/noiz/detail/data2.hpp
  include/noiz/detail/grid2.hpp

  include/noiz/cell2.hpp
  include/noiz/index2.hpp
  include/noiz/noise-processing2.hpp
  include/noiz/noise2.hpp
  include/noiz/vec2.hpp

  include/noiz/detail/data3.hpp
  include/noiz/detail/grid3.hpp

  include/noiz/cell3.hpp
  include/noiz/index3.hpp
  include/noiz/noise-processing3.hpp
  include/noiz/noise3.hpp
  include/noiz/vec3.hpp

)

target_include_directories(${PROJECT_NAME} INTERFACE
  include
)
