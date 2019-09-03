#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "globals.hpp"

class Graphics;
class Scroll;
class Unit;

/* Level class
 * Contains all info about the current state of the level.
 * Decided to hardcode the map, since there is only one map anyway.
 */
class Level {
public:
    Level() = delete;
    Level(Level&) = delete;
    explicit Level(Graphics &graphics);
    ~Level();

    void update(int elapsedTime);
    void drawGround(Graphics &graphics, Scroll &scroll);
    void drawMap(Graphics &graphics, Scroll &scroll, int x, int y);

    void changeSquareState(SDL_Point coordinates, std::vector<Unit*> units);

    bool squareIsFree(int x, int y);
    SDL_Point getSpawn();
    SDL_Point getFinish();
    SDL_Point getSize();

private:
    SDL_Point size;
    SDL_Point spawnPoint;
    SDL_Point finishPoint;

    static int map[30][30];

    SDL_Texture* texture;
};

#endif // LEVEL_H
