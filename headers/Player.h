#ifndef PLAYER_H
#define PLAYER_H

#include "Unit.h"

class Graphics;

class Player : public Unit {
public:
    Player();
    Player(Graphics &graphics, const std::string &filepath, int coordinateX, int coordinateY,
         int frameTime = 50, int totalFrames = 16);

    void setDestination(SDL_Point dest);
    SDL_Point* followDirections();
};

#endif // PLAYER_H