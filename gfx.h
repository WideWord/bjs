#ifndef GFX_H_INCLUDED
#define GFX_H_INCLUDED

#include <SDL.h>
#include <vector>


namespace gfx {



    class Image {
        friend class Renderer;
    public:
        Image(const char* filename);
        ~Image();
        int w, h;
    protected:
        SDL_Surface* data;
        unsigned texture;
    };

    class Sprite {
        friend class Renderer;
    public:
        Sprite(Image* img);
        ~Sprite();
        void addFrame (Image* img);
        float x;
        float y;
        void move (float dx, float dy);
        void show ();
        void hide ();
        int z;
        void setSpeed (float s);
    private:
        bool onscr;
        std::vector<Image*> frames;
        float speed;
        int lTime;
        int fTime;
        unsigned frame;
    protected:
        Image* getFrame();

    };


    class Scene {
    friend class Renderer;
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

    class Renderer {
    public:
        Renderer(int w, int h, bool full);
        ~Renderer();
        void render ();
        void setScene (Scene* scn);
    private:
        SDL_Surface* screen;
        Scene* scene;
    };

}


#endif // GFX_H_INCLUDED
