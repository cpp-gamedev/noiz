project(noiz-lib)

add_library(${PROJECT_NAME} INTERFACE)
add_library(noiz::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS include FILES
    include/noiz/detail/dataTemplate.hpp
    include/noiz/detail/generator.hpp
    include/noiz/detail/gridTemplate.hpp

    include/noiz/cellTemplate.hpp
    include/noiz/indexTemplate.hpp
    include/noiz/noiseTemplate.hpp
    include/noiz/seed.hpp
    include/noiz/vecTemplate.hpp
  )

target_include_directories(${PROJECT_NAME} INTERFACE
  include
)
