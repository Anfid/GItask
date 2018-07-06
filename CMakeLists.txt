cmake_minimum_required(VERSION 3.9)
project(GItask)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(headers)

set(SOURCE_FILES
        main.cpp headers/Graphics.h src/Graphics.cpp headers/Game.h src/Game.cpp headers/Input.h src/Input.cpp
        headers/globals.h src/globals.cpp headers/Player.h src/Player.cpp headers/Level.h src/Level.cpp
        headers/Pointer.h src/Pointer.cpp headers/Scroll.h src/Scroll.cpp headers/Unit.h src/Unit.cpp
        headers/Enemy.h src/Enemy.cpp headers/Cannon.h src/Cannon.cpp headers/Circle.h
)

if(WIN32)
    message("Building for Windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")

    include_directories(SDL2/include/SDL2 SDL2_image/include/SDL2)
    link_directories(SDL2/lib SDL2_image/lib)

    add_executable(GItask ${SOURCE_FILES})

    target_link_libraries(GItask mingw32 SDL2main SDL2 SDL2_image)
endif()

if(UNIX AND NOT APPLE)
    message("Building for Linux")
    include(FindPkgConfig)
    pkg_search_module(SDL2 REQUIRED sdl2)
    pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

    add_executable(GItask ${SOURCE_FILES})

    target_link_libraries(GItask ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
endif()
