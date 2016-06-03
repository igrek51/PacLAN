#include "death_animation.h"
#include "../../app.h"

DeathAnimation::DeathAnimation(int x, int y, SDL_Texture *texture, SDL_Color color, int *clip, Graphics* graphics) : Animation(x, y, graphics){
    this->texture = App::graphics->blend_texture(texture, Graphics::rgba(200,200,200));
    this->color = color;
    this->clip = clip;
    ends = 60;
}

DeathAnimation::~DeathAnimation(){
    delete[] clip;
}

void DeathAnimation::animate(){
    ends--;
    SDL_SetTextureAlphaMod(texture,ends*255/60);
}

void DeathAnimation::draw(){
    App::graphics->draw_texture_clip_center(texture, x, y, clip[0], clip[1], clip[2], clip[3]);
}
