#include "item.h"

Item::Item(int xmap, int ymap, int subclass, GameEngine* game_engine, Graphics* graphics){
    this->xmap = xmap;
    this->ymap = ymap;
    this->subclass = subclass;
    this->game_engine = game_engine;
    this->graphics = graphics;
}

Item::~Item(){

}

void Item::draw(){
    x = game_engine->xmap_to_x(xmap);
    y = game_engine->ymap_to_y(ymap);
    if(subclass == I_SMALLDOT){ //mała kropka
        graphics->draw_texture_center(graphics->tex("dot1"), x, y);
    }else if(subclass == I_LARGEDOT){ //duża kropka bonusowa
        graphics->draw_texture_center(graphics->tex("dot2"), x, y);
    }
}
