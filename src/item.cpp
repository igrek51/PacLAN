#include "item.h"
#include "app.h"

Item::Item(int xmap, int ymap, int subclass){
    this->xmap = xmap;
    this->ymap = ymap;
    this->subclass = subclass;
}

Item::~Item(){

}

void Item::draw(){
    x = App::game_engine->xmap_to_x(xmap);
    y = App::game_engine->ymap_to_y(ymap);
    if(subclass == I_SMALLDOT){ //mała kropka
        App::graphics->draw_texture_center(App::graphics->tex("dot1"), x, y);
    }else if(subclass == I_LARGEDOT){ //duża kropka bonusowa
        App::graphics->draw_texture_center(App::graphics->tex("dot2"), x, y);
    }
}
