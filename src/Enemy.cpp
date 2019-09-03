#include "Enemy.hpp"
#include "Graphics.hpp"
#include "Player.hpp"
#include "Scroll.hpp"

Enemy::Enemy(Graphics &graphics, const std::string &filepath, SDL_Point coordinates,
             std::vector<SDL_Point> &&route, int frameTime, int totalFrames)
    : Unit(graphics, filepath, coordinates, frameTime, totalFrames)
    , currentRoutePoint(0)
    , route(route)
    , following(false)
    , followDirections(nullptr)
{
    this->hitbox.r = 16;
}

Enemy::Enemy(Graphics &graphics, const std::string &filepath, SDL_Point coordinates,
             Player &player, int frameTime, int totalFrames)
    : Unit(graphics, filepath, coordinates, frameTime, totalFrames)
    , currentRoutePoint(0)
    , route {}
    , following(true)
    , followDirections(player.followDirections())
{
    this->hitbox.r = 16;
}

Enemy::~Enemy() {}

void Enemy::draw(Graphics &graphics, Scroll &scroll) {
    SDL_Rect sourceRect {this->currentFrame * globals::TILE_WIDTH, this->direction * globals::TILE_WIDTH,
                         globals::TILE_WIDTH, globals::TILE_WIDTH};
    SDL_Rect destRect = globals::coordToIsoRect(this->coordinates.x, this->coordinates.y, scroll.getDelta());

    // patrol robot is sightly bigger than scout and requires some offset to fit it cell properly
    destRect.y -= globals::TILE_HEIGHT + 8;
    destRect.h += globals::TILE_HEIGHT;

    destRect.x += this->movementDelta.x;
    destRect.y += this->movementDelta.y;

    this->hitbox.x = destRect.x + destRect.w / 2;
    this->hitbox.y = destRect.y + destRect.h - this->hitbox.r;

    graphics.blitSurface(this->spriteSheet, &sourceRect, &destRect);
}

void Enemy::update(int elapsedTime, Level &level) {
    if (!this->following &&
            this->coordinates.x == this->destination.x && this->coordinates.y == this->destination.y) {
        if (this->currentRoutePoint < this->route.size() - 1) {
            this->currentRoutePoint += 1;
        } else {
            this->currentRoutePoint = 0;
        }
        this->setDestination(route[currentRoutePoint]);
    } else if (this->following) {
        this->setDestination(*followDirections);
    }
    Unit::update(elapsedTime, level);
}
