#ifndef UNIT_H
#define UNIT_H

#include <string>
#include "globals.h"
#include "Circle.h"

struct SDL_Texture;
class Graphics;
class Scroll;
class Level;

struct Circle;

class Unit {
public:
    Unit() = delete;
    Unit(Unit&) = delete;
    Unit(Graphics &graphics, const std::string &filepath, SDL_Point coordinates,
         int frameTime = 100, int totalFrames = 16);
    virtual ~Unit();

    virtual void draw(Graphics &graphics, Scroll &scroll);
    virtual void update(int elapsedTime, Level &level);

    SDL_Point getPosition();
    Circle getHitbox();

protected:
    SDL_Texture* spriteSheet;

    SDL_Point coordinates;
    Direction direction;
    SDL_Point next;
    SDL_Point destination;

    SDL_Point movementDelta;

    Circle hitbox;

    int currentFrame;
    int totalFrames;
    int timeAfterNewFrame;
    int frameTime;

    virtual void setDestination(SDL_Point dest);

    void findPath(Level &level);
    void walk();
    void walkBack();
};

#endif // UNIT_H
