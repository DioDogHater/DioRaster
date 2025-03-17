cmake_minimum_required(VERSION 3.20)

project(triangle_raster)

set(CMAKE_C_STANDARD 17)

add_executable(raster src/main.c)
target_link_libraries(raster m)
target_link_libraries(raster SDL2)