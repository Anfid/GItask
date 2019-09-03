#include <SDL2/SDL.h>

#include "Graphics.hpp"
#include "Input.hpp"
#include "Scroll.hpp"
#include "globals.hpp"

Scroll::Scroll()
    : delta {-950, 0}
{}

Scroll::~Scroll() {}

void Scroll::update(Graphics &graphics, Input &input) {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(graphics.getWindow(), &windowWidth, &windowHeight);

    if (windowWidth + this->delta.x < 1030 &&
            (input.getMousePos().x > windowWidth - globals::SCROLL_ZONE ||
            input.isKeyHeld(SDL_SCANCODE_D))) {
        this->delta.x += globals::SCROLL_SENSITIVITY;
    }
    if (windowHeight + this->delta.y < 1000 &&
            (input.getMousePos().y > windowHeight - globals::SCROLL_ZONE ||
            input.isKeyHeld(SDL_SCANCODE_S))) {
        this->delta.y += globals::SCROLL_SENSITIVITY;
    }
    if (this->delta.x > -970 &&
            (input.getMousePos().x < globals::SCROLL_ZONE ||
            input.isKeyHeld(SDL_SCANCODE_A))) {
        this->delta.x -= globals::SCROLL_SENSITIVITY;
    }
    if (this->delta.y > -100 &&
            (input.getMousePos().y < globals::SCROLL_ZONE ||
            input.isKeyHeld(SDL_SCANCODE_W))) {
        this->delta.y -= globals::SCROLL_SENSITIVITY;
    }
}

SDL_Point Scroll::getDelta() const {
    return this->delta;
}
