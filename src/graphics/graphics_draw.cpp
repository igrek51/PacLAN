#include "graphics.h"
#include "../log/log.h"
#include "../config.h"
#include "../utils.h"

void Graphics::animate() {
    if (game_engine->menu > 0) { //MENU
        game_engine->menu_pacman->animate();
        game_engine->menu_ghost->animate();
    } else { //MAPKA
        for (unsigned int i = 0; i < game_engine->players.size(); i++) {
            game_engine->players[i]->animate();
        }
        for (unsigned int i = 0; i < animations.size(); i++) {
            if (animations[i]->ends <= 0) {
                //usunięcie skończonej animacji
                delete animations[i];
                animations.erase(animations.begin() + i);
                i--;
                continue;
            }
            animations[i]->animate();
        }
    }
}

void Graphics::draw_menu() {
    //podpis
    draw_text("Created by Ireneusz Szulc", font2, rgba(40, 40, 40), wnd_w, wnd_h - 15, TEXT_RIGHT);
    stringstream ss;
    ss << "P a c L A N  v" << Config::version;
    draw_text(ss.str(), font2, rgba(255, 255, 0), wnd_w / 2, 45, TEXT_CENTER);
    //obrazek gracza
    Player* menu_player;
    if (game_engine->menu_subclass == P_PACMAN) {
        menu_player = game_engine->menu_pacman;
    } else {
        menu_player = game_engine->menu_ghost;
    }
    menu_player->draw_sprite_at(menu_player->texture, wnd_w / 2, 100);
    ss_clear(ss);
    ss << getLanguage()->player_name << game_engine->menu_name;
    if (game_engine->menu == MENU_NAME && game_engine->cycles / 10 % 2 == 0) {
        ss << "_";
    } else {
        ss << "  ";
    }
    draw_text(ss.str(), font1, rgba(255, 255, 255), wnd_w / 2, 140, TEXT_CENTER);
    ss_clear(ss);
    ss << getLanguage()->player_class <<
    (game_engine->menu_subclass == P_PACMAN ? getLanguage()->player_class_pacman
                                            : getLanguage()->player_class_ghost);
    draw_text(ss.str(), font1, rgba(255, 255, 255), wnd_w / 2, 170, TEXT_CENTER);
    ss_clear(ss);
    ss << getLanguage()->player_color << "(" << (int) game_engine->menu_color.r << ", " <<
    (int) game_engine->menu_color.g << ", " << (int) game_engine->menu_color.b << ")";
    draw_text(ss.str(), font1, game_engine->menu_color, wnd_w / 2, 200, TEXT_CENTER);
    ss_clear(ss);
    ss << getLanguage()->host_game;
    if (game_engine->mode == MODE_CLASSIC) ss << getLanguage()->mode_classic;
    if (game_engine->mode == MODE_ZOMBIE) ss << getLanguage()->mode_zombie;
    draw_text(ss.str(), font1, rgba(255, 255, 255), wnd_w / 2, 230, TEXT_CENTER);
    ss_clear(ss);
    ss << getLanguage()->connect_to << game_engine->menu_ip;
    if (game_engine->menu == MENU_JOIN && game_engine->cycles / 10 % 2 == 0) {
        ss << "_";
    } else {
        ss << "  ";
    }
    draw_text(ss.str(), font1, rgba(255, 255, 255), wnd_w / 2, 260, TEXT_CENTER);
    ss_clear(ss);
    ss << getLanguage()->language << getLanguage()->lang_name;
    draw_text(ss.str(), font1, rgba(255, 255, 255), wnd_w / 2, 290, TEXT_CENTER);
    draw_text(getLanguage()->exit, font1, rgba(255, 255, 255), wnd_w / 2, 320, TEXT_CENTER);
    //kursor
    draw_fill_rect(260, 135 + (game_engine->menu - 1) * 30, 280, 26, rgba(150, 150, 150, 40));
    game_engine->menu_pacman->draw_sprite_at(game_engine->menu_pacman->texture, 260,
                                             148 + (game_engine->menu - 1) * 30);
    game_engine->menu_ghost->draw_sprite_at(game_engine->menu_ghost->texture, 540,
                                            148 + (game_engine->menu - 1) * 30);
    draw_text(getLanguage()->controls, font1, rgba(120, 120, 120), wnd_w / 2, 350, TEXT_CENTER);
    draw_text(getLanguage()->controls_1, font1, rgba(120, 120, 120), wnd_w / 2, 370, TEXT_CENTER);
    draw_text(getLanguage()->controls_2, font1, rgba(120, 120, 120), wnd_w / 2, 385, TEXT_CENTER);
    draw_text(getLanguage()->controls_3, font1, rgba(120, 120, 120), wnd_w / 2, 400, TEXT_CENTER);
    draw_text(getLanguage()->controls_4, font1, rgba(120, 120, 120), wnd_w / 2, 415, TEXT_CENTER);
    draw_text(getLanguage()->controls_5, font1, rgba(120, 120, 120), wnd_w / 2, 430, TEXT_CENTER);
    draw_text(getLanguage()->controls_6, font1, rgba(120, 120, 120), wnd_w / 2, 445, TEXT_CENTER);
}

void Graphics::draw_round() {
    //tło - mapka
    draw_texture_center(game_engine->map->background, wnd_w / 2, wnd_h / 2);
    //siatka pól mapy
    if (Config::show_grid) {
        ccolor = rgba(0, 0, 255, 50);
        for (int i = 0; i <= game_engine->map->grid_w; i++) {
            draw_line((int) (game_engine->map->grid_space * i + game_engine->map->grid_x_pos), 0,
                      (int) (game_engine->map->grid_space * i + game_engine->map->grid_x_pos),
                      wnd_h, ccolor);
        }
        for (int j = 0; j <= game_engine->map->grid_h; j++) {
            draw_line(0, (int) (game_engine->map->grid_space * j + game_engine->map->grid_y_pos),
                      wnd_w,
                      (int) (game_engine->map->grid_space * j + game_engine->map->grid_y_pos),
                      ccolor);
        }
        //niedostępne pola
        for (int w = 0; w < game_engine->map->grid_h; w++) { //dla każdego wiersza
            for (int k = 0; k < game_engine->map->grid_w; k++) { //dla każdej kolumny
                if (game_engine->map->grid[w][k] == 'x') {
                    draw_fill_rect((int) (game_engine->map->grid_space * k +
                                          game_engine->map->grid_x_pos),
                                   (int) (game_engine->map->grid_space * w +
                                          game_engine->map->grid_y_pos),
                                   (int) (game_engine->map->grid_space),
                                   (int) (game_engine->map->grid_space), ccolor);
                }
            }
        }
    }
    //ścieżki celów komputerów
    if (Config::show_paths) {
        for (unsigned int i = 0; i < game_engine->players.size(); i++) {
            if (game_engine->players[i]->controlby == P_AI) { //dla AI
                if (game_engine->players[i]->sciezka != nullptr) {
                    for (unsigned int j = 0;
                         j < game_engine->players[i]->sciezka->points.size() - 1; j++) {
                        int x1 = game_engine->players[i]->sciezka->points.at(j)->first;
                        int y1 = game_engine->players[i]->sciezka->points.at(j)->second;
                        int x2 = game_engine->players[i]->sciezka->points.at(j + 1)->first;
                        int y2 = game_engine->players[i]->sciezka->points.at(j + 1)->second;
                        //jeśli linia przebiega przez krańce mapy
                        if (abs(x1 - x2) == game_engine->map->grid_w - 1)
                            continue;
                        if (abs(y1 - y2) == game_engine->map->grid_h - 1)
                            continue;
                        draw_line(game_engine->xmap_to_x(x1), game_engine->ymap_to_y(y1),
                                  game_engine->xmap_to_x(x2),
                                  game_engine->ymap_to_y(y2),
                                  game_engine->players[i]->color);
                    }
                }
            }
        }
    }
    //animacje
    for (unsigned int i = 0; i < animations.size(); i++) {
        animations[i]->draw();
    }
    //obiekty
    for (unsigned int i = 0; i < game_engine->items.size(); i++) {
        game_engine->items[i]->draw();
    }
    //gracze - duszki i pacmany
    for (unsigned int i = 0; i < game_engine->players.size(); i++) {
        game_engine->players[i]->draw();
    }
    //pauza
    if (game_engine->pause) {
        draw_text(getLanguage()->pause, font2, rgba(255, 255, 255), wnd_w / 2, wnd_h / 2 - 22,
                  TEXT_CENTER);
    }
    //  INFO, TRYBY GRY
    draw_fill_rect(0, wnd_h - 100, 184, 100, rgba(20, 20, 50, 100));
    stringstream ss;
    if (game_engine->mode == MODE_CLASSIC) ss << getLanguage()->mode_classic;
    if (game_engine->mode == MODE_ZOMBIE) ss << getLanguage()->mode_zombie;
    draw_text(ss.str(), font1, rgba(200, 200, 255), 92, wnd_h - 95, TEXT_CENTER);
    if (game_engine->eating > 0) {
        ss_clear(ss);
        ss << getLanguage()->eating_ghosts << Timer::cycles_to_s(game_engine->eating) << " s";
        draw_text(ss.str(), font1, rgba(100, 100, 255), 92, wnd_h - 75, TEXT_CENTER);
    }
    if (game_engine->round_next > 0) {
        ss_clear(ss);
        if (game_engine->round_next > Config::next_round_time) {
            ss << getLanguage()->round_finished <<
            Timer::cycles_to_s(game_engine->round_next - Config::next_round_time);
        } else {
            ss << getLanguage()->next_round << Timer::cycles_to_s(game_engine->round_next);
        }
        ss << " s";
        draw_text(ss.str(), font1, rgba(255, 255, 100), 92, wnd_h - 55, TEXT_CENTER);
    }
    if (game_engine->mode == MODE_ZOMBIE) {
        ss_clear(ss);
        ss << getLanguage()->next_death << Timer::cycles_to_s(game_engine->game_c1) << " s";
        draw_text(ss.str(), font1, rgba(255, 100, 100), 92, wnd_h - 35, TEXT_CENTER);
    }
    //listy pacmanów
    unsigned int list_spacing = 30;
    if (42 + (game_engine->pacmans.size() - 1) * list_spacing >=
        (unsigned int) (wnd_h - 100)) {
        list_spacing = (unsigned int) ((wnd_h - 100 - 42) / (game_engine->pacmans.size() - 1));
    }
    int suma = 0;
    for (unsigned int i = 0; i < game_engine->pacmans.size(); i++) {
        suma += game_engine->pacmans[i]->score;
        ss_clear(ss);
        ss << game_engine->pacmans[i]->name << ": " << game_engine->pacmans[i]->score;
        draw_text(ss.str(), font1, game_engine->pacmans[i]->color, 30, 30 + i * list_spacing,
                  TEXT_LEFT);
        //obrazek gracza
        game_engine->pacmans[i]->draw_sprite_at(game_engine->pacmans[i]->texture, 14,
                                                38 + i * list_spacing);
    }
    ss_clear(ss);
    ss << getLanguage()->pacmans << " (" << game_engine->count_players(P_PACMAN) << "): " << suma;
    draw_text(ss.str(), font1, rgba(255, 255, 0), 90, 5, TEXT_CENTER);
    //lista duszków
    list_spacing = 30;
    if (42 + (game_engine->ghosts.size() - 1) * list_spacing >= (unsigned int) wnd_h) {
        list_spacing = (unsigned int) ((wnd_h - 42) / (game_engine->ghosts.size() - 1));
    }
    suma = 0;
    for (unsigned int i = 0; i < game_engine->ghosts.size(); i++) {
        suma += game_engine->ghosts[i]->score;
        ss_clear(ss);
        ss << game_engine->ghosts[i]->name << ": " << game_engine->ghosts[i]->score;
        draw_text(ss.str(), font1, game_engine->ghosts[i]->color, wnd_w - 30,
                  30 + i * list_spacing, TEXT_RIGHT);
        //obrazek gracza
        SDL_Texture* texture2;
        if (game_engine->eating > 0) {
            texture2 = tex("ghost_eatme");
        } else {
            texture2 = game_engine->ghosts[i]->texture;
        }
        game_engine->ghosts[i]->draw_sprite_at(texture2, wnd_w - 14, 38 + i * list_spacing);
    }
    ss_clear(ss);
    ss << getLanguage()->ghosts << " (" << game_engine->count_players(P_GHOST) << "): " << suma;
    draw_text(ss.str(), font1, rgba(255, 0, 0), wnd_w - 80, 5, TEXT_CENTER);
}

void Graphics::draw_console() {
    draw_fill_rect(50, 0, wnd_w - 100, 40 + Config::echoes_max * 18, rgba(100, 100, 100, 210));
    stringstream ss;
    ss << "> " << game_engine->cmd_in;
    if (game_engine->cycles / 10 % 2 == 0) {
        ss << "_";
    }
    draw_text(ss.str(), font1, rgba(0, 255, 0), 60, 15 + Config::echoes_max * 18);
    int i = Config::echoes_max;
    for (auto it = Log::getEchoes()->rbegin(); it != Log::getEchoes()->rend(); it++) {
        i--;
        draw_text(*it, font1, rgba(0, 200, 0), 60, 10 + i * 18);
    }
}
