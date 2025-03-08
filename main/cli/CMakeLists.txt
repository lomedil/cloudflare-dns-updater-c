cmake_minimum_required(VERSION 2.8)

add_executable(cfcli src/main.c)

set_target_properties(cfcli PROPERTIES C_STANDARD 11)
target_include_directories(cf PUBLIC include)
target_compile_options(cf PRIVATE -Wall -Werror -Wpedantic)

target_link_libraries(cfcli PUBLIC cf)
target_include_directories(test PRIVATE $<TARGET_PROPERTY:cf,INTERFACE_INCLUDE_DIRECTORIES>)