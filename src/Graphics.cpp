#include <SDL.h>
#include <SDL_image.h>

#include "Graphics.h"
#include "globals.h"

Graphics::Graphics() {
    SDL_CreateWindowAndRenderer(globals::WINDOW_WIDTH, globals::WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &this->window, &this->renderer);
    SDL_SetWindowTitle(this->window, "Game");
}

Graphics::~Graphics() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
}

SDL_Surface* Graphics::loadImage(const std::string &filepath) {
    if (this->spriteSheets.count(filepath) == 0) {
        this->spriteSheets[filepath] = IMG_Load(filepath.c_str());
    }
    return this->spriteSheets[filepath];
}

void Graphics::blitSurface(SDL_Texture *source, SDL_Rect *sourceRect, SDL_Rect *destRect) {
    SDL_RenderCopy(this->renderer, source, sourceRect, destRect);
}

void Graphics::flip() {
    SDL_RenderPresent(this->renderer);
}

void Graphics::clear() {
    SDL_RenderClear(this->renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
    return this->renderer;
}

SDL_Window* Graphics::getWindow() const {
    return this->window;
}
