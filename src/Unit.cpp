#include "Unit.h"

#include "SDL2/SDL.h"
#include <forward_list>
#include <list>
#include <vector>
#include <limits>

#include "Graphics.h"
#include "Scroll.h"
#include "Level.h"

namespace {
    struct Cell {
        Cell(int x, int y, SDL_Point* destination, struct Cell* parent = nullptr) {
            this->x = x;
            this->y = y;
            this->parent = parent;
            int gCost;
            if (this->parent)
                gCost = this->parent->gCost + (this->parent->x - this->x != 0 && y != 0 ? 14 : 10);
            else
                gCost = 0;

            this->gCost = gCost;

            int dX = std::abs(this->x - destination->x);
            int dY = std::abs(this->y - destination->y);
            // 10 and 14 are costs to get to neighbouring cell
            this->hCost = std::min(dX, dY) * 14 + std::abs(dX - dY) * 10;

            this->fCost = this->gCost + this->hCost;
        }
        int x, y;
        int gCost;
        int hCost;
        int fCost;
        struct Cell* parent;
    };
}

Unit::Unit(Graphics &graphics, const std::string &filepath, int coordinateX, int coordinateY,
           int frameTime, int totalFrames)
    : currentFrame(2)
    , totalFrames(totalFrames)
    , timeAfterNewFrame(0)
    , frameTime(frameTime)
{
    this->spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filepath));

    this->coordinates = new SDL_Point {coordinateX, coordinateY};
    this->direction = RIGHT;
    this->next = new SDL_Point {coordinateX, coordinateY};
    this->destination = new SDL_Point {coordinateX, coordinateY};

    this->movementDelta = new SDL_Point {0, 0};

    this->hitbox = new Circle {0, 0, 0};
}

Unit::~Unit() {
    delete this->coordinates;
    delete this->next;
    delete this->destination;
    delete this->movementDelta;
    delete this->hitbox;
    SDL_DestroyTexture(this->spriteSheet);
}

void Unit::draw(Graphics &graphics, Scroll &scroll) {
    SDL_Rect sourceRect {this->currentFrame * globals::TILE_WIDTH, this->direction * globals::TILE_WIDTH,
                         globals::TILE_WIDTH, globals::TILE_WIDTH};
    SDL_Rect destRect = globals::coordToIsoRect(this->coordinates->x, this->coordinates->y, scroll.getDelta());
    destRect.y -= globals::TILE_HEIGHT;
    destRect.h += globals::TILE_HEIGHT;

    destRect.x += this->movementDelta->x;
    destRect.y += this->movementDelta->y;

    this->hitbox->x = destRect.x + destRect.w / 2;
    this->hitbox->y = destRect.y + destRect.h - this->hitbox->r;

    graphics.blitSurface(this->spriteSheet, &sourceRect, &destRect);
}

void Unit::update(int elapsedTime, Level &level) {
    this->timeAfterNewFrame += elapsedTime;
    if (this->timeAfterNewFrame > this->frameTime) {

        if (!level.squareIsFree(this->next->x, this->next->y)) {
            this->next->x = this->coordinates->x;
            this->next->y = this->coordinates->y;
        }
        // Update animation frame
        if (this->coordinates->x == this->next->x && this->coordinates->y == this->next->y &&
                this->movementDelta->x == 0 && this->movementDelta->y == 0) {
            if (this->coordinates->x != this->destination->x || this->coordinates->y != this->destination->y) {
                this->findPath(level);
            }
        } else {
            // Update position on the grid
            if (this->coordinates->x == this->next->x && this->coordinates->y == this->next->y &&
                    (this->movementDelta->x != 0 || this->movementDelta->y != 0)) {
                this->walkBack(); // Make delta 0 to return to proper position on the grid
            } else {
                this->walk(); // Move towards another cell
            }
        }
        this->timeAfterNewFrame -= this->frameTime;
    }
}

SDL_Point Unit::getPosition() {
    return *this->coordinates;
}

Circle Unit::getHitbox() {
    return *this->hitbox;
}

void Unit::setDestination(SDL_Point dest) {
    this->destination->x = dest.x;
    this->destination->y = dest.y;
}

void Unit::findPath(Level &level) {
    std::forward_list<Cell*> path;
    Cell* open[30][30] = {{nullptr}};
    Cell* closed[30][30] = {{nullptr}};

    Cell* startingCell = new Cell(this->coordinates->x, this->coordinates->y, this->destination);
    open[startingCell->x][startingCell->y] = startingCell;

    Cell* endingCell = nullptr;

    while (!endingCell) {
        Cell* promisingCell = nullptr;
        int minFCost = std::numeric_limits<int>::max();
        for (auto &openCells : open) {
            for (auto &openCell : openCells) {
                if (openCell) {
                    if (openCell->fCost < minFCost) {
                        minFCost = openCell->fCost;
                        promisingCell = openCell;
                    }
                }
            }
        }

        if (!promisingCell) {
            endingCell = startingCell;
            for (auto &closedCells : closed) {
                for (auto &closedCell : closedCells) {
                    if (closedCell) {
                        if (closedCell->hCost < endingCell->hCost) {
                            endingCell = closedCell;
                        }
                    }
                }
            }
            this->destination->x = endingCell->x;
            this->destination->y = endingCell->y;
            break;
       }

        open[promisingCell->x][promisingCell->y] = nullptr;
        closed[promisingCell->x][promisingCell->y] = promisingCell;

        for (int x = promisingCell->x - 1; x <= promisingCell->x + 1; ++x) {
            for (int y = promisingCell->y - 1; y <= promisingCell->y + 1; ++y) {
                if ((x == promisingCell->x && y == promisingCell->y) ||
                    (closed[x][y]) ||
                    (!level.squareIsFree(x, y))) {
                    continue;
                }

                Cell *tempCell = new Cell(x, y, this->destination, promisingCell);
                if (open[x][y]) {
                    if (open[x][y]->fCost > tempCell->fCost) {
                        delete open[x][y];
                        open[x][y] = tempCell;
                    } else {
                        delete tempCell;
                    }
                } else {
                    open[x][y] = tempCell;
                }

                if (open[x][y]->hCost == 0) {
                    endingCell = open[x][y];
                }
            }
        }
    }
    path.push_front(endingCell);
    while (endingCell->parent ? (bool)endingCell->parent->parent : false) {
        endingCell = endingCell->parent;
        path.push_front(endingCell);
    }
    this->next->x = endingCell->x;
    this->next->y = endingCell->y;

    for (int x = 0; x < 30; ++x) {
        for (int y = 0; y < 30; ++y) {
            delete open[x][y];
            delete closed[x][y];
        }
    }
}

void Unit::walk() {
    if (this->currentFrame < this->totalFrames - 1) {
        this->currentFrame += 1;
    } else {
        this->currentFrame = 0;
    }

    int dX = this->next->x - this->coordinates->x;
    int dY = this->next->y - this->coordinates->y;
    if (dX == 1 && dY == 1)
        this->direction = DOWN;
    else if (dX == 1 && dY == 0)
        this->direction = DOWNRIGHT;
    else if (dX == 1 && dY == -1)
        this->direction = RIGHT;
    else if (dX == 0 && dY == -1)
        this->direction = UPRIGHT;
    else if (dX == -1 && dY == -1)
        this->direction = UP;
    else if (dX == -1 && dY == 0)
        this->direction = UPLEFT;
    else if (dX == -1 && dY == 1)
        this->direction = LEFT;
    else if (dX == 0 && dY == 1)
        this->direction = DOWNLEFT;

    if (this->direction == DOWN || this->direction == DOWNLEFT || this->direction == DOWNRIGHT) {
        this->movementDelta->y += globals::TILE_HEIGHT / this->totalFrames;
    } else if (this->direction == UP || this->direction == UPLEFT || this->direction == UPRIGHT) {
        this->movementDelta->y -= globals::TILE_HEIGHT / this->totalFrames;
    }
    if (this->direction == RIGHT || this->direction == UPRIGHT || this->direction == DOWNRIGHT) {
        this->movementDelta->x += globals::TILE_WIDTH / this->totalFrames;
    } else if (this->direction == LEFT || this->direction == UPLEFT || this->direction == DOWNLEFT) {
        this->movementDelta->x -= globals::TILE_WIDTH / this->totalFrames;
    }


    if (    std::abs(this->movementDelta->x) == globals::TILE_WIDTH / 2 &&
            std::abs(this->movementDelta->y) == globals::TILE_HEIGHT / 2) {
        int mDXSign = this->movementDelta->x / std::abs(this->movementDelta->x);
        int mDYSign = this->movementDelta->y / std::abs(this->movementDelta->y);
        this->coordinates->x += (mDXSign + mDYSign) / 2;
        this->coordinates->y += (mDYSign - mDXSign) / 2;

        this->movementDelta->x = 0;
        this->movementDelta->y = 0;
    }
    if (std::abs(this->movementDelta->x) >= globals::TILE_WIDTH) {
        int mDXSign = this->movementDelta->x / std::abs(this->movementDelta->x);
        this->coordinates->x += 1 * mDXSign;
        this->coordinates->y -= 1 * mDXSign;

        this->movementDelta->x = 0;
    }
    if (std::abs(this->movementDelta->y) >= globals::TILE_HEIGHT) {
        int mDYSign = this->movementDelta->y / std::abs(this->movementDelta->y);
        this->coordinates->x += 1 * mDYSign;
        this->coordinates->y += 1 * mDYSign;

        this->movementDelta->y = 0;
    }
}

void Unit::walkBack() {
    if (this->currentFrame > 0) {
        this->currentFrame -= 1;
    } else {
        this->currentFrame = 15;
    }

    this->next->x = this->coordinates->x;
    this->next->y = this->coordinates->y;
    if (this->direction == DOWN || this->direction == DOWNLEFT || this->direction == DOWNRIGHT) {
        this->movementDelta->y -= globals::TILE_HEIGHT / this->totalFrames;
    } else if (this->direction == UP || this->direction == UPLEFT || this->direction == UPRIGHT) {
        this->movementDelta->y += globals::TILE_HEIGHT / this->totalFrames;
    }
    if (this->direction == RIGHT || this->direction == UPRIGHT || this->direction == DOWNRIGHT) {
        this->movementDelta->x -= globals::TILE_WIDTH / this->totalFrames;
    } else if (this->direction == LEFT || this->direction == UPLEFT || this->direction == DOWNLEFT) {
        this->movementDelta->x += globals::TILE_WIDTH / this->totalFrames;
    }
}
