#include "pacman.h"
#include "../config.h"
#include "../app.h"

Pacman::Pacman(int xmap, int ymap, SDL_Color color, string name, int controlby, GameEngine* game_engine) :
    Player(xmap,ymap,color,name,controlby, game_engine){
    texture = nullptr;
    reload_texture();
    can_move_n = Config::pacman_speed;
    subclass = P_PACMAN;
    esc_histereza = 0;
}

Pacman::~Pacman(){

}

void Pacman::draw(){
    draw_sprite(texture);
    if(Config::show_names){
        draw_name();
    }
}

void Pacman::animate(){
    if(moving>0){
        a_cycle++;
        if(a_cycle>=7){
            a_cycle = 0;
            a_sprite++;
            if(a_sprite>=4) a_sprite = -2;
        }
    }
    move_smooth();
}

void Pacman::clip_table(){
    int sprite2 = a_sprite>=0 ? a_sprite : -a_sprite;
    clip[0] = direction;
    clip[1] = 4;
    clip[2] = sprite2;
    clip[3] = 4;
}

void Pacman::reload_texture(){
    if(texture!=nullptr)
        App::graphics->destroy_texture(texture);
    texture =  App::graphics->blend_texture(App::graphics->tex("pacman"),color);
}

void Pacman::ai_control(){
    //AI = 0 - DUMB - tylko losowe ruchy
    //AI >= 1 - INTELIGENTY PACMAN
    next_moving = 1;
    int target_x;
    int target_y;
    bool escape = false; //tryb uciekania
    if(ai_level==0){
        move_random();
        return;
    }
    //usunięcie starej ścieżki
    if(sciezka!=NULL){
        delete sciezka;
        sciezka = nullptr;
    }
    //odległość do najbliższego duszka
    int min_d = -1, min_i = -1;
    for(unsigned int i=0; i<App::game_engine->players.size(); i++){
        if(App::game_engine->players[i]->subclass==P_GHOST && App::game_engine->players[i]->respawn==0){
            //odległość w metryce miejskiej
            if(min_d==-1 || App::game_engine->distance_m(this,App::game_engine->players[i])<min_d){
                min_d = App::game_engine->distance_m(this,App::game_engine->players[i]);
                min_i = i;
            }
        }
    }
    if(min_d >= Config::ai_pacman_stop_escape){
        esc_histereza = 0;
    }
    //zjadanie duszków
    if(min_i!=-1 && App::game_engine->eating>=Config::eating_time_critical){
        target_x = App::game_engine->players[min_i]->xmap;
        target_y = App::game_engine->players[min_i]->ymap;
    }else if(min_i!=-1 && (min_d <= Config::ai_pacman_start_escape || esc_histereza==1)){ //gdy duszki są w odległości <= LIMIT_D
        //uciekaj od duszka
        escape = true;
        esc_histereza = 1;
        target_x = App::game_engine->players[min_i]->xmap;
        target_y = App::game_engine->players[min_i]->ymap;
    }else{
        //odległość do najbliższej kropki
        int min_d_kropka = -1, min_i_kropka = -1;
        for(unsigned int i=0; i<App::game_engine->items.size(); i++){
            if(App::game_engine->items[i]->subclass==I_SMALLDOT || App::game_engine->items[i]->subclass==I_LARGEDOT){
                //odległość w metryce miejskiej
                int current_d = App::game_engine->distance_m(this,App::game_engine->items[i]);
                if(min_i_kropka==-1 || current_d<min_d_kropka){
                    min_d_kropka = current_d;
                    min_i_kropka = i;
                }
            }
        }
        //jeśli nie ma już kropek
        if(min_i_kropka==-1){
            move_random();
            return;
        }
        target_x = App::game_engine->items[min_i_kropka]->xmap;
        target_y = App::game_engine->items[min_i_kropka]->ymap;
    }
    //szukanie drogi
    App::game_engine->pathfind->set_xy(xmap, ymap, target_x, target_y);
    sciezka = App::game_engine->pathfind->find_path();
    if(sciezka!=nullptr){
        if(sciezka->points.size()>1){
            //podążaj ścieżką
            App::game_engine->follow_path(xmap, ymap, next_direction, sciezka);
            if(escape){ //jeśli ucieka od celu
                int wrong_d = next_direction;
                //sprawdzenie 3 pozostałych kierunków - szukanie kierunku w ktorym odleglosc bedzie najwieksza
                int max_d = -1, max_i = -1;
                int suma;
                for(int d=0; d<3; d++){
                    next_direction = (wrong_d+d+1)%4; //sprawdź inny kierunek
                    //sprawdź czy można tam iść
                    int temp_moving = 1, temp_direction = next_direction;
                    App::game_engine->check_next(this, temp_moving, temp_direction, next_direction);
                    if(temp_moving==0)
                        continue;
                    suma = 0;
                    for(unsigned int i=0; i<App::game_engine->players.size(); i++){
                        if(App::game_engine->players[i]->subclass==P_GHOST && App::game_engine->players[i]->respawn==0){
                            int xmap_next = this->xmap;
                            if(next_direction==P_RIGHT)
                                xmap_next++;
                            if(next_direction==P_LEFT)
                                xmap_next--;
                            int ymap_next = this->ymap;
                            if(next_direction==P_UP)
                                ymap_next--;
                            if(next_direction==P_DOWN)
                                ymap_next++;
                            //przybliżenie odległości zmodyfikowaną metryką miejską
                            int dist = abs(xmap_next - App::game_engine->players[i]->xmap);
                            if(App::game_engine->map->grid_w - dist < dist) //jeśli odległość jest większa od połowy mapki
                                dist = App::game_engine->map->grid_w - dist;
                            dist = dist + abs(ymap_next - App::game_engine->players[i]->ymap);
                            if(dist >= Config::ai_pacman_stop_escape ){
                                suma += dist;
                            }
                        }
                    }
                    if(max_i==-1 || suma>max_d){
                        max_d = suma;
                        max_i = d;
                    }
                }
                if(max_i==-1)
                    return;
                next_direction = (wrong_d+max_i+1)%4;
                /*
                int r = rand()%3;
                for(int i=0; i<3; i++){
                    int d = (i+r)%3;
                    next_direction = (wrong_d+d+1)%4; //sprawdź inny kierunek
                    //sprawdź czy można tam iść
                    int temp_moving = 1, temp_direction = next_direction;
                    App::game_engine->check_next(this, temp_moving, temp_direction, next_direction);
                    if(temp_moving==1)
                        break;
                }
                */
            }
        }
    }
}
