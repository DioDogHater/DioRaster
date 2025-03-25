cmake_minimum_required(VERSION 3.20)

project(triangle_raster)

set(CMAKE_C_STANDARD 17)

set(CMAKE_C_FLAGS,"-Wall -Wextra")
set(CMAKE_BUILD_TYPE Release)

add_executable(raster src/main.c)
target_link_libraries(raster m)
target_link_libraries(raster SDL2)
target_link_libraries(raster pthread)