#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED


#include "gfx.h"
#include "input.h"
#include "timer.h"
#include <Box2D/Box2D.h>


namespace bjs {
    class Core  {
    public:
        gfx::Renderer* getRenderer();
        input::InputDevice* getInputDevice();
        Timer* getTimer();
        bool run();
        Core(int w, int h, bool full);
        ~Core();
        void exit ();
    private:
        gfx::Renderer* renderer;
        input::InputDevice* inputDevice;
        Timer* timer;
        bool running;

    };


}

#endif // CORE_H_INCLUDED
