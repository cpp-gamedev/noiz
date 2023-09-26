project(noiz-histogram)

add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE
  noiz::noiz-lib
  noiz::noiz-compile-options
)

target_sources(${PROJECT_NAME} PRIVATE
  histogram.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ../common
)
