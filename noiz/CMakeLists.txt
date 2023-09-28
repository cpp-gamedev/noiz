project(noiz-lib)

add_library(${PROJECT_NAME} INTERFACE)
add_library(noiz::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS include FILES
  include/noiz/detail/data2.hpp
  include/noiz/detail/generator.hpp
  include/noiz/detail/grid2.hpp

  include/noiz/cell2.hpp
  include/noiz/index2.hpp
  include/noiz/noise2.hpp
  include/noiz/seed.hpp
  include/noiz/vec2.hpp


  include/noiz/detail/data3.hpp
  include/noiz/detail/grid3.hpp

  include/noiz/cell3.hpp
  include/noiz/index3.hpp
  include/noiz/noise3.hpp
  include/noiz/vec3.hpp


  include/noiz/detail/data4.hpp
  include/noiz/detail/grid4.hpp

  include/noiz/cell4.hpp
  include/noiz/index4.hpp
  include/noiz/noise4.hpp
  include/noiz/vec4.hpp
  )

target_include_directories(${PROJECT_NAME} INTERFACE
  include
)
