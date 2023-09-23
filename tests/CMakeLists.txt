project(noiz-test)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
  test/test.cpp
  test/test.hpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  noiz::noiz-lib
  noiz::noiz-compile-options
)

target_include_directories(${PROJECT_NAME} PUBLIC
  .
)

add_subdirectory(tests)

# add_noiz_test(vec2 test_vec2.cpp)
# add_noiz_test(generator test_generator.cpp)
# add_noiz_test(grid2 test_grid2.cpp)
# add_noiz_test(data2 test_data2.cpp)
