cmake_minimum_required(VERSION 3.9)
project(GItask)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# For linters
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

set(SOURCE_FILES
        main.cpp include/Graphics.hpp src/Graphics.cpp include/Game.hpp src/Game.cpp include/Input.hpp src/Input.cpp
        include/globals.hpp src/globals.cpp include/Player.hpp src/Player.cpp include/Level.hpp src/Level.cpp
        include/Pointer.hpp src/Pointer.cpp include/Scroll.hpp src/Scroll.cpp include/Unit.hpp src/Unit.cpp
        include/Enemy.hpp src/Enemy.cpp include/Cannon.hpp src/Cannon.cpp include/Circle.hpp
)

if(WIN32)
    message("Building for Windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")

    include_directories(SDL2/include SDL2_image/include)
    link_directories(SDL2/lib SDL2_image/lib)

    add_executable(GItask ${SOURCE_FILES})

    target_link_libraries(GItask mingw32 SDL2main SDL2 SDL2_image)
endif()

if(UNIX AND NOT APPLE)
    message("Building for Linux")
    include(FindPkgConfig)
    pkg_search_module(SDL2 REQUIRED sdl2)
    pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

    add_executable(GItask ${SOURCE_FILES})

    target_link_libraries(GItask ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
endif()
