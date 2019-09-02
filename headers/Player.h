#ifndef PLAYER_H
#define PLAYER_H

#include "Unit.h"

class Graphics;

class Player : public Unit {
public:
    Player() = delete;
    Player(Player&) = delete;
    Player(Graphics &graphics, const std::string &filepath, SDL_Point coordinates,
           int frameTime = 50, int totalFrames = 16);

    void setDestination(SDL_Point dest);
    SDL_Point* followDirections();
};

#endif // PLAYER_H
