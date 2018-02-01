#ifndef GAME_H
#define GAME_H

#include <vector>

class Graphics;
class Input;
class Scroll;
class Unit;
class Player;
class Cannon;
class Level;
class Pointer;

class Game {
public:
    Game();
    ~Game();

private:
    void gameLoop();
    void draw(Graphics &graphics, Scroll &scroll);
    void update(Graphics &graphics, Input &input, Scroll &scroll, int elapsedTime);
    void collide();
    void reset();

    Player* player;
    Pointer* pointer;
    Level* level;
    std::vector<Unit*> units;
    std::vector<Cannon*> cannons;

    bool defeat;
    bool victory;
};

#endif // GAME_H
