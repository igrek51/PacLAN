#include "game_engine.h"
#include "../config.h"
#include "../utils.h"

void GameEngine::logic_game() {
    //wiadomości z sieci
    if (network->client) { //wiadomości z serwera
        while (network->connections.at(0)->isReady()) {
            string packet = network->connections.at(0)->popPacket();
            network_packets_split(0, packet);
        }
    } else if (network->server) { //wiadomości od klientów (i wiadomości do samego serwera)
        for (unsigned int client = 0;
             client < network->connections.size(); client++) { //dla każdego klienta
            while (network->connections.at(client)->isReady()) {
                string packet = network->connections.at(client)->popPacket();
                network_packets_split(client, packet);
            }
        }
    }
    //synchronizacja klientów przez serwer
    ss_clear(synchro);
    //obliczenia wspólne dla serwera i klienta
    if (pause)
        return;
    //odliczanie do następnej rundy
    if (round_next > 0) {
        round_next--;
        if (network->server) {
            //przygotowanie na miejsca
            if (round_next == Config::next_round_time) {
                mode_init();
            }
            if (round_next == 0) {
                stringstream synchro2;
                synchro2 << "013 " << round_next << "\r";
                network_send_to_clients(synchro2.str());
            }
        }
        return;
    }
    //tryb zjadania duszków
    if (eating > 0) {
        eating--;
        if (network->server) {
            if (eating == 0) {
                synchro << "015 " << eating << "\r";
            }
        }
    }
    //dostosowanie tempa graczy
    for (unsigned int i = 0; i < players.size(); i++) {
        if (mode == MODE_CLASSIC) {
            if (players[i]->subclass == P_PACMAN) {
                players[i]->can_move_n = Config::pacman_speed;
            } else if (players[i]->subclass == P_GHOST) {
                if (eating > 0) {
                    players[i]->can_move_n = Config::ghost_eatme_speed;
                } else {
                    players[i]->can_move_n = Config::ghost_speed;
                }
            }
        } else if (mode == MODE_ZOMBIE) {
            if (players[i]->subclass == P_PACMAN) {
                players[i]->can_move_n = Config::zombie_pacman_speed;
            } else if (players[i]->subclass == P_GHOST) {
                if (eating > 0) {
                    players[i]->can_move_n = Config::zombie_ghost_eatme_speed;
                } else {
                    players[i]->can_move_n = Config::zombie_ghost_speed;
                }
            }
        }
    }
    for (unsigned int i = 0; i < players.size(); i++) {
        //odblokowanie możliwości zmiany kierunku - zmiana kratki
        players[i]->can_move++;
        if (players[i]->can_move >= players[i]->can_move_n) {
            players[i]->can_move = 0;
        }
        //respawn
        if (players[i]->respawn > 0) {
            players[i]->respawn--;
        }
        if (players[i]->can_move == 0) {
            if (network->client) { //predykcja ruchu gracza zanim przyjdzie pakiet dla klienta
                //zaakceptowanie lub odrzucenie wciśniętych kierunków i ruchów
                if (Config::move_while_respawn || players[i]->respawn == 0) {
                    //players[i]->next_moving = players[i]->moving;
                    check_next(players[i], players[i]->moving, players[i]->direction,
                               players[i]->next_direction);
                } else {
                    players[i]->moving = 0;
                }
                //wykonanie ruchu
                if (players[i]->moving == 1) {
                    if (players[i]->direction == P_RIGHT) {
                        players[i]->xmap++;
                    } else if (players[i]->direction == P_UP) {
                        players[i]->ymap--;
                    } else if (players[i]->direction == P_LEFT) {
                        players[i]->xmap--;
                    } else if (players[i]->direction == P_DOWN) {
                        players[i]->ymap++;
                    }
                }
                //zawijanie poza mapą
                if (next_grid(players[i], -1) == '0') {
                    int przesx = 0, przesy = 0;
                    if (players[i]->xmap < 0) {
                        players[i]->xmap = map->grid_w - 1;
                        przesx = 1;
                    }
                    if (players[i]->ymap < 0) {
                        players[i]->ymap = map->grid_h - 1;
                        przesy = 1;
                    }
                    if (players[i]->xmap >= map->grid_w) {
                        players[i]->xmap = 0;
                        przesx = -1;
                    }
                    if (players[i]->ymap >= map->grid_h) {
                        players[i]->ymap = 0;
                        przesy = -1;
                    }
                    //zachowanie płynnego ruchu przy skoku
                    players[i]->xmap += przesx;
                    players[i]->ymap += przesy;
                    xymap_to_xy(players[i]);
                    players[i]->move_abs(players[i]->x, players[i]->y);
                    players[i]->xmap -= przesx;
                    players[i]->ymap -= przesy;
                    xymap_to_xy(players[i]);
                    players[i]->move();
                } else {
                    //przesunięcie na ekranie
                    xymap_to_xy(players[i]);
                    players[i]->move();
                }
            }
        }
    }

    if (network->client) {
        if (mode == MODE_ZOMBIE) {
            game_c1--;
        }
        return;
    }
    //tryb classic
    if (mode == MODE_CLASSIC) {
        if (item_count(I_LARGEDOT) == 0 && item_count(I_SMALLDOT) == 0) {
            round_next = Config::next_round_time * 2;
            synchro << "013 " << round_next << "\r";
        }
    }
    //tryb zombie
    if (mode == MODE_ZOMBIE) {
        //jeśli prawie wszystko wyzbierane - dodaj kropki
        if (item_count(I_LARGEDOT) == 0 &&
            item_count(I_SMALLDOT) <= Config::zombie_dots_threshold) {
            item_add(I_LARGEDOT, " .o");
            for (int i = 0; i < Config::zombie_dots_add; i++) {
                item_add(I_SMALLDOT, " .o");
            }
        }
        game_c1--;
        if (game_c1 == 0) {
            //zmiana ostatniego gracza w duszka
            int imin = -1;
            for (unsigned int i = 0; i < players.size(); i++) {
                if (players[i]->subclass == P_PACMAN) {
                    if (imin == -1 || players[i]->score < players[imin]->score) {
                        imin = i;
                    }
                }
            }
            if (imin != -1) {
                change_subclass(imin, P_GHOST);
                players[imin]->score = 0;
                synchro << "023 " << imin << " " << players[imin]->score << "\r";
            }
            game_c1 = Timer::s_to_cycles(Config::zombie_death_time) - 1;
            synchro << "014 " << game_c1 << " " << game_c2 << " " << game_c3 << "\r";
            //jeśli został już ostatni
            if (count_players(P_PACMAN) <= 1) {
                //koniec rundy
                round_next = Config::next_round_time * 2;
                synchro << "013 " << round_next << "\r";
            }
        }
    }
    //dla wszystkich graczy
    for (unsigned int i = 0; i < players.size(); i++) {
        //ruch dla komputerów
        if (players[i]->can_move == 0 && players[i]->controlby == P_AI) {
            if (Config::move_while_respawn || players[i]->respawn == 0) {
                players[i]->ai_control();
            } else {
                players[i]->next_moving = 0;
            }
        }
        if (players[i]->can_move == 0) {
            //zaakceptowanie lub odrzucenie wciśniętych kierunków i ruchów
            if (Config::move_while_respawn || players[i]->respawn == 0) {
                check_next(players[i], players[i]->moving, players[i]->direction,
                           players[i]->next_direction);
            } else {
                players[i]->moving = 0;
            }
            //wykonanie ruchu
            if (players[i]->moving == 1) {
                if (players[i]->direction == P_RIGHT) {
                    players[i]->xmap++;
                } else if (players[i]->direction == P_UP) {
                    players[i]->ymap--;
                } else if (players[i]->direction == P_LEFT) {
                    players[i]->xmap--;
                } else if (players[i]->direction == P_DOWN) {
                    players[i]->ymap++;
                }
            }
            //zawijanie poza mapą
            if (next_grid(players[i], -1) == '0') {
                int przesx = 0, przesy = 0;
                if (players[i]->xmap < 0) {
                    players[i]->xmap = map->grid_w - 1;
                    przesx = 1;
                }
                if (players[i]->ymap < 0) {
                    players[i]->ymap = map->grid_h - 1;
                    przesy = 1;
                }
                if (players[i]->xmap >= map->grid_w) {
                    players[i]->xmap = 0;
                    przesx = -1;
                }
                if (players[i]->ymap >= map->grid_h) {
                    players[i]->ymap = 0;
                    przesy = -1;
                }
                //zachowanie płynnego ruchu przy skoku
                players[i]->xmap += przesx;
                players[i]->ymap += przesy;
                xymap_to_xy(players[i]);
                players[i]->move_abs(players[i]->x, players[i]->y);
                players[i]->xmap -= przesx;
                players[i]->ymap -= przesy;
                xymap_to_xy(players[i]);
                players[i]->move();
                synchro << "021 " << i << " " << players[i]->xmap << " " << players[i]->ymap <<
                "\r";
            } else {
                //płynne przesunięcie na ekranie
                xymap_to_xy(players[i]);
                players[i]->move();
                if (players[i]->prev_moving != players[i]->moving ||
                    players[i]->prev_direction != players[i]->direction) {
                    synchro << "020 " << i << " " << players[i]->xmap << " " << players[i]->ymap <<
                    "\r";
                }
            }
            if (players[i]->prev_moving != players[i]->moving ||
                players[i]->prev_direction != players[i]->direction) {
                synchro << "022 " << i << " " << players[i]->moving << " " <<
                players[i]->direction << "\r";
            }
            players[i]->prev_direction = players[i]->direction;
            players[i]->prev_moving = players[i]->moving;
        }
    }
    //kolizje graczy
    for (unsigned int i = 0; i < players.size(); i++) {
        if (players[i]->respawn > 0)
            continue;
        for (unsigned int j = i + 1; j < players.size(); j++) { // i < j
            if (players[j]->respawn > 0)
                continue;
            if (is_collision(players[i], players[j])) {
                //kolizja playera z duszkiem
                if (players[i]->subclass + players[j]->subclass == 1) { //różne klasy (0 i 1)
                    unsigned int killer = j;
                    unsigned int killed = i;
                    if ((players[i]->subclass == P_GHOST && eating == 0) ||
                        (players[i]->subclass == P_PACMAN && eating > 0)) {
                        killer = i;
                        killed = j;
                    }
                    //punkty za zabicie pacmana
                    if (players[killed]->subclass == P_PACMAN) {
                        if (mode == MODE_ZOMBIE) {
                            players[killer]->score += Config::zombie_points_kill_pacman;
                        } else {
                            players[killer]->score += Config::points_kill_pacman;
                        }
                    } else if (players[killed]->subclass == P_GHOST) {
                        //punkty za zabicie duszka
                        if (mode == MODE_ZOMBIE) {
                            players[killer]->score += Config::zombie_points_kill_ghost;
                        } else {
                            players[killer]->score += Config::points_kill_ghost;
                        }
                    }
                    kill_player(killed);
                    sort_players();
                    synchro << "023 " << j << " " << players[killer]->score << "\r";
                    //nie sprawdzaj dalej tego i
                    if (killed == i) break;
                }
            }
        }
    }
    //wzięcie obiektów
    for (unsigned int i = 0; i < players.size(); i++) { //dla gracza i
        if (players[i]->subclass == P_PACMAN && players[i]->respawn == 0) { //jeśli jest pacmanem
            for (unsigned int j = 0; j < items.size(); j++) { //dla obiektu j
                if (is_collision(players[i], items[j])) {
                    int punkty;
                    if (items[j]->subclass == I_SMALLDOT) { //mała kropka
                        if (mode == MODE_ZOMBIE) {
                            punkty = Config::zombie_points_small;
                        } else {
                            punkty = 1;
                        }
                        players[i]->score += punkty;
                        sort_players();
                        synchro << "023 " << i << " " << players[i]->score << "\r";
                    } else if (items[j]->subclass == I_LARGEDOT) { //duża kropka
                        if (mode == MODE_ZOMBIE) {
                            punkty = Config::zombie_points_large;
                            eating = Config::zombie_eating_time;
                        } else {
                            punkty = 1;
                            eating = Config::eating_time; //tryb zjadania duszków
                        }
                        players[i]->score += punkty;
                        sort_players();
                        synchro << "015 " << eating << "\r";
                    }
                    synchro << "041 " << items[j]->subclass << " " << items[j]->xmap << " " <<
                    items[j]->ymap << "\r";
                    delete items[j];
                    items.erase(items.begin() + j);
                    j--;
                    continue;
                }
            }
        }
    }
    if (network->server) {
        network_send_to_clients(synchro.str());
    }
}

void GameEngine::logic_menu() {
    if (network->client) { //jeśli udało się połączyć
        //odczytaj komunikat o pomyślnym połączeniu
        while (network->connections.at(0)->isReady()) {
            string packet = network->connections.at(0)->popPacket();
            network_packets_split(0, packet);
        }
    }
}

//https://wiki.libsdl.org/SDL_Scancode
void GameEngine::keyboard_event(SDL_Keysym keysym) {
    SDL_Keycode key = keysym.sym;
    if (key == SDLK_F4 || key == SDLK_F11) {
        graphics->fullscreen_toggle();
    }
    if (key == SDLK_BACKQUOTE) {
        cmd_on = !cmd_on;
        return;
    }
    if (cmd_on) {
        cmd_input_char(key);
        return;
    }
    if (menu > 0) { //MENU
        if (menu == MENU_NAME) {
            if (key == SDLK_BACKSPACE) {
                if (menu_name.length() > 0)
                    menu_name = menu_name.substr(0, menu_name.length() - 1);
            }
            if (key == SDLK_DELETE) {
                menu_name = "";
            }
            char c = key_to_char(key);
            if (c != 0) {
                menu_name += c;
            }
        } else if (menu == MENU_JOIN) {
            if (key == SDLK_BACKSPACE) {
                if (menu_ip.length() > 0)
                    menu_ip = menu_ip.substr(0, menu_ip.length() - 1);
            }
            if (key == SDLK_DELETE) {
                menu_ip = "";
            }
            char c = key_to_char(key);
            if (c != 0) {
                menu_ip += c;
            }
        }
        if (key == SDLK_ESCAPE) {
            if (menu == MENU_EXIT) {
                app->exit();
            } else {
                menu = MENU_EXIT;
            }
        } else if (key == SDLK_UP) {
            menu--;
            if (menu <= 0) menu = MENU_MAX;
        } else if (key == SDLK_DOWN) {
            menu++;
            if (menu > MENU_MAX) menu = 1;
        } else if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
            if (menu == MENU_EXIT) {
                app->exit();
            } else if (menu == MENU_HOST) {
                menu_host();
            } else if (menu == MENU_JOIN) {
                menu_join();
            } else if (menu == MENU_SUBCLASS) {
                menu_subclass = 1 - menu_subclass;
            } else if (menu == MENU_COLOR) {
                menu_color = Graphics::rand_bcolor();
                menu_refresh_color();
            } else if (menu == MENU_LANGUAGE) {
                Language::language_selected++;
                if (Language::language_selected >= Language::languages_n)
                    Language::language_selected = 0;
                language_refresh();
            }
        } else if (key == SDLK_LEFT) {
            if (menu == MENU_SUBCLASS) {
                menu_subclass = 1 - menu_subclass;
            } else if (menu == MENU_COLOR) {
                if (keystate[SDL_SCANCODE_R]) {
                    menu_color.r = (Uint8) ((menu_color.r - 5) % 256);
                }
                if (keystate[SDL_SCANCODE_G]) {
                    menu_color.g = (Uint8) ((menu_color.g - 5) % 256);
                }
                if (keystate[SDL_SCANCODE_B]) {
                    menu_color.b = (Uint8) ((menu_color.b - 5) % 256);
                }
                menu_refresh_color();
            } else if (menu == MENU_HOST) {
                mode--;
                if (mode < 0) mode = MODE_MAX;
            } else if (menu == MENU_LANGUAGE) {
                Language::language_selected--;
                if (Language::language_selected < 0)
                    Language::language_selected = Language::languages_n - 1;
                language_refresh();
            }
        } else if (key == SDLK_RIGHT) {
            if (menu == MENU_SUBCLASS) {
                menu_subclass = 1 - menu_subclass;
            } else if (menu == MENU_COLOR) {
                if (keystate[SDL_SCANCODE_R]) {
                    menu_color.r = (Uint8) ((menu_color.r + 5) % 256);
                }
                if (keystate[SDL_SCANCODE_G]) {
                    menu_color.g = (Uint8) ((menu_color.g + 5) % 256);
                }
                if (keystate[SDL_SCANCODE_B]) {
                    menu_color.b = (Uint8) ((menu_color.b + 5) % 256);
                }
                menu_refresh_color();
            } else if (menu == MENU_HOST) {
                mode++;
                if (mode > MODE_MAX) mode = 0;
            } else if (menu == MENU_LANGUAGE) {
                Language::language_selected++;
                if (Language::language_selected >= Language::languages_n)
                    Language::language_selected = 0;
                language_refresh();
            }
        }
    } else { //GRA
        if (key == SDLK_ESCAPE) {
            clear_all();
            if (network->server) {
                network->addtask_close_server();
            } else if (network->client) {
                network->addtask_close_client();
            }
            menu = 1;
            return;
        }
        if (key == SDLK_g) {
            Config::show_grid = !Config::show_grid;
        }
        if (key == SDLK_p) {
            Config::show_paths = !Config::show_paths;
        }
        if (key == SDLK_n) {
            Config::show_names = !Config::show_names;
        }
        if (key == SDLK_f) {
            Config::show_fps = !Config::show_fps;
        }
        if (key == SDLK_r) {
            mode_init();
        }
        if (key == SDLK_1) {
            add_player(P_GHOST, "", Graphics::rand_bcolor(), P_AI, 1);
        }
        if (key == SDLK_2) {
            add_player(P_GHOST, "", Graphics::rand_bcolor(), P_AI, 2);
        }
        if (key == SDLK_3) {
            add_player(P_GHOST, "", Graphics::rand_bcolor(), P_AI, 3);
        }
        if (key == SDLK_4) {
            add_player(P_GHOST, "", Graphics::rand_bcolor(), P_AI, 4);
        }
        if (key == SDLK_5) {
            add_player(P_GHOST, "", Graphics::rand_bcolor(), P_AI, 5);
        }
        if (key == SDLK_RETURN) {
            if (!network->client) {
                pause = !pause;
                if (network->server) {
                    if (pause) {
                        synchronize_players();
                        network_send_to_clients("010");
                    } else {
                        network_send_to_clients("011");
                        synchronize_players();
                    }
                }
            }
        }
        if (network->client) {
            //wysłanie informacji do serwera o zmianie kierunku lub ruchu
            stringstream synchroc;
            if (key == SDLK_LEFT) {
                synchroc << "102 " << P_LEFT << "\r";
            }
            if (key == SDLK_RIGHT) {
                synchroc << "102 " << P_RIGHT << "\r";
            }
            if (key == SDLK_UP) {
                synchroc << "102 " << P_UP << "\r";
            }
            if (key == SDLK_DOWN) {
                synchroc << "102 " << P_DOWN << "\r";
            }
            if (key == SDLK_SPACE) {
                synchroc << "101 " << 0 << "\r";
            }
            network_send_to_server(synchroc.str());
        } else {
            //dla sterowanych klawiaturą
            for (unsigned int i = 0; i < players.size(); i++) {
                if (players[i]->controlby == P_KEYBOARD) {
                    if (key == SDLK_LEFT) {
                        players[i]->next_direction = P_LEFT;
                        players[i]->next_moving = 1;
                    }
                    if (key == SDLK_RIGHT) {
                        players[i]->next_direction = P_RIGHT;
                        players[i]->next_moving = 1;
                    }
                    if (key == SDLK_UP) {
                        players[i]->next_direction = P_UP;
                        players[i]->next_moving = 1;
                    }
                    if (key == SDLK_DOWN) {
                        players[i]->next_direction = P_DOWN;
                        players[i]->next_moving = 1;
                    }
                    if (key == SDLK_SPACE) {
                        players[i]->next_moving = 0;
                    }
                }
            }
        }
    }
}
