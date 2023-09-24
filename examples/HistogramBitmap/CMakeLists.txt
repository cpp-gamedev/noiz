project(noiz-HistogramBitmap)

add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE
  noiz::noiz-lib
  noiz::noiz-compile-options
)

target_sources(${PROJECT_NAME} PRIVATE
  HistogramBitmap.cpp
)
