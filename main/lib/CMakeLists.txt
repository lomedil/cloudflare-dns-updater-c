cmake_minimum_required(VERSION 3.12)


file(GLOB MAIN_SOURCES src/*.c)
file(GLOB MAIN_HEADERS include/*.h)

add_library(cf STATIC ${MAIN_SOURCES} ${MAIN_HEADERS})

find_package(CURL REQUIRED)

if(CURL_FOUND)
  message(STATUS "libcurl found ${CURL_LIBRARIES}")
else()
  message(FATAL_ERROR "libcurl not found")
endif()

target_link_libraries(cf PRIVATE ${CURL_LIBRARIES})

set_target_properties(cf PROPERTIES C_STANDARD 11)
target_compile_options(cf PRIVATE -Wall -Werror -Wpedantic)

target_include_directories(cf PUBLIC include)
