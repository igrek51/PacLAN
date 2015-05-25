#include "animation.h"
#include "app.h"

Animation::Animation(int x, int y){
    this->x = x;
    this->y = y;
    texture = NULL;
    clip = NULL;
    ends = 1;
}

Animation::~Animation(){
    App::graphics->destroy_texture(texture);
}

//animacja śmierci Playera
AnimationDeath::AnimationDeath(int x, int y, SDL_Texture *texture, SDL_Color color, int *clip) : Animation(x, y){
    this->texture = App::graphics->blend_texture(texture, Graphics::rgba(200,200,200));
    this->color = color;
    this->clip = clip;
    ends = 60;
}

AnimationDeath::~AnimationDeath(){
    delete[] clip;
}

void AnimationDeath::animate(){
    ends--;
    SDL_SetTextureAlphaMod(texture,ends*255/60);
}

void AnimationDeath::draw(){
    App::graphics->draw_texture_clip_center(texture, x, y, clip[0], clip[1], clip[2], clip[3]);
}
