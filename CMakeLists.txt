cmake_minimum_required(VERSION 3.9)
project(GItask)

set(CMAKE_CXX_STANDARD 11)

add_executable(GItask main.cpp headers/Graphics.h src/Graphics.cpp headers/Game.h src/Game.cpp headers/Input.h src/Input.cpp headers/globals.h headers/Player.h src/Player.cpp headers/Level.h src/Level.cpp headers/Pointer.h src/Pointer.cpp headers/Scroll.h src/Scroll.cpp headers/Unit.h src/Unit.cpp src/globals.cpp headers/Enemy.h src/Enemy.cpp headers/Cannon.h src/Cannon.cpp headers/Circle.h)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(GItask ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

include_directories(headers)
