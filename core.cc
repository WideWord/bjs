#include "core.h"

using namespace gfx;
using namespace input;
using namespace bjs;

Core::Core(int w, int h, bool full) {
    running = true;
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError());
        return;
    }


    renderer = new Renderer( w, h, full);
    inputDevice = new InputDevice();
}

Renderer* Core::getRenderer() {
    return renderer;
}

InputDevice* Core::getInputDevice() {
    return inputDevice;
}

bool Core::run () {
    if (!running) {
        return false;
    }
    renderer->render();
    return inputDevice->update();
}

void Core::exit() {
    running = false;
    delete renderer;
    delete inputDevice;
}

Core::~Core() {
    delete renderer;
    delete inputDevice;
}


