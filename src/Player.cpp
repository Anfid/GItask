#include "Graphics.hpp"
#include "Player.hpp"

Player::Player(Graphics &graphics, const std::string &filepath, SDL_Point coordinates,
               int frameTime, int totalFrames) :
        Unit::Unit(graphics, filepath, coordinates, frameTime, totalFrames)
{
    this->hitbox.r = 14;
}

SDL_Point* Player::followDirections() {
    return &this->coordinates;
}

void Player::setDestination(SDL_Point dest) {
    Unit::setDestination(dest);
}
