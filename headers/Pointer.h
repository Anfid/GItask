#ifndef POINTER_H
#define POINTER_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

class Graphics;
class Level;

class Pointer {
public:
    Pointer() = delete;
    Pointer(Pointer&) = delete;
    Pointer(Graphics &graphics, const std::string &filepath);
    ~Pointer();

    void draw(Graphics &graphics);
    void update(SDL_Point newPos, SDL_Point scrollDelta, bool isFree);

    SDL_Point getPosition() const;
    SDL_Point getTilePos() const;

private:
    bool free;

    SDL_Rect sourceRect;
    SDL_Texture* spriteSheet;

    SDL_Point position;
    SDL_Point tilePos;
};

#endif // POINTER_H
