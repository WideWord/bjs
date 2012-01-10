#include "gfx.h"
#include <SDL_image.h>
#include <GL/gl.h>
#include <iostream>
#include <stdlib.h>

// timer FPS
int last = 0;
int framerate= 0;
bool GetTime () {
    int now = SDL_GetTicks();
    int res = now - last;

    if (res > 1000) {
        last = now;
        return true;
    }
    return false;
}



using namespace gfx;

Renderer::Renderer(int w, int h, bool full) {



    screen = NULL;


    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);   //use 24 if BPP is 32
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 0);



    unsigned flags = SDL_OPENGL;

    if (full)flags |= SDL_FULLSCREEN;

    screen = SDL_SetVideoMode(w, h, 32, flags);

    if (!screen) printf("Can't set videomode: %s\n", SDL_GetError());

    if(IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG)) printf ("Unable to init SDL image loader: %s\n", IMG_GetError());


    //disable unused features for 2D//
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    // This allows alpha blending of 2D textures with the scene //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    //setup viewport & ortho mode to emulate standard 2D API conventions
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0.0, (GLdouble)w, (GLdouble)h, 0.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glClearColor(0.0f,0.0f,0.0f,1.0f);
}

Renderer::~Renderer() {
    SDL_Quit();
    IMG_Quit();
}

void Renderer::render () {
    Sprite** sprites = scene->spriteSort();
    size_t len = scene->spriteLen();


    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    for (unsigned i = 0; i < len; i++) {
        Sprite* spr = sprites[i];
        if(!spr->onscr)continue;
        Image* img = spr->getFrame();
        float x = spr->x;
        float y = spr->y;
        float w = img->w;
        float h = img->h;

        //glColor4ub(255,255,255,225);
        glBindTexture(GL_TEXTURE_2D, img->texture);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(0,0);    glVertex2f(x,y);
            glTexCoord2f(1,0);    glVertex2f(x+w,y);
            glTexCoord2f(0,1);    glVertex2f(x,y+h);
            glTexCoord2f(1,1);    glVertex2f(x+w,y+h);
        glEnd();
        //glColor4ub(255,255,255,255);
    }

    SDL_GL_SwapBuffers();

    //TimerFPS
    framerate++;
    if (GetTime()) {
        printf("FPS: %d\n", framerate);
        framerate = 0;
    }
}

void Renderer::setScene (Scene* scn) {
    scene = scn;
}



Scene::Scene () {

}

Scene::~Scene() {

}

size_t Scene::spriteLen () {
    return sprites.size();
}

int compare (const void * a, const void * b)
{
    return (((Sprite*)a)->z) - (((Sprite*)b)->z);
}

Sprite** Scene::spriteSort () {
    Sprite** res = new Sprite*[sprites.size()];

    for (unsigned i = 0; i < sprites.size(); i++) res[i] = sprites[i];


    //qsort (res, sprites.size(), sizeof(int), compare);

    return res;
}

void Scene::addSprite (Sprite* spr) {
    sprites.push_back(spr);
}




Sprite::Sprite (Image* img) {
    frames.push_back(img);
    x = 0;
    y = 0;
    z = 0;
    onscr = true;
    lTime = SDL_GetTicks();
    frame = 0;
}

Sprite::~Sprite() {

}

void Sprite::addFrame (Image* img) {
    frames.push_back(img);
}

void Sprite::move (float dx, float dy) {
    x += dx;
    y += dy;
}

void Sprite::show () {
    onscr = true;
}

void Sprite::hide () {
    onscr = false;
}

void Sprite::setSpeed (float s) {
    speed = s;
}

Image* Sprite::getFrame() {
    if (frames.size() == 1) return frames[0];
    int now = SDL_GetTicks();
    fTime += now - lTime;
    lTime = now;
    if (fTime > speed) {
        fTime = 0;
        frame++;
        if (frame >= frames.size()) frame = 0;
    }
    return frames[frame];
}




Image::Image (const char* filename) {
    data = NULL;
    data = IMG_Load(filename);

    w = data->w;
    h = data->h;

    GLenum texture_format;

    int nOfColors = data->format->BytesPerPixel;

    if (nOfColors == 4)
    {
        if (data->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    } else if (nOfColors == 3) {
        if (data->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    } else {
        printf("Warning: the image is not truecolor.. nOfColors = %d \n", nOfColors);
        return;
    }


    glGenTextures (1, &texture);

	glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, data->w, data->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, data->pixels );
}

Image::~Image () {
    delete data;
}
