#include <SDL.h>
#include "Input.h"


void Input::beginNewFrame(SDL_Event event) {
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    this->pressedKeys.clear();
    this->releasedKeys.clear();
    this->pressedButtons.clear();

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.repeat == 0) {
                this->keyDownEvent(event);
            }
        } else if (event.type == SDL_KEYUP) {
            this->keyUpEvent(event);
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            this->mouseButtonEvent(event);
        } else if (event.type == SDL_QUIT) {
            return;
        }
    }
}

void Input::keyDownEvent(const SDL_Event &event) {
    this->pressedKeys[event.key.keysym.scancode] = true;
    this->heldKeys[event.key.keysym.scancode] = true;
}

void Input::keyUpEvent(const SDL_Event &event) {
    this->releasedKeys[event.key.keysym.scancode] = true;
    this->heldKeys[event.key.keysym.scancode] = false;
}

void Input::mouseButtonEvent(const SDL_Event &event) {
    this->pressedButtons[event.button.button] = true;
}

SDL_Point Input::getMousePos() {
    return this->mousePos;
}

bool Input::wasKeyPressed(SDL_Scancode key) {
    return this->pressedKeys[key];
}

bool Input::wasKeyReleased(SDL_Scancode key) {
    return this->releasedKeys[key];
}

bool Input::isKeyHeld(SDL_Scancode key) {
    return this->heldKeys[key];
}

bool Input::wasButtonPressed(int button) {
    return this->pressedButtons[button];
}
