cmake_minimum_required(VERSION 3.29)
project(adventOfCode24)

set(CMAKE_CXX_STANDARD 23)

# SFML stuff
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)

add_executable(adventOfCode24
        graphics_utils.h
        utilities.h
#        day1/day1.cpp
#        day2/day2.cpp
#        day3/day3.cpp
#        day4/day4.cpp
#        day4/day4p2.cpp
#        day5/day5.cpp
#        day6/day6.cpp
#        day7/day7.cpp
#        day8/day8.cpp
#        day9/day9.cpp
#        day9/day9p2.cpp
#         day10/day10.cpp
#         day11/day11.cpp
#         day11/day11p2.cpp
#         day12/day12.cpp
#         day13/day13.cpp
#         day14/day14.cpp
#         day15/day15.cpp
#         day15/day15p2.cpp
#          day16/day16.cpp
#          day17/day17.cpp
#          day18/day18.cpp
#          day19/day19.cpp
#          day20/day20.cpp
#          day20/day20p2.cpp
#          day21/day21.cpp
          day22/day22.cpp
)

target_link_libraries(adventOfCode24 PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)