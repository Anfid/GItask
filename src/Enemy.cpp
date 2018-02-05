#include "Enemy.h"
#include "Graphics.h"
#include "Scroll.h"
#include "Player.h"

Enemy::Enemy() = default;

Enemy::Enemy(Graphics &graphics, const std::string &filepath, int coordinateX, int coordinateY,
        int routePointsCount, SDL_Point* route[], int frameTime, int totalFrames) :
        Unit(graphics, filepath, coordinateX, coordinateY, frameTime, totalFrames)
{
    this->routePointCount = routePointsCount;
    for (int i = 0; i < routePointsCount; ++i) {
        this->route.push_back(route[i]);
    }
    this->currentRoutePoint = 0;
    this->hitbox->r = 16;
}

Enemy::Enemy(Graphics &graphics, const std::string &filepath, int coordinateX, int coordinateY,
        Player &player, int frameTime, int totalFrames) :
        Unit(graphics, filepath, coordinateX, coordinateY, frameTime, totalFrames)
{
    this->route.push_back(player.followDirections());
    following = true;
}

Enemy::~Enemy() {
    if (following) return;
    for (auto &point : this->route) {
        delete point;
    }
}

void Enemy::draw(Graphics &graphics, Scroll &scroll) {
    SDL_Rect sourceRect {this->currentFrame * globals::TILE_WIDTH, this->direction * globals::TILE_WIDTH,
                         globals::TILE_WIDTH, globals::TILE_WIDTH};
    SDL_Rect destRect = globals::coordToIsoRect(this->coordinates->x, this->coordinates->y, scroll.getDelta());

    // patrol robot is sightly bigger than scout and requires some offset to fit it cell properly
    destRect.y -= globals::TILE_HEIGHT + 8;
    destRect.h += globals::TILE_HEIGHT;

    destRect.x += this->movementDelta->x;
    destRect.y += this->movementDelta->y;

    this->hitbox->x = destRect.x + destRect.w / 2;
    this->hitbox->y = destRect.y + destRect.h - this->hitbox->r;

    graphics.blitSurface(this->spriteSheet, &sourceRect, &destRect);
}

void Enemy::update(int elapsedTime, Level &level) {
    if (!this->following &&
            this->coordinates->x == this->destination->x && this->coordinates->y == this->destination->y) {
        if (this->currentRoutePoint < this->routePointCount - 1) {
            ++this->currentRoutePoint;
        } else {
            this->currentRoutePoint = 0;
        }
    }
    this->setDestination(*route[currentRoutePoint]);
    Unit::update(elapsedTime, level);
}
