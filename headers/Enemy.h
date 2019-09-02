#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include "Unit.h"

class Player;

class Enemy : public Unit {
public:
    Enemy() = delete;
    Enemy(Enemy&) = delete;
    Enemy(Graphics &graphics, const std::string &filepath, SDL_Point coordinates,
          std::vector<SDL_Point> &&route, int frameTime = 100, int totalFrames = 16);
    Enemy(Graphics &graphics, const std::string &filepath, SDL_Point coordinates,
          Player &player, int frameTime = 100, int totalFrames = 16);
    ~Enemy();

    void draw(Graphics &graphics, Scroll &scroll);
    void update(int elapsedTime, Level &level);

private:
    int currentRoutePoint;
    std::vector<SDL_Point> route;

    bool following;
    SDL_Point* followDirections;
};

#endif // ENEMY_H
