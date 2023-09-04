project(noiz-tests)

add_library(noiz-test)

target_sources(noiz-test PRIVATE
  test/test.cpp
  test/test.hpp
)

target_link_libraries(noiz-test PUBLIC
  noiz::noiz-lib
  noiz::noiz-compile-options
)

target_include_directories(noiz-test PUBLIC
  .
)

function(add_noiz_test NAME SOURCES)
  add_executable(noiz-test-${NAME})
  target_sources(noiz-test-${NAME} PRIVATE ${SOURCES})
  target_link_libraries(noiz-test-${NAME} PRIVATE noiz-test)
  add_test(NAME noiz-test-${NAME} COMMAND noiz-test-${NAME})
endfunction()

add_noiz_test(vec2 test_vec2.cpp)
add_noiz_test(generator test_generator.cpp)
add_noiz_test(grid2 test_grid2.cpp)
add_noiz_test(data2 test_data2.cpp)
