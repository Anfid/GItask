#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL_rect.h>

namespace globals {
    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 768;

    const int TILE_WIDTH = 64;
    const int TILE_HEIGHT = 32;

    const float SCROLL_SENSITIVITY = 10.0f; // comfort values are ~8
    const int SCROLL_ZONE = 50; //px. 40-80 are best

    SDL_Rect coordToIsoRect(int x, int y, SDL_Point scrollDelta);
    SDL_Point isoToCoord(SDL_Point tilePos, SDL_Point scrollDelta);
}

enum Direction {
    DOWN,
    DOWNRIGHT,
    RIGHT,
    UPRIGHT,
    UP,
    UPLEFT,
    LEFT,
    DOWNLEFT
};


#endif // GLOBALS_H
