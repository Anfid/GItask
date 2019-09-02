#include <SDL2/SDL.h>
#include <cmath>

#include "Game.h"
#include "Input.h"
#include "Graphics.h"
#include "Scroll.h"
#include "Level.h"
#include "Pointer.h"
#include "Player.h"
#include "Enemy.h"
#include "Cannon.h"

namespace {
    const int FPS = 50;
    const int FRAME_TIME = 1000 / FPS;
    const int MAX_FRAME_TIME = 5 * 1000 / FPS;
}

Game::Game() {
    SDL_Init(SDL_INIT_EVERYTHING);
    this->player = nullptr;
    this->pointer = nullptr;
    this->level = nullptr;
    this->gameLoop();
}

Game::~Game() {
    this->reset();
    delete this->pointer;
}

void Game::gameLoop() {
    // ------------------------------ Setup ------------------------------

    SDL_Event event;

    Graphics graphics;
    Scroll scroll;
    Input input;

    this->pointer = new Pointer(graphics, "../content/tiles.png");

    while (true) {
        this->reset();

        this->pointer = new Pointer(graphics, "../content/tiles.png");
        this->level = new Level(graphics);

        this->player = new Player(graphics, "../content/scout.png", this->level->getSpawn().x, this->level->getSpawn().y);
        this->units.push_back(this->player);

        // Number of robots and complexity of their route can vary
        this->units.push_back(new Enemy(graphics, "../content/patrol.png", 14, 14, *this->player));

        SDL_Point *route[] = {new SDL_Point{20, 20},
                              new SDL_Point{10, 20},
                              new SDL_Point{10, 10},
                              new SDL_Point{20, 10}};
        this->units.push_back(new Enemy(graphics, "../content/patrol.png", 20, 20, 4, route));

        this->cannons.push_back(new Cannon(graphics, SDL_Point{0, 10}));
        this->cannons.push_back(new Cannon(graphics, SDL_Point{0, 20}));
        this->cannons.push_back(new Cannon(graphics, SDL_Point{10, 0}));
        this->cannons.push_back(new Cannon(graphics, SDL_Point{20, 0}));

        int LAST_UPDATE_TIME = SDL_GetTicks();


        // ------------------------------ Game loop ------------------------------
        while (true) {
            input.beginNewFrame(event);

            if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
                return;
            }

            if (input.wasButtonPressed(SDL_BUTTON_LEFT)) {
                player->setDestination(globals::isoToCoord(pointer->getTilePos(), scroll.getDelta()));
            }
            if (input.wasButtonPressed(SDL_BUTTON_RIGHT)) {
                level->changeSquareState(globals::isoToCoord(pointer->getTilePos(), scroll.getDelta()), this->units);
            }
            if (    (this->victory || this->defeat) &&
                    input.wasKeyPressed(SDL_SCANCODE_SPACE)) {
                this->victory = false;
                this->defeat = false;
                break;
            }

            if (!this->defeat) {
                const int CURRENT_TIME_MS = SDL_GetTicks();
                int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
                this->update(graphics, input, scroll, std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
                LAST_UPDATE_TIME = CURRENT_TIME_MS;
            }
        }
    }
}

void Game::draw(Graphics &graphics, Scroll &scroll) {
    graphics.clear();

    this->level->drawGround(graphics, scroll);

    SDL_Point levelSize = this->level->getSize();
    for (int i = 0; i < levelSize.x + levelSize.y; ++i) {
        int x = i < levelSize.y ? 0 : i - levelSize.x + 1;
        int y = i < levelSize.y ? i : levelSize.y - 1;
        while (x < levelSize.x && y >= 0) {

            this->level->drawMap(graphics, scroll, x, y);

            for (auto &unit : this->units) {
                if (unit->getPosition().x == x && unit->getPosition().y == y)
                    unit->draw(graphics, scroll);
            }
            ++x; --y;
        }
    }

    for (auto &cannon : this->cannons) {
        cannon->draw(graphics, scroll);
    }

    this->pointer->draw(graphics);

    graphics.flip();
}

void Game::update(Graphics &graphics, Input &input, Scroll &scroll, int elapsedTime) {
    static int timeAfterUpdate;
    timeAfterUpdate += elapsedTime;

    for (auto unit : this->units) {
        unit->update(elapsedTime, *level);
    }

    if (this->player->getPosition().x == this->level->getFinish().x &&
            this->player->getPosition().y == this->level->getFinish().y) {
        this->victory = true;
        for (auto unit : this->units) {
            if (unit != this->player) {
                delete unit;
            }
        }
        this->units.resize(1);
        for (auto cannon : this->cannons) {
            delete cannon;
        }
        this->cannons.clear();
    }

    for (auto cannon : this->cannons) {
        cannon->update(elapsedTime, scroll);
    }

    this->pointer->update(input.getMousePos(), scroll.getDelta(),
                          level->squareIsFree(globals::isoToCoord(pointer->getTilePos(), scroll.getDelta()).x,
                                              globals::isoToCoord(pointer->getTilePos(), scroll.getDelta()).y));

    if (timeAfterUpdate > FRAME_TIME) {
        scroll.update(graphics, input); // restrained with FPS for smoother scroll
        this->draw(graphics, scroll);
        this->collide();
        timeAfterUpdate -= FRAME_TIME;
    }
};

void Game::collide() {
    for (auto unit : units) {
        if (unit == this->player) continue;
        int distanceX = std::abs(this->player->getHitbox().x - unit->getHitbox().x);
        int distanceY = std::abs(this->player->getHitbox().y - unit->getHitbox().y);
        int distance = (int)sqrt(pow(distanceX, 2) + pow(distanceY, 2));
        if (distance < this->player->getHitbox().r + unit->getHitbox().r) {
            this->defeat = true;
        }
    }
    for (auto &cannon : cannons) {
        for (auto &fireball : cannon->getFireballs()) {
            int distanceX = std::abs(this->player->getHitbox().x - fireball->hitbox.x);
            int distanceY = std::abs(this->player->getHitbox().y - fireball->hitbox.y);
            int distance = (int) sqrt(pow(distanceX, 2) + pow(distanceY, 2));
            if (distance < this->player->getHitbox().r + fireball->hitbox.r) {
                this->defeat = true;
            }
        }
    }
}

void Game::reset() {
    this->defeat = false;

    delete this->level;
    this->level = nullptr;

    for (auto unit : this->units) {
        delete unit;
    }
    this->units.clear();
    this->player = nullptr;

    for (auto cannon : this->cannons) {
        delete cannon;
    }
    this->cannons.clear();
}
