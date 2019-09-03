#ifndef SCROLL_H
#define SCROLL_H

#include <SDL2/SDL_rect.h>

class Graphics;
class Input;

class Scroll {
public:
    Scroll();
    ~Scroll();

    void update(Graphics &graphics, Input &input);
    SDL_Point getDelta() const;
private:
    SDL_Point delta;
};

#endif // SCROLL_H
