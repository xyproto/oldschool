cmake_minimum_required(VERSION 2.8)

project(oldschool)

add_executable(simple simple.c)

add_library(oldschool STATIC oldschool.c)
install(TARGETS oldschool DESTINATION lib)
install(FILES oldschool.h DESTINATION include)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(simple ${SDL2_LIBRARIES} oldschool)
install(TARGETS simple DESTINATION bin)

#list(APPEND CMAKE_C_FLAGS "-pthread -std=c11 ${CMAKE_C_FLAGS} -g -ftest-coverage -fprofile-arcs")
list(APPEND CMAKE_C_FLAGS "-O3 -Wall -Wextra -pedantic -std=c11 ${CMAKE_C_FLAGS}")
