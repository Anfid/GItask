#include <globals.h>
#include <Level.h>
#include "Pointer.h"
#include "Graphics.h"

Pointer::Pointer(Graphics &graphics, const std::string &filepath)
    : free(false)
    , position {0, 0}
    , tilePos {0, 0}
{
    this->spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filepath));
    if (spriteSheet == NULL) {
        printf("\nError: Unable to load the image\n");
    }
}

Pointer::~Pointer() {}

void Pointer::draw(Graphics &graphics) {
    this->sourceRect.x = this->free ? 704 : 704 + globals::TILE_WIDTH;
    this->sourceRect.y = 928;
    this->sourceRect.w = globals::TILE_WIDTH;
    this->sourceRect.h = globals::TILE_HEIGHT;

    SDL_Rect destRect = {this->tilePos.x, this->tilePos.y, this->sourceRect.w,
                         this->sourceRect.h};
    graphics.blitSurface(this->spriteSheet, &this->sourceRect, &destRect);
}

void Pointer::update(SDL_Point newPos, SDL_Point scrollDelta, bool isFree) {
    this->position = newPos;

    this->free = isFree;

    // Compensate the tile size, so highlighting and mouse pointer match
    SDL_Point compPos;
    compPos.x = newPos.x - globals::TILE_WIDTH / 4;
    compPos.y = newPos.y - globals::TILE_HEIGHT / 4;

    this->tilePos.x = compPos.x - (compPos.x + scrollDelta.x) % (globals::TILE_WIDTH / 2);
    // Compensates modulus operator result when value goes negative resulting in more precise pointer.
    if (compPos.x + scrollDelta.x < 0)
        this->tilePos.x -= globals::TILE_WIDTH / 2;

    this->tilePos.y = compPos.y - (compPos.y + scrollDelta.y) % (globals::TILE_HEIGHT);
    if (compPos.y + scrollDelta.y < 0)
        this->tilePos.y -= globals::TILE_HEIGHT;

    if (((this->tilePos.x + scrollDelta.x) / (globals::TILE_WIDTH / 2)) % 2) {
        this->tilePos.y += globals::TILE_HEIGHT / 2;
    }
}

SDL_Point Pointer::getPosition() const {
    return this->position;
}

SDL_Point Pointer::getTilePos() const {
    return this->tilePos;
}
