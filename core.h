#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED


#include "gfx.h"
#include "input.h"


namespace bjs {
    class Core  {
    public:
        gfx::Renderer* getRenderer();
        input::InputDevice* getInputDevice();
        bool run();
        Core(int w, int h, bool full);
        ~Core();
        void exit ();
    private:
        gfx::Renderer* renderer;
        input::InputDevice* inputDevice;
        bool running;

    };


}

#endif // CORE_H_INCLUDED
