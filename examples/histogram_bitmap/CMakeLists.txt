project(noiz-histogram_bitmap)

add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE
  noiz::noiz-lib
  noiz::noiz-compile-options
)

target_sources(${PROJECT_NAME} PRIVATE
  histogram_bitmap.cpp
)
