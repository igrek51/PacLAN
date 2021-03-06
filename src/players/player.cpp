#include "player.h"
#include "../config.h"
#include "../log/log.h"
#include "../app.h"
#include "../utils.h"

Player::Player(int xmap, int ymap, SDL_Color color, string name, int controlby,
               GameEngine* game_engine, Graphics* graphics) {
    this->xmap = xmap;
    this->ymap = ymap;
    this->color = color;
    this->name = name;
    this->controlby = controlby;
    this->game_engine = game_engine;
    this->graphics = graphics;
    texture = nullptr;
    game_engine->xymap_to_xy(this);
    move_abs(x, y);
    can_move = 0;
    direction = P_RIGHT;
    next_direction = direction;
    a_sprite = 0;
    a_cycle = 0;
    moving = 0;
    next_moving = moving;
    prev_direction = -1;
    prev_moving = -1;
    clip = new int[4];
    sciezka = nullptr;
    score = 0;
    ai_level = 0;
    respawn = 0;
    lan_id = 0;
}

Player::~Player() {
    if (texture != nullptr)
        graphics->destroy_texture(texture);
    delete[] clip;
    if (sciezka != nullptr)
        delete sciezka;
}

void Player::move() {
    if (graphics->fps > 0) {
        a_dx = (((x - a_x) * 1000)) / (graphics->fps * Config::logic_timer_ms * can_move_n);
        a_dy = (((y - a_y) * 1000)) / (graphics->fps * Config::logic_timer_ms * can_move_n);
    } else {
        Log::warn("Player::move: fps = 0");
        a_x = x;
        a_y = y;
    }
}

void Player::move_abs(int absx, int absy) {
    x = absx;
    y = absy;
    a_x = x;
    a_y = y;
    a_dx = 0;
    a_dy = 0;
}

void Player::move_random() {
    next_moving = 1;
    next_direction = rand() % 4;
}

void Player::move_smooth() {
    if ((a_x < x && a_x + a_dx >= x) || (a_x > x && a_x + a_dx <= x)) {
        a_x = x;
        a_dx = 0;
    } else {
        a_x += a_dx;
    }
    if ((a_y < y && a_y + a_dy >= y) || (a_y > y && a_y + a_dy <= y)) {
        a_y = y;
        a_dy = 0;
    } else {
        a_y += a_dy;
    }
}

void Player::draw_sprite_at(SDL_Texture* texture2, int x, int y) {
    clip_table();
    if (respawn > 0) {
        SDL_SetTextureAlphaMod(texture2, 80);
        //liczba do końca respawnu
        stringstream ss;
        ss << Timer::cycles_to_s(respawn);
        graphics->draw_text(ss.str(), graphics->font2,
                            graphics->rgba(color.r, color.g, color.b, 255), x, y - 6, 0);
    } else {
        SDL_SetTextureAlphaMod(texture2, 255);
    }
    graphics->draw_texture_clip_center(texture2, x, y, clip[0], clip[1], clip[2], clip[3]);
}

void Player::draw_sprite(SDL_Texture* texture2) {
    draw_sprite_at(texture2, round_to_int(a_x), round_to_int(a_y));
}

void Player::draw_name() {
    graphics->draw_text(name, graphics->font1, graphics->rgba(color.r, color.g, color.b, 150),
                        round_to_int(a_x), round_to_int(a_y - 30), 0);
}
