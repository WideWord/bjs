#include "core.h"
#include <iostream>


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
    timer = new Timer();
}

Renderer* Core::getRenderer() {
    return renderer;
}

InputDevice* Core::getInputDevice() {
    return inputDevice;
}

Timer* Core::getTimer() {
    return timer;
}

bool Core::run () {
    if (!running) {
        return false;
    }
    renderer->render();
    timer->frame();
    return inputDevice->update();
    return false;
}

void Core::exit() {
    running = false;
    delete renderer;
    delete inputDevice;
    delete timer;
}

Core::~Core() {
    delete renderer;
    delete inputDevice;
}


