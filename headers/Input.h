#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <SDL.h>

/* Input class
 * Keeps track of keyboard state
 */
class Input {
public:
    /* void beginNewFrame()
     * Called at the beginning of each frame to reset no longer relevant keys
     */
    void beginNewFrame(SDL_Event event);
    void keyUpEvent(const SDL_Event &event);
    void keyDownEvent(const SDL_Event &event);

    void mouseButtonEvent(const SDL_Event &event);
    SDL_Point getMousePos();

    bool wasKeyPressed(SDL_Scancode key);
    bool wasKeyReleased(SDL_Scancode key);
    bool isKeyHeld(SDL_Scancode key);

    bool wasButtonPressed(int button);
private:
    SDL_Point mousePos;
    std::map<SDL_Scancode, bool> heldKeys;
    std::map<SDL_Scancode, bool> pressedKeys;
    std::map<SDL_Scancode, bool> releasedKeys;
    std::map<int, bool> pressedButtons;
};

#endif // INPUT_H
