#include "Cannon.h"

#include "SDL2/SDL.h"
#include "Graphics.h"
#include "Scroll.h"

Cannon::Cannon() {}

Cannon::Cannon(Graphics& graphics, SDL_Point *coordinates, int fireFrequency, int fireballLifetime, int fireballSpeed) :
        fireFrequency(fireFrequency),
        maxFireballLifetime(fireballLifetime),
        fireballSpeed(fireballSpeed)
{
    this->timeAfterShot = 0;
    this->spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("../content/cannon.png"));
    this->coordinates = coordinates;
    if (this->coordinates->x == 0)
        this->direction = DOWNRIGHT;
    else if (this->coordinates->y == 0)
        this->direction = DOWNLEFT;

    this->timeAfterUpdate = 0;
    this->timeToUpdate = 20;
}

Cannon::~Cannon() {
    delete this->coordinates;
    for (auto fireball : this->fireballs) {
        delete fireball;
    }
}

void Cannon::update(int elapsedTime, Scroll &scroll) {
    this->timeAfterShot += elapsedTime;
    this->timeAfterUpdate += elapsedTime;
    if (this->timeAfterUpdate > this->timeToUpdate) {
        if (this->timeAfterShot > this->fireFrequency) {
            SDL_Rect rect = globals::coordToIsoRect(this->coordinates->x, this->coordinates->y, scroll.getDelta());
            rect.x += scroll.getDelta().x;
            rect.y += scroll.getDelta().y;
            if (this->direction == DOWNRIGHT) rect.x += 32;
            this->fireballs.push_back(new Fireball(rect.x, rect.y));
            this->timeAfterShot -= this->fireFrequency;
        }
        for (auto &fireball : this->fireballs) {
            fireball->lifetime += this->timeAfterUpdate;
            if (this->direction == DOWNRIGHT) {
                fireball->x += this->fireballSpeed;
                fireball->y += this->fireballSpeed / 2;
            } else if (this->direction == DOWNLEFT) {
                fireball->x -= this->fireballSpeed;
                fireball->y += this->fireballSpeed / 2;
            }
            fireball->hitbox.x = fireball->x + 16 - scroll.getDelta().x;
            fireball->hitbox.y = fireball->y + 24 - scroll.getDelta().y;
        }
        if (fireballs.front() ? fireballs.front()->lifetime > this->maxFireballLifetime : false) {
            delete fireballs.front();
            this->fireballs.pop_front();
        }
        this->timeAfterUpdate -= this->timeToUpdate;
    }
}

void Cannon::draw(Graphics &graphics, Scroll &scroll) {
    SDL_Rect sourceRect = {0, 0, globals::TILE_WIDTH, 2 * globals::TILE_HEIGHT};
    SDL_Rect destRect = globals::coordToIsoRect(this->coordinates->x, this->coordinates->y, scroll.getDelta());

    destRect.h += globals::TILE_HEIGHT;
    destRect.y -= globals::TILE_HEIGHT;
    if (this->direction == DOWNLEFT)
        sourceRect.x += globals::TILE_WIDTH;

    graphics.blitSurface(this->spriteSheet, &sourceRect, &destRect);

    for (auto fireball : this->fireballs) {
        sourceRect = {128, 0, globals::TILE_HEIGHT, globals::TILE_HEIGHT};
        if (this->direction == DOWNRIGHT) sourceRect.y += globals::TILE_HEIGHT;
        destRect = {fireball->x - scroll.getDelta().x, fireball->y - scroll.getDelta().y, globals::TILE_HEIGHT, globals::TILE_HEIGHT};
        graphics.blitSurface(this->spriteSheet, &sourceRect, &destRect);
    }
}

SDL_Point Cannon::getPosition() {
    return *this->coordinates;
}

std::list<Fireball*> Cannon::getFireballs() {
    return this->fireballs;
}

Fireball::Fireball(int x, int y) {
    this->x = x;
    this->y = y;
    this->lifetime = 0;
    this->hitbox = Circle {0, 0, 12};
}
