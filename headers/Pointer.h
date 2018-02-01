#ifndef POINTER_H
#define POINTER_H

struct SDL_Point;
class Graphics;
class Level;

class Pointer {
public:
    Pointer();
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

    SDL_Point* position;
    SDL_Point* tilePos;
};

#endif // POINTER_H
