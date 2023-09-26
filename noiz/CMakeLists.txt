project(noiz-lib)

add_library(${PROJECT_NAME} INTERFACE)
add_library(noiz::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS include FILES

#does the order of these files matter?
  include/generator.hpp
  include/seed.hpp

  include/noiz2/detail/data2.hpp
  include/noiz2/detail/grid2.hpp

  include/noiz2/cell2.hpp
  include/noiz2/index2.hpp
  include/noiz2/noise-processing2.hpp
  include/noiz2/noise2.hpp
  include/noiz2/vec2.hpp

  include/noiz3/detail/data3.hpp
  include/noiz3/detail/grid3.hpp

  include/noiz3/cell3.hpp
  include/noiz3/index3.hpp
  include/noiz3/noise-processing3.hpp
  include/noiz3/noise3.hpp
  include/noiz3/vec3.hpp

)

target_include_directories(${PROJECT_NAME} INTERFACE
  include
)
