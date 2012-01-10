#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

namespace input {
    class InputDevice {
    public:
        bool update ();
        bool getKey(int keycode);
        bool getKeyDown (int keycode);
        bool getKeyUp(int keycode);
        bool getMouse(int button);
        bool getMouseDown(int button);
        bool getMouseUp(int button);
        int getMouseX();
        int getMouseY();
        InputDevice();
    private:
        bool keys[256];
        bool keyDownEvent[256];
        bool keyUpEvent[256];
        int mouseX , mouseY;
        bool mouseDownEvent[3];
        bool mouseUpEvent[3];
        bool mouseButton[3];
    };
}

#endif // INPUT_H_INCLUDED
