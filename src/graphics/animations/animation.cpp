#include "animation.h"

Animation::Animation(int x, int y, Graphics* graphics) {
    this->x = x;
    this->y = y;
    this->graphics = graphics;
    texture = nullptr;
    clip = nullptr;
    ends = 1;
}

Animation::~Animation() {
    graphics->destroy_texture(texture);
}
