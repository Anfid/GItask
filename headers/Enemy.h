#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include "Unit.h"

class Player;

class Enemy : public Unit {
public:
    Enemy() = delete;
    Enemy(Enemy&) = delete;
    Enemy(Graphics &graphics, const std::string &filepath, int coordinateX, int coordinateY,
         int routePointsCount, SDL_Point* route[], int frameTime = 100, int totalFrames = 16);
    Enemy(Graphics &graphics, const std::string &filepath, int coordinateX, int coordinateY,
          Player &player, int frameTime = 100, int totalFrames = 16);
    ~Enemy();

    void draw(Graphics &graphics, Scroll &scroll);
    void update(int elapsedTime, Level &level);

private:

    int currentRoutePoint;
    int routePointCount;
    std::vector<SDL_Point*> route;

    bool following;
};

#endif // ENEMY_H
