#include "globals.h"

SDL_Rect globals::coordToIsoRect(int x, int y, SDL_Point scrollDelta) {
    SDL_Rect isoRect;
    isoRect.x = (x - y) * globals::TILE_WIDTH / 2 - scrollDelta.x;
    isoRect.y = (x + y) * globals::TILE_HEIGHT / 2 - scrollDelta.y;
    isoRect.w = globals::TILE_WIDTH;
    isoRect.h = globals::TILE_HEIGHT;
    return isoRect;
}

SDL_Point globals::isoToCoord(SDL_Point tilePos, SDL_Point scrollDelta) {
    SDL_Point coordinates;
    coordinates.x = (tilePos.x + scrollDelta.x + 2 * (tilePos.y + scrollDelta.y)) / globals::TILE_WIDTH;
    coordinates.y = (2 * (tilePos.y + scrollDelta.y) - (tilePos.x + scrollDelta.x)) / globals::TILE_WIDTH;
    return coordinates;
}
