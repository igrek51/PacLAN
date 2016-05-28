#include "game_engine.h"
#include "app.h"

char GameEngine::next_grid(Player *player, int direction){
    int xmap_next = player->xmap;
    if(direction==P_RIGHT)
        xmap_next++;
    if(direction==P_LEFT)
        xmap_next--;
    int ymap_next = player->ymap;
    if(direction==P_UP)
        ymap_next--;
    if(direction==P_DOWN)
        ymap_next++;
    return map->get_grid(xmap_next,ymap_next);
}

int GameEngine::xmap_to_x(int xmap){
    return map->grid_space*xmap + map->grid_x_pos + map->grid_space/2;
}

int GameEngine::ymap_to_y(int ymap){
    return map->grid_space*ymap + map->grid_y_pos + map->grid_space/2;
}

void GameEngine::xymap_to_xy(Player *player){
    player->x = xmap_to_x(player->xmap);
    player->y = ymap_to_y(player->ymap);
}

bool GameEngine::is_collision(Player *player1, Player *player2){
    //są na tym samym polu
    if(player1->xmap==player2->xmap && player1->ymap==player2->ymap){
        return true;
    }
    //jeśli jeden z nich się nie rusza
    if(player1->moving==0 || player2->moving==0)
        return false;
    //różnica 1 kratki, ten sam kierunek, przeciwny zwrot
    //1 - po lewej, 2 - po prawej
    if(player1->xmap-1==player2->xmap && player1->ymap==player2->ymap){
        if(player1->direction==0 && player2->direction==2)
            return true;
    }
    //1 - po prawej, 2 - po lewej
    if(player1->xmap+1==player2->xmap && player1->ymap==player2->ymap){
        if(player1->direction==2 && player2->direction==0)
            return true;
    }
    //1 - na górze, 2 - na dole
    if(player1->ymap-1==player2->ymap && player1->xmap==player2->xmap){
        if(player1->direction==3 && player2->direction==1)
            return true;
    }
    //1 - na dole, 2 - na górze
    if(player1->ymap+1==player2->ymap && player1->xmap==player2->xmap){
        if(player1->direction==1 && player2->direction==3)
            return true;
    }
    return false;
}

bool GameEngine::is_collision(Player *player1, Item *item2){
    if(player1->xmap==item2->xmap && player1->ymap==item2->ymap){ //gdy są na tym samym polu
        return true;
    }
    return false;
}

void GameEngine::pathfind_init(){
    pathfind = new Pathfind();
    pathfind->map_w = map->grid_w;
    pathfind->map_h = map->grid_h;
    pathfind->map = new char* [pathfind->map_h];
    for(int w=0; w<pathfind->map_h; w++){
        pathfind->map[w] = new char [pathfind->map_w];
        for(int k=0; k<pathfind->map_w; k++){
            if(map->grid[w][k]=='x'){ //nie można przejść
                pathfind->map[w][k] = 0;
            }else{
                pathfind->map[w][k] = 1; //można przejść
            }
        }
    }
}

void GameEngine::check_next(Player *player1, int &moving, int &direction, int next_direction){
    if(player1->next_moving==0){
        moving = 0;
    }else if(next_grid(player1,next_direction)!='x'){
        if(!(next_grid(player1,next_direction)=='-' && (player1->subclass==P_PACMAN || eating>=1))){
            direction = next_direction;
            moving = 1;
        }
    }
    //zatrzymanie się w przypadku napotkania przeszkody
    if(moving==1){
        if(next_grid(player1,direction)=='x' || (next_grid(player1,direction)=='-'&&player1->subclass==P_PACMAN)){ //pola dostępne tylko dla duszków
            moving = 0;
        }
    }
}

bool GameEngine::is_field_correct(int x, int y, string pattern){
    if(pattern.length()==0)
        return false;
    for(unsigned int i=0; i<pattern.length(); i++){
        if(map->get_grid(x,y)==pattern[i])
            return true;
    }
    return false;
}

bool GameEngine::is_field_empty(int x, int y){
    for(unsigned int i=0; i<items.size(); i++){
        if(items[i]->xmap==x && items[i]->ymap==y)
            return false;
    }
    return true;
}

void GameEngine::random_field(int &x, int &y, string pattern){
    //zakładając że istnieje chociaż jedno takie pole
    do{
        x = rand()%map->grid_w;
        y = rand()%map->grid_h;
    }while(!is_field_correct(x,y,pattern));
}

void GameEngine::follow_path(int xmap, int ymap, int &direction, Path *sciezka){
    if(sciezka==NULL)
        return;
    if(sciezka->points.size()<=1)
        return;
    if(sciezka->points.at(1)[0] == xmap+1){
        direction = P_RIGHT;
    }else if(sciezka->points.at(1)[0]+1 == xmap){
        direction = P_LEFT;
    }else if(sciezka->points.at(1)[1] == ymap+1){
        direction = P_DOWN;
    }else if(sciezka->points.at(1)[1]+1 == ymap){
        direction = P_UP;
    }
    //jeśli ma ruszyć się poza mapę, zachowa swój kierunek (różnica będzie większa od 1), więc nie trzeba nic zmieniać
}


int GameEngine::distance_m(Player *player1, Player *player2){
    int dxmin = abs(player1->xmap-player2->xmap);
    if(App::game_engine->map->grid_w - dxmin < dxmin) //jeśli odległość jest większa od połowy mapki
        dxmin = App::game_engine->map->grid_w - dxmin;
    return dxmin + abs(player1->ymap-player2->ymap);
}

int GameEngine::distance_m(Player *player1, Item *item2){
    int dxmin = abs(player1->xmap-item2->xmap);
    if(App::game_engine->map->grid_w - dxmin < dxmin) //jeśli odległość jest większa od połowy mapki
        dxmin = App::game_engine->map->grid_w - dxmin;
    return dxmin + abs(player1->ymap-item2->ymap);
    //return abs(player1->xmap-item2->xmap) + abs(player1->ymap-item2->ymap);
}
