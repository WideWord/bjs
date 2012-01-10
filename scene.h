#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include "engine.h"



namespace bjs {

    class Sprite {
        friend class gfx::Renderer;
    public:
        Sprite(gfx::Image* img);
        ~Sprite();
        void addFrame (gfx::Image* img);
        float x;
        float y;
        void move (float dx, float dy);
        void show ();
        void hide ();
        int z;
        void setSpeed (int s);
    private:
        bool onscr;
        std::vector<gfx::Image*> frames;
        float speed;
        int lTime;
        int fTime;
        unsigned frame;
    protected:
        gfx::Image* getFrame();

    };


    class Scene {
    friend class gfx::Renderer;
    public:
        Scene();
        ~Scene();
        void addSprite (Sprite* spr);
    private:
        std::vector<Sprite*> sprites;
    protected:
        size_t spriteLen();
        Sprite** spriteSort();
    };
}


#endif // SCENE_H_INCLUDED
