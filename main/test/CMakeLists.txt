cmake_minimum_required(VERSION 3.12)

project(cf-updater)

enable_testing()

file(GLOB TEST_SOURCES src/*.c)
file(GLOB TEST_HEADERS src/*.h)

add_executable(test ${TEST_SOURCES} ${TEST_HEADERS})

target_link_libraries(test PUBLIC cf)
target_compile_options(test PRIVATE -Wall -Werror -Wextra -pedantic -Wmissing-declarations -g)

target_include_directories(test PRIVATE $<TARGET_PROPERTY:cf,INTERFACE_INCLUDE_DIRECTORIES>)