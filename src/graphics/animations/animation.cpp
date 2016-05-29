#include "animation.h"
#include "../../app.h"

Animation::Animation(int x, int y){
    this->x = x;
    this->y = y;
    texture = nullptr;
    clip = nullptr;
    ends = 1;
}

Animation::~Animation(){
    App::graphics->destroy_texture(texture);
}
