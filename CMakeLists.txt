cmake_minimum_required(VERSION 3.14)
project(GameShoot)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE src src/*.cpp src/*.c src/*.h)

find_package(PkgConfig REQUIRED)
pkg_check_modules(sdl2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_image REQUIRED IMPORTED_TARGET SDL2_image)

add_executable(${PROJECT_NAME} ${src})


target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::sdl2 PkgConfig::SDL2_image)

