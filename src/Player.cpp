#include "Player.h"
#include "Graphics.h"

Player::Player(Graphics &graphics, const std::string &filepath, int coordinateX, int coordinateY,
               int frameTime, int totalFrames) :
        Unit::Unit(graphics, filepath, coordinateX, coordinateY, frameTime, totalFrames)
{
    this->hitbox->r = 14;
}

SDL_Point* Player::followDirections() {
    return this->coordinates;
}

void Player::setDestination(SDL_Point dest) {
    Unit::setDestination(dest);
}
