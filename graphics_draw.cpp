#include "graphics.h"
#include "config.h"
#include "log.h"
#include "app.h"

void Graphics::animate(){
    if(App::game_engine->menu>0){ //MENU
        App::game_engine->menu_pacman->animate();
        App::game_engine->menu_ghost->animate();
    }else{ //MAPKA
        for(unsigned int i=0; i<App::game_engine->players.size(); i++){
            App::game_engine->players[i]->animate();
        }
        for(unsigned int i=0; i<animations.size(); i++){
            if(animations[i]->ends<=0){
                //usunięcie skończonej animacji
                delete animations[i];
                animations.erase(animations.begin()+i);
                i--;
                continue;
            }
            animations[i]->animate();
        }
    }
}

void Graphics::draw2(){
    //FPSy
    stringstream ss;
    if(Config::show_fps){
        ss<<fps<<" FPS";
        draw_text(ss.str(), font1, rgba(0,255,0), 0, screen_h-15, TEXT_LEFT);
    }
    if(App::game_engine->menu>0){ //    MENU
        //podpis
        draw_text("Created by Igrek", font2, rgba(30,30,30), screen_w, screen_h-15, TEXT_RIGHT);
        ss_clear(ss);
        ss<<"P a c L A N  v"<<Config::version;
        draw_text(ss.str(), font2, rgba(255,255,0), screen_w/2, 45, TEXT_CENTER);
        //obrazek gracza
        Player *menu_player;
        if(App::game_engine->menu_subclass==P_PACMAN){
            menu_player = App::game_engine->menu_pacman;
        }else{
            menu_player = App::game_engine->menu_ghost;
        }
        menu_player->draw_sprite_at(menu_player->texture, screen_w/2, 100);
        ss_clear(ss);
        ss<<"Nazwa gracza: "<<App::game_engine->menu_name;
        if(App::game_engine->menu==MENU_NAME && App::game_engine->cycles/10%2==0){
            ss<<"_";
        }else{
            ss<<"  ";
        }
        draw_text(ss.str(), font1, rgba(255,255,255), screen_w/2, 140, TEXT_CENTER);
        ss_clear(ss);
        ss<<"Klasa gracza: "<<(App::game_engine->menu_subclass==P_PACMAN?"Pacman":"Ghost");
        draw_text(ss.str(), font1, rgba(255,255,255), screen_w/2, 170, TEXT_CENTER);
        ss_clear(ss);
        ss<<"Kolor gracza: ("<<(int)App::game_engine->menu_color.r<<", "<<(int)App::game_engine->menu_color.g<<", "<<(int)App::game_engine->menu_color.b<<")";
        draw_text(ss.str(), font1, App::game_engine->menu_color, screen_w/2, 200, TEXT_CENTER);
        ss_clear(ss);
        ss<<"Załóż nową grę: Tryb ";
        if(App::game_engine->mode==MODE_CLASSIC) ss<<"Classic";
        if(App::game_engine->mode==MODE_ZOMBIE) ss<<"Zombie";
        draw_text(ss.str(), font1, rgba(255,255,255), screen_w/2, 230, TEXT_CENTER);
        ss_clear(ss);
        ss<<"Dołącz do: "<<App::game_engine->menu_ip;
        if(App::game_engine->menu==MENU_JOIN && App::game_engine->cycles/10%2==0){
            ss<<"_";
        }else{
            ss<<"  ";
        }
        draw_text(ss.str(), font1, rgba(255,255,255), screen_w/2, 260, TEXT_CENTER);
        draw_text("Wyjście", font1, rgba(255,255,255), screen_w/2, 290, TEXT_CENTER);
        //kursor
        draw_fill_rect(260, 135+(App::game_engine->menu-1)*30, 280, 26, rgba(150,150,150,40));
        App::game_engine->menu_pacman->draw_sprite_at(App::game_engine->menu_pacman->texture, 260, 148+(App::game_engine->menu-1)*30);
        App::game_engine->menu_ghost->draw_sprite_at(App::game_engine->menu_ghost->texture, 540, 148+(App::game_engine->menu-1)*30);
        draw_text("Sterowanie:", font1, rgba(120,120,120), screen_w/2, 340, TEXT_CENTER);
        draw_text("Strzałki - ruch gracza", font1, rgba(120,120,120), screen_w/2, 360, TEXT_CENTER);
        draw_text("Spacja - zatrzymanie gracza", font1, rgba(120,120,120), screen_w/2, 375, TEXT_CENTER);
        draw_text("Enter - pauza", font1, rgba(120,120,120), screen_w/2, 390, TEXT_CENTER);
        draw_text("` - wiersz poleceń", font1, rgba(120,120,120), screen_w/2, 405, TEXT_CENTER);
        draw_text("F11 - pełny ekran", font1, rgba(120,120,120), screen_w/2, 420, TEXT_CENTER);
        draw_text("1,2,3,4,5 - szybkie dodanie duszków AI", font1, rgba(120,120,120), screen_w/2, 435, TEXT_CENTER);
    }else{  //  MAPKA
        //tło - mapka
        draw_texture_center(App::game_engine->map->background,screen_w/2,screen_h/2);
        //siatka pól mapy
        if(Config::show_grid){
            ccolor = rgba(0,0,255,50);
            for(int i=0; i<=App::game_engine->map->grid_w; i++){
                draw_line(App::game_engine->map->grid_space*i+App::game_engine->map->grid_x_pos, 0, App::game_engine->map->grid_space*i+App::game_engine->map->grid_x_pos, screen_h, ccolor);
            }
            for(int j=0; j<=App::game_engine->map->grid_h; j++){
                draw_line(0, App::game_engine->map->grid_space*j+App::game_engine->map->grid_y_pos, screen_w, App::game_engine->map->grid_space*j+App::game_engine->map->grid_y_pos, ccolor);
            }
            //niedostępne pola
            for(int w=0; w<App::game_engine->map->grid_h; w++){ //dla każdego wiersza
                for(int k=0; k<App::game_engine->map->grid_w; k++){ //dla każdej kolumny
                    if(App::game_engine->map->grid[w][k]=='x'){
                        draw_fill_rect(App::game_engine->map->grid_space*k+App::game_engine->map->grid_x_pos, App::game_engine->map->grid_space*w+App::game_engine->map->grid_y_pos, App::game_engine->map->grid_space, App::game_engine->map->grid_space, ccolor);
                    }
                }
            }
        }
        //ścieżki celów komputerów
        if(Config::show_paths){
            for(unsigned int i=0; i<App::game_engine->players.size(); i++){
                if(App::game_engine->players[i]->controlby==P_AI){ //dla AI
                    if(App::game_engine->players[i]->sciezka!=NULL){
                        for(unsigned int j=0; j<App::game_engine->players[i]->sciezka->points.size()-1; j++){
                            int x1 = App::game_engine->players[i]->sciezka->points.at(j)[0];
                            int y1 = App::game_engine->players[i]->sciezka->points.at(j)[1];
                            int x2 = App::game_engine->players[i]->sciezka->points.at(j+1)[0];
                            int y2 = App::game_engine->players[i]->sciezka->points.at(j+1)[1];
                            //jeśli linia przebiega przez krańce mapy
                            if(abs(x1-x2)==App::game_engine->map->grid_w-1)
                                continue;
                            if(abs(y1-y2)==App::game_engine->map->grid_h-1)
                                continue;
                            draw_line(App::game_engine->xmap_to_x(x1), App::game_engine->ymap_to_y(y1), App::game_engine->xmap_to_x(x2),
                                App::game_engine->ymap_to_y(y2), App::game_engine->players[i]->color);
                        }
                    }
                }
            }
        }

        //animacje
        for(unsigned int i=0; i<animations.size(); i++){
            animations[i]->draw();
        }
        //obiekty
        for(unsigned int i=0; i<App::game_engine->items.size(); i++){
            App::game_engine->items[i]->draw();
        }
        //gracze - duszki i pacmany
        for(unsigned int i=0; i<App::game_engine->players.size(); i++){
            App::game_engine->players[i]->draw();
        }
        //pauza
        if(App::game_engine->pause){
            draw_text("PAUZA", font2, rgba(255,255,255), screen_w/2, screen_h/2-22, TEXT_CENTER);
        }

        //  INFO, TRYBY GRY
        draw_fill_rect(0, screen_h-100, 184, 100, rgba(20,20,50,100));
        ss_clear(ss);
        ss<<"Tryb gry: ";
        if(App::game_engine->mode==MODE_CLASSIC) ss<<"Classic";
        if(App::game_engine->mode==MODE_ZOMBIE) ss<<"Zombie";
        draw_text(ss.str(), font1, rgba(200,200,255), 92, screen_h-95, TEXT_CENTER);
        if(App::game_engine->eating>0){
            ss_clear(ss);
            ss<<"Zjadanie duszków: "<<Timer::cycles_to_s(App::game_engine->eating)<<" s";
            draw_text(ss.str(), font1, rgba(100,100,255), 92, screen_h-75, TEXT_CENTER);
        }
        if(App::game_engine->round_next>0){
            ss_clear(ss);
            if(App::game_engine->round_next > Config::next_round_time){
                ss<<"Koniec rundy: "<<Timer::cycles_to_s(App::game_engine->round_next - Config::next_round_time);
            }else{
                ss<<"Następna runda za: "<<Timer::cycles_to_s(App::game_engine->round_next);
            }
            ss<<" s";
            draw_text(ss.str(), font1, rgba(255,255,100), 92, screen_h-55, TEXT_CENTER);
        }
        if(App::game_engine->mode==MODE_ZOMBIE){
            ss_clear(ss);
            ss<<"Kolejna śmierć za: "<<Timer::cycles_to_s(App::game_engine->game_c1)<<" s";
            draw_text(ss.str(), font1, rgba(255,100,100), 92, screen_h-35, TEXT_CENTER);
        }


        //listy pacmanów
        unsigned int list_spacing = 30;
        if(42 + (App::game_engine->pacmans.size()-1)*list_spacing >= (unsigned int)(screen_h-100)){
            list_spacing = (screen_h-100-42)/(App::game_engine->pacmans.size()-1);
        }
        int suma = 0;
        for(unsigned int i=0; i<App::game_engine->pacmans.size(); i++){
            suma+=App::game_engine->pacmans[i]->score;
            ss_clear(ss);
            ss<<App::game_engine->pacmans[i]->name<<": "<<App::game_engine->pacmans[i]->score;
            draw_text(ss.str(), font1, App::game_engine->pacmans[i]->color, 30, 30 + i*list_spacing, TEXT_LEFT);
            //obrazek gracza
            App::game_engine->pacmans[i]->draw_sprite_at(App::game_engine->pacmans[i]->texture, 14, 38 + i*list_spacing);
        }
        ss_clear(ss);
        ss<<"Pacmany ("<<App::game_engine->count_players(P_PACMAN)<<"): "<<suma;
        draw_text(ss.str(), font1, rgba(255,255,0), 90, 5, TEXT_CENTER);
        //lista duszków
        list_spacing = 30;
        if(42 + (App::game_engine->ghosts.size()-1)*list_spacing >= (unsigned int)screen_h){
            list_spacing = (screen_h-42)/(App::game_engine->ghosts.size()-1);
        }
        suma = 0;
        for(unsigned int i=0; i<App::game_engine->ghosts.size(); i++){
            suma+=App::game_engine->ghosts[i]->score;
            ss_clear(ss);
            ss<<App::game_engine->ghosts[i]->name<<": "<<App::game_engine->ghosts[i]->score;
            draw_text(ss.str(), font1, App::game_engine->ghosts[i]->color, screen_w-30, 30 + i*list_spacing, TEXT_RIGHT);
            //obrazek gracza
            SDL_Texture *texture2;
            if(App::game_engine->eating>0){
                texture2 = App::graphics->tex("ghost_eatme");
            }else{
                texture2 = App::game_engine->ghosts[i]->texture;
            }
            App::game_engine->ghosts[i]->draw_sprite_at(texture2, screen_w-14, 38 + i*list_spacing);
        }
        ss_clear(ss);
        ss<<"Duszki ("<<App::game_engine->count_players(P_GHOST)<<"): "<<suma;
        draw_text(ss.str(), font1, rgba(255,0,0), screen_w-80, 5, TEXT_CENTER);
    }
    //wiersz poleceń i konsola
    if(App::game_engine->cmd_on){
        draw_fill_rect(50, 0, screen_w-100, 40+Config::cmd_outputs*18, rgba(100,100,100,210));
        ss_clear(ss);
        ss<<"> "<<App::game_engine->cmd_in;
        if(App::game_engine->cycles/10%2==0){
            ss<<"_";
        }
        draw_text(ss.str(), font1, rgba(0,255,0), 60,15+Config::cmd_outputs*18);
        for(int i=0; i<Config::cmd_outputs; i++){
            draw_text(App::game_engine->cmd_out[i], font1, rgba(0,200,0), 60, 10+i*18);
        }
    }
}
