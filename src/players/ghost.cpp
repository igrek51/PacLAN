#include "ghost.h"
#include "../config.h"
#include "../app.h"

Ghost::Ghost(int xmap, int ymap, SDL_Color color, string name, int controlby, GameEngine* game_engine, Graphics* graphics) :
    Player(xmap,ymap,color,name,controlby, game_engine, graphics){
    texture = nullptr;
    reload_texture();
    can_move_n = Config::ghost_speed;
    subclass = P_GHOST;
}

Ghost::~Ghost(){

}

void Ghost::draw(){
    if(game_engine->eating>0){
        draw_sprite(graphics->tex("ghost_eatme"));
    }else{
        draw_sprite(texture);
    }
    if(Config::show_names){
        draw_name();
    }
}

void Ghost::animate(){
    a_cycle++;
    if(a_cycle>=7){
        a_cycle = 0;
        a_sprite++;
        if(a_sprite>=3) a_sprite = 0;
    }
    move_smooth();
}

void Ghost::clip_table(){
    if(game_engine->eating>0){
        if(game_engine->eating < Config::eating_time_critical){
            clip[0] = (game_engine->eating/2)%2;
        }else{
            clip[0] = 0;
        }
        clip[1] = 2;
        clip[2] = a_sprite;
        clip[3] = 3;
    }else{
        clip[0] = 0;
        clip[1] = 1;
        clip[2] = a_sprite;
        clip[3] = 3;
    }
}

void Ghost::reload_texture(){
    if(texture!=nullptr)
        graphics->destroy_texture(texture);
    texture =  graphics->blend_texture(graphics->tex("ghost"),color);
}

void Ghost::ai_control(){
    //AI = 0 - DUMB - tylko losowe ruchy
    //AI = 1 - PATHFINDER - losuje punkt na mapie i do niego podąża, następnie losuje kolejny
    //AI = 2 - GUARD - gdy pacman znajdzie się w obszarze (odległości), dopiero goni, pilnuje kropek
    //AI = 3 - FOLLOWER - podąża do starego położenia pacmana, dopiero gdy dojdzie wyznacza kolejny cel
    //AI = 4 - DUMB CHASER - podąża za najbliższym pacmanem + losowe ruchy
    //AI = 5 - CHASER - nieustannie podąża za najbliższym pacmanem
    next_moving = 1;
    int target_x = -1;
    int target_y = -1;
    if(ai_level==0){
        move_random();
        return;
    }
    if(ai_level==4){
        //szansa na losowe ruchy
        if(rand()% 4 ==0){
            move_random();
            return;
        }
    }
    if(ai_level==1 || ai_level==3){
        //jeśli dotarł do celu
        if(sciezka!=nullptr){
            if(sciezka->points.size()<=1){
                delete sciezka;
                sciezka = nullptr;
            }
        }
        //jeśli nie ma celu (lub dotarł do celu)
        if(sciezka==nullptr){
            if(ai_level==1){
                //wyznacz nowy cel - losowy punkt na mapie
                game_engine->random_field(target_x,target_y," .o");
            }
            if(ai_level==3){
                //wyznacz nowy cel - stare położenie losowego pacmana
                int pl_count = 0; //liczba żywych pacmanów
                for(unsigned int i=0; i<game_engine->players.size(); i++){
                    if(game_engine->players[i]->subclass==P_PACMAN && game_engine->players[i]->respawn==0){
                        pl_count++;
                    }
                }
                if(pl_count==0) //brak pacmanów
                    return;
                int pacman_index = rand()%pl_count;
                for(unsigned int i=0; i<game_engine->players.size(); i++){
                    if(game_engine->players[i]->subclass==P_PACMAN && game_engine->players[i]->respawn==0){
                        if(pacman_index>0){
                            pacman_index--;
                            continue;
                        }
                        target_x = game_engine->players[i]->xmap;
                        target_y = game_engine->players[i]->ymap;
                        break;
                    }
                }
            }
            //szukanie drogi do celu
            game_engine->pathfind->set_xy(xmap, ymap, target_x, target_y);
            sciezka = game_engine->pathfind->find_path();
        }
    }
    if(ai_level==2 || ai_level==4 || ai_level==5){
        //usunięcie starej ścieżki
        if(sciezka!=nullptr){
            delete sciezka;
            sciezka = nullptr;
        }
        //odległość do najbliższego pacmana
        int min_d = -1, min_i = -1;
        for(unsigned int i=0; i<game_engine->players.size(); i++){
            if(game_engine->players[i]->subclass==P_PACMAN && game_engine->players[i]->respawn==0){
                //odległość w metryce miejskiej
                if(min_i==-1 || game_engine->distance_m(this,game_engine->players[i])<min_d){
                    min_d = game_engine->distance_m(this,game_engine->players[i]);
                    min_i = i;
                }
            }
        }
        if(ai_level==2 && (min_d > 10 || min_i==-1)){ //gdy pacmany są w odległości większej niż LIMIT_D lub nie ma pacmanów
            //podążaj do najbliższej kropki
            min_d = -1, min_i = -1;
            for(unsigned int i=0; i<game_engine->items.size(); i++){
                if(game_engine->items[i]->subclass==I_SMALLDOT || game_engine->items[i]->subclass==I_LARGEDOT){
                    //odległość w metryce miejskiej
                    if(min_d==-1 || game_engine->distance_m(this,game_engine->items[i])<min_d){
                        min_d = game_engine->distance_m(this,game_engine->items[i]);
                        min_i = i;
                    }
                }
            }
            //jeśli nie ma już kropek
            if(min_i==-1){
                move_random();
                return;
            }
            target_x = game_engine->items[min_i]->xmap;
            target_y = game_engine->items[min_i]->ymap;
        }else if(min_i==-1){ //brak żywych pacmanów
            return;
        }else{
            //szukanie drogi do najbliższego pacmana
            target_x = game_engine->players[min_i]->xmap;
            target_y = game_engine->players[min_i]->ymap;
        }
        game_engine->pathfind->set_xy(xmap, ymap, target_x, target_y);
        sciezka = game_engine->pathfind->find_path();
    }
    if(sciezka!=nullptr){
        if(sciezka->points.size()>1){
            //podążaj ścieżką
            game_engine->follow_path(xmap, ymap, next_direction, sciezka);
            if(ai_level==2 || ai_level==3 || ai_level==4 || ai_level==5){
                if(game_engine->eating>0){ //w trybie zjadania duszków
                    //idź w innym kierunku
                    int forbidden_d = next_direction;
                    int r = rand()%3;
                    for(int i=0; i<3; i++){
                        int d = (r+i)%3;
                        next_direction = (forbidden_d+d+1)%4; //sprawdź inny kierunek
                        //sprawdź czy można tam iść
                        int temp_moving = 1, temp_direction = next_direction;
                        game_engine->check_next(this, temp_moving, temp_direction, next_direction);
                        if(temp_moving==1)
                            break;
                    }
                    if(ai_level==3){
                        delete sciezka; //wygeneruj nową ścieżkę następnym razem, gdyż jest nieaktualna
                        sciezka = nullptr;
                        return;
                    }
                }
            }
            if(ai_level==1 || ai_level==3){
                //usuń pierwszy element ze ścieżki
                delete sciezka->points.at(0);
                sciezka->points.erase(sciezka->points.begin());
            }
        }
    }
}
