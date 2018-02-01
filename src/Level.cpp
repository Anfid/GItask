#include <Scroll.h>
#include "Unit.h"
#include "Level.h"
#include "Graphics.h"

namespace {
    SDL_Rect groundSource = {0, 0, globals::TILE_WIDTH, globals::TILE_HEIGHT};
    SDL_Rect closedPortalSource = {0, 1065, globals::TILE_WIDTH * 2, globals::TILE_HEIGHT * 2};
    SDL_Rect openPortalSource = {128, 1065, globals::TILE_WIDTH * 2, globals::TILE_HEIGHT * 2};
}

Level::Level() {}

Level::Level(Graphics &graphics) {
    this->spawnPoint = new SDL_Point {1, 27};
    this->finishPoint = new SDL_Point {27, 1};

    this->size = new SDL_Point {30, 30};

    this->texture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("../content/tiles.png"));
}

Level::~Level() {
    delete this->spawnPoint;
    delete this->finishPoint;
    delete this->size;
}

void Level::update(int elapsedTime) {}

void Level::drawGround(Graphics &graphics, Scroll &scroll) {
    SDL_Rect destRect;
    for (int x = 0; x < this->size->x; ++x) {
        for (int y = 0; y < this->size->y; ++y) {
            destRect = globals::coordToIsoRect(x, y, scroll.getDelta());
            graphics.blitSurface(this->texture, &groundSource, &destRect);
        }
    }
    destRect = globals::coordToIsoRect(0, 27, scroll.getDelta());
    destRect.y += globals::TILE_HEIGHT / 2;
    destRect.w += globals::TILE_WIDTH;
    destRect.h += globals::TILE_HEIGHT;
    graphics.blitSurface(this->texture, &closedPortalSource, &destRect);

    destRect = globals::coordToIsoRect(26, 1, scroll.getDelta());
    destRect.y += globals::TILE_HEIGHT / 2;
    destRect.w += globals::TILE_WIDTH;
    destRect.h += globals::TILE_HEIGHT;
    graphics.blitSurface(this->texture, &openPortalSource, &destRect);
}

void Level::drawMap(Graphics &graphics, Scroll &scroll, int x, int y) {
    if (this->map[x][y]) {
        SDL_Rect sourceRect;
        SDL_Rect destRect;

        if (this->map[x][y] > 0 && this->map[x][y] <= 16) {
            sourceRect = {0, 65, 64, 96};
            sourceRect.x = (this->map[x][y] - 1) * globals::TILE_WIDTH;
            destRect = globals::coordToIsoRect(x, y, scroll.getDelta());
            // Walls have different size, so it has to be recalculated
            destRect.h = 96;
            destRect.y -= destRect.h - globals::TILE_HEIGHT;
        } else if (this->map[x][y] == 17) {
            sourceRect = {444, 992, 64, 32};
            destRect = globals::coordToIsoRect(x, y, scroll.getDelta());
            destRect.y -= 8;
        }
        graphics.blitSurface(this->texture, &sourceRect, &destRect);
    }
}

void Level::changeSquareState(SDL_Point coordinates, std::vector<Unit*> units) {
    if (coordinates.x >= 0 && coordinates.x <30 &&
            coordinates.y >= 0 && coordinates.y < 30) {
        for (auto unit : units) {
            if (unit->getPosition().x == coordinates.x && unit->getPosition().y == coordinates.y) return;
        }
        if (map[coordinates.x][coordinates.y] == 0) {
            map[coordinates.x][coordinates.y] = 17;
        } else if (map[coordinates.x][coordinates.y] == 17) {
            map[coordinates.x][coordinates.y] = 0;
        }
    }
}

bool Level::squareIsFree(int x, int y) {
    // no walls or objects and within level bounds
    if (x >= 0 && x < 30 && y >= 0 && y < 30) {
        return !map[x][y];
    }
    return false;
}

SDL_Point Level::getSpawn() {
    return *this->spawnPoint;
}

SDL_Point Level::getFinish() {
    return *this->finishPoint;
}

SDL_Point Level::getSize() {
    return *this->size;
}

int Level::map[30][30] = {
        {13, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 12},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8},
        {10, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 3, 7, 15},
};
