#include "script.h"


int main() {
    runScript("main.js");


    /*Core* c = new Core(800,600, false);
    Scene* scn = new Scene();

    Image* habrI = new Image("img.bmp");

    for(unsigned i = 0; i < 10000; i++) {
        Sprite* habrS = new Sprite(habrI);
        scn->addSprite(habrS);
        //habrS->move(i * habrI->w, 0);
    }



    c->getRenderer()->setScene(scn);



    while(c->run()) {
       // if (c->getInputDevice()->getKey(27))c->exit();
    }
*/
    return 1;
}
