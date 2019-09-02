#ifndef CANNON_H
#define CANNON_H

#include <list>
#include "Circle.h"
#include "globals.h"

struct SDL_Texture;
class Graphics;
class Scroll;
struct Fireball;

class Cannon {
public:
    Cannon();
    Cannon(Graphics &graphics, SDL_Point coordinates, int fireFrequency = 3000, int fireballLifetime = 4000,
           int fireballSpeed = 4);
    ~Cannon();

    void update(int elapsedTime, Scroll &scroll);
    void draw(Graphics &graphics, Scroll &scroll);

    SDL_Point getPosition();
    std::list<Fireball*> getFireballs();

private:
    SDL_Texture* spriteSheet;

    SDL_Point coordinates;
    Direction direction;

    std::list<Fireball*> fireballs;
    int fireFrequency;
    int timeAfterShot;
    int maxFireballLifetime;
    int fireballSpeed;

    int timeAfterUpdate;
    int timeToUpdate;
};

struct Fireball {
    Fireball(int x, int y);
    int x, y;
    int lifetime;
    Circle hitbox;
};

#endif // CANNON_H
