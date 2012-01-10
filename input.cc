#include "input.h"
#include <SDL.h>

using namespace input;

InputDevice::InputDevice() {
    for (unsigned i = 0; i < 256; i++){
        keys[i] = false;
        keyDownEvent[i] = false;
        keyUpEvent[i] = false;
    }
    mouseX = 0;
    mouseY = 0;
    for (unsigned i = 0; i < 3; i++) {
        mouseDownEvent[i] = false;
        mouseUpEvent[i] = false;
        mouseButton[i] = false;
    }
}

bool InputDevice::update() {
    bool result = true;
    SDL_Event event;

    while ( SDL_PollEvent(&event) ) {
        switch (event.type) {
            case SDL_MOUSEMOTION:
                mouseX = event.motion.x;
                mouseY = event.motion.y;
            break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button >= 1 && event.button.button <= 3) {
                    mouseDownEvent[event.button.button - 1] = true;
                    mouseButton[event.button.button - 1] = true;
                }
            break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button >= 1 && event.button.button <= 3) {
                    mouseUpEvent[event.button.button - 1] = true;
                    mouseButton[event.button.button - 1] = false;
                }
            break;
            case SDL_KEYDOWN:
                keys[event.key.keysym.sym] = true;
                keyDownEvent[event.key.keysym.sym] = true;
                break;
            case SDL_KEYUP:

                keys[event.key.keysym.sym] = false;
                keyUpEvent[event.key.keysym.sym] = true;
                break;
            case SDL_QUIT:
                result = false;

        }

    }
    return result;
}

bool InputDevice::getKey(int keycode) {
    return keys[keycode];
}

bool InputDevice::getKeyDown(int keycode) {
    bool res = keyDownEvent[keycode];
    keyDownEvent[keycode] = false;
    return res;
}

bool InputDevice::getKeyUp(int keycode) {
    bool res = keyUpEvent[keycode];
    keyUpEvent[keycode] = false;
    return res;
}

int InputDevice::getMouseX () {
    return mouseX;
}

int InputDevice::getMouseY () {
    return mouseY;
}

bool InputDevice::getMouse(int keycode) {
    return mouseButton[keycode];
}

bool InputDevice::getMouseDown(int keycode) {
    bool res = mouseDownEvent[keycode];
    mouseDownEvent[keycode] = false;
    return res;
}

bool InputDevice::getMouseUp(int keycode) {
    bool res = mouseUpEvent[keycode];
    mouseUpEvent[keycode] = false;
    return res;
}

