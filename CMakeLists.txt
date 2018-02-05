cmake_minimum_required(VERSION 3.9)
project(GItask)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -mwindows")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(headers SDL2/include/SDL2 SDL2_image/include/SDL2)
link_directories(SDL2/lib SDL2_image/lib)

set(SOURCE_FILES
        main.cpp headers/Graphics.h src/Graphics.cpp headers/Game.h src/Game.cpp headers/Input.h src/Input.cpp
        headers/globals.h src/globals.cpp headers/Player.h src/Player.cpp headers/Level.h src/Level.cpp
        headers/Pointer.h src/Pointer.cpp headers/Scroll.h src/Scroll.cpp headers/Unit.h src/Unit.cpp
        headers/Enemy.h src/Enemy.cpp headers/Cannon.h src/Cannon.cpp headers/Circle.h
)

add_executable(GItask ${SOURCE_FILES})

target_link_libraries(GItask mingw32 SDL2main SDL2 SDL2_image)
