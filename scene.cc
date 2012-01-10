#include "scene.h"

using namespace bjs;

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

void Sprite::setSpeed (int s) {
    speed = s;
}
