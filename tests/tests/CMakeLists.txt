project(noiz-tests)

add_executable(${PROJECT_NAME})
file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE noiz-test)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
