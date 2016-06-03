#include "game_engine.h"
#include "../config.h"
#include "../log/log.h"
#include "../app.h"
#include "../graphics/animations/death_animation.h"
#include "../utils.h"

GameEngine::GameEngine(Graphics* graphics){
    this->graphics = graphics;
    Log::info("Inicjalizacja logiki gry...");
    keystate = SDL_GetKeyboardState(NULL);
    cycles = 0;
    menu_pacman = nullptr;
    menu_ghost = nullptr;
    //fullscreen
    if(Config::fullscreen_start)
        graphics->fullscreen_toggle();
    //mapka
    map = new Map(graphics->tex("map1"),Config::mapDefault);
    if(App::exit)
        return;
    pathfind_init();
    //wiersz poleceń
    cmd_on = false;
    //inicjalizacja ustawień językowych
    language_refresh();
    //inicjalizacja menu
    menu = 1;
    stringstream ss;
    ss<<"Player_"<<rand()%1000;
    menu_name = ss.str(); //wybrane imię gracza
    menu_color = Graphics::rand_bcolor();
    menu_subclass = P_PACMAN;
    menu_ip = "localhost";
    menu_pacman = new Pacman(0,0,menu_color,"menu_pacman",P_KEYBOARD, this, graphics);
    menu_pacman->moving = 1;
    menu_ghost = new Ghost(0,0,menu_color,"menu_ghost",P_KEYBOARD, this, graphics);
    menu_pacman->moving = 1;
    //inicjalizacja zmiennych gry
    mode = MODE_CLASSIC;
    eating = 0;
    game_c1 = 0;
    game_c2 = 0;
    game_c3 = 0;
    round_next = 0;
    pause = false;
    update_players_lists();
    Log::info("Silnik gry zainicalizowany.");
    Log::info("Wpisz \"help\", aby uzyskać pomoc.");
}

GameEngine::~GameEngine(){
    Log::debug("Usuwanie graczy...");
    for(unsigned int i=0; i<players.size(); i++)
        delete players[i];
    players.clear();
    pacmans.clear();
    ghosts.clear();
    delete menu_pacman;
    delete menu_ghost;
    Log::debug("Usuwanie obiektów...");
    for(unsigned int i=0; i<items.size(); i++)
        delete items[i];
    items.clear();
    Log::debug("Usuwanie mapy...");
    if(map!=nullptr)
        delete map;
    delete pathfind;
}

void GameEngine::setNetwork(Network* network){
    this->network = network;
}

void GameEngine::logic(volatile int &logic_cycles){
    if(logic_cycles>Config::logic_cycles_critical){
        Log::criticalError(
                "Przekroczono liczbę niewykonanych cykli logicznych. Komputer nie nadąża z obliczeniami.");
        App::exit = true;
        logic_cycles = -1;
        return;
    }
    cycles++;
    while(SDL_PollEvent(&event)&&!App::exit){ //pętla komunikatów SDL
        if(event.type==SDL_QUIT){
            App::exit = true;
        }
        if(event.type==SDL_KEYDOWN){
            keyboard_event(event.key.keysym);
        }
        if(event.type==SDL_WINDOWEVENT){
            if(event.window.event==SDL_WINDOWEVENT_MOVED){
                Log::debug("Zmiana położenia okna (zatrzymana aplikacja) - reset licznika logicznego");
                logic_cycles = 1;
            }
            if(event.window.event==SDL_WINDOWEVENT_RESTORED){
                Log::debug("Okno przywrócone.");
                graphics->reload_textures();
            }
        }

    }
    if(menu>0){
        logic_menu();
    }else{
        logic_game();
    }
    logic_cycles--;
}


int GameEngine::count_players(int subclass){
    int n = 0;
    for(unsigned int i=0; i<players.size(); i++){
        if(players[i]->subclass == subclass)
            n++;
    }
    return n;
}

void GameEngine::insert_items(){
    stringstream synchro2;
    for(int w=0; w<map->grid_h; w++){
        for(int k=0; k<map->grid_w; k++){
            if(map->grid[w][k]=='.'){ //mała kropka
                items.push_back(new Item(k,w,I_SMALLDOT, this, graphics));
                synchro2<<"040 "<<I_SMALLDOT<<" "<<k<<" "<<w<<"\r";
            }else if(map->grid[w][k]=='o'){ //duża kropka
                items.push_back(new Item(k,w,I_LARGEDOT, this, graphics));
                synchro2<<"040 "<<I_LARGEDOT<<" "<<k<<" "<<w<<"\r";
            }
        }
    }
    if(network->server){
        network_send_to_clients(synchro2.str());
    }
}

int GameEngine::item_count(int subclass){
    int n = 0;
    for(unsigned int i=0; i<items.size(); i++){
        if(items[i]->subclass == subclass)
            n++;
    }
    return n;
}

void GameEngine::item_add(int subclass, string pattern){
    int x, y;
    //zakładając że istnieje chociaż jedno takie pole
    do{
        x = rand()%map->grid_w;
        y = rand()%map->grid_h;
    }while(!is_field_correct(x,y,pattern) || !is_field_empty(x,y));
    items.push_back(new Item(x,y,subclass, this, graphics));
    synchro<<"040 "<<subclass<<" "<<x<<" "<<y<<"\r";
}


void GameEngine::update_players_lists(){
    //wyczyszczenie starych list (usunięcie samych wskaźników)
    pacmans.clear();
    ghosts.clear();
    for(unsigned int i=0; i<players.size(); i++){
        if(players[i]->subclass==P_PACMAN){
            pacmans.push_back((Pacman*)players[i]);
        }else if(players[i]->subclass==P_GHOST){
            ghosts.push_back((Ghost*)players[i]);
        }
    }
    sort_players();
}

void GameEngine::sort_players(){
    sort_players((vector<Player *>*)(&pacmans));
    sort_players((vector<Player *>*)(&ghosts));
}

void GameEngine::sort_players(vector<Player *> *lista){
    bool change;
    do{
        change = false;
        for(unsigned int i=0; i+1<lista->size(); i++){
            if(lista->at(i)->score < lista->at(i+1)->score){
                Player *buf = lista->at(i);
                lista->at(i) = lista->at(i+1);
                lista->at(i+1) = buf;
                change = true;
            }
        }
    }while(change);
}

Player* GameEngine::change_subclass(int index, int subclass){
    if(index<0 || index>=(int)players.size()) return nullptr;
    if(subclass==-1)
        subclass = 1-players[index]->subclass;
    Player *nowy;
    if(subclass==P_GHOST){
        nowy = new Ghost(players[index]->xmap,players[index]->ymap,players[index]->color,players[index]->name,players[index]->controlby, this, graphics);
    }else if(subclass==P_PACMAN){
        nowy = new Pacman(players[index]->xmap,players[index]->ymap,players[index]->color,players[index]->name,players[index]->controlby, this, graphics);
    }
    nowy->ai_level = players[index]->ai_level;
    nowy->lan_id = players[index]->lan_id;
    nowy->score = players[index]->score;
    delete players[index];
    players[index] = nowy;
    update_players_lists();
    if(network->server){
        stringstream synchro2;
        synchro2<<"025 "<<index<<" "<<subclass<<"\r";
        network_send_to_clients(synchro2.str());
    }
    return nowy;
}

void GameEngine::synchronize_players(int to){
    if(!network->server) return;
    stringstream synchro2;
    for(unsigned int i=0; i<players.size(); i++){
        synchro2<<"021 "<<i<<" "<<players[i]->xmap<<" "<<players[i]->ymap<<"\r"; //przemieszczenie bezwzględne
        synchro2<<"022 "<<i<<" "<<players[i]->moving<<" "<<players[i]->direction<<"\r";
    }
    network_send_to_clients(synchro2.str(),to);
}

void GameEngine::menu_refresh_color(){
    menu_pacman->color = menu_color;
    menu_pacman->reload_texture();
    menu_ghost->color = menu_color;
    menu_ghost->reload_texture();
}

void GameEngine::menu_host(){
    menu = 0;
    clear_all();
    mode_init();
    pause = false;
    network->addtask_open_server();
    add_player(menu_subclass, menu_name, menu_color, P_KEYBOARD);
}

void GameEngine::menu_join(){
    clear_all();
    //próba połączenia - nie daje pewności powodzenia
    network->addtask_connect_client(menu_ip);
}

Player* GameEngine::add_player(int subclass, string name, SDL_Color color, int controlby, int ai_level){
    if(controlby==P_AI){
        if(ai_level<0 || ai_level>5)
            ai_level = rand()%6;
    }
    if(name.length()==0){
        stringstream ss;
        if(controlby==P_AI)
            ss<<"AI"<<ai_level<<":";
        if(subclass==P_GHOST){
            ss<<"Ghost";
        }else if(subclass==P_PACMAN){
            ss<<"Pacman";
        }
        ss<<"_"<<rand()%1000;
        name = ss.str();
    }
    int xmap, ymap;
    if(subclass==P_GHOST){
        random_field(xmap,ymap,"-");
    }else if(subclass==P_PACMAN){
        random_field(xmap,ymap," .o");
    }
    Player *nowy;
    if(subclass==P_GHOST){
        nowy = new Ghost(xmap,ymap,color,name,controlby, this, graphics);
    }else if(subclass==P_PACMAN){
        nowy = new Pacman(xmap,ymap,color,name,controlby, this, graphics);
    }
    if(controlby==P_AI){
        nowy->ai_level = ai_level;
    }
    players.push_back(nowy);
    update_players_lists();
    Log::info("Dodano gracza: "+name);
    //wysłanie informacji o dodaniu gracza innym klientom
    if(network->server){
        stringstream synchro2;
        synchro2<<"030 "<<subclass<<" "<<(int)color.r<<" "<<(int)color.g<<" "<<(int)color.b<<" "<<name<<"\r";
        synchro2<<"021 "<<players.size()-1<<" "<<xmap<<" "<<ymap<<"\r";//bezwzględne umieszczenie na mapie
        synchro2<<"022 "<<players.size()-1<<" "<<nowy->moving<<" "<<nowy->direction<<"\r";
        network_send_to_clients(synchro2.str());
    }
    return nowy;
}

void GameEngine::kick_player(int index){
    if(index<0 || index>=(int)players.size()) return;
    delete players[index];
    players.erase(players.begin()+index);
    update_players_lists();
    //wysłanie informacji o usunięciu gracza innym klientom
    if(network->server){
        stringstream synchro2;
        synchro2<<"031 "<<index<<"\r";
        network_send_to_clients(synchro2.str());
        //rozłączenie klienta
    }
    stringstream ss;
    ss<<"Wyrzucono gracza nr: "<<index;
    Log::info(ss.str());
}

void GameEngine::kill_player(int index){
    if(index>=(int)players.size()) return;
    //tryb zombie
    if(mode==MODE_ZOMBIE){
        if(players[index]->subclass==P_PACMAN){
            change_subclass(index,P_GHOST);
            players[index]->score = 0;
            synchro<<"023 "<<index<<" "<<players[index]->score<<"\r";
            game_c1 = Timer::s_to_cycles(Config::zombie_death_time) - 1;
            synchro<<"014 "<<game_c1<<" "<<game_c2<<" "<<game_c3<<"\r";
            //jeśli został już ostatni
            if(count_players(P_PACMAN)<=1){
                //koniec rundy
                round_next = Config::next_round_time * 2;
                synchro<<"013 "<<round_next<<"\r";
            }
            return;
        }
    }
    //utworzenie animacji
    int *clip = new int [4];
    for(int k=0; k<4; k++)
        clip[k] = players[index]->clip[k];
    SDL_Texture *to_copy;
    if(eating>0 && players[index]->subclass==P_GHOST){
        to_copy = graphics->tex("ghost_eatme");
    }else{
        to_copy = players[index]->texture;
    }
    graphics->animations.push_back(new DeathAnimation(round_to_int(players[index]->a_x),round_to_int(players[index]->a_y),to_copy,players[index]->color,clip, graphics));
    synchro<<"050 "<<index<<" "<<round_to_int(players[index]->a_x)<<" "<<round_to_int(players[index]->a_y)<<"\r";
    //zabicie gracza
    players[index]->respawn = Config::respawn_time;
    //zmiana położenia
    int x, y;
    players[index]->moving = 0;
    players[index]->next_moving = 0;
    if(players[index]->subclass==P_PACMAN){ //pacman
        //odrodzenie w losowym miejscu
        random_field(x,y," .o");
    }else if(players[index]->subclass==P_GHOST){ //duszek
        //odrodzenie w miejscu respawnu
        random_field(x,y,"-");
    }
    players[index]->xmap = x;
    players[index]->ymap = y;
    xymap_to_xy(players[index]);
    players[index]->move_abs(players[index]->x,players[index]->y);
    //usunięcie ścieżki
    if(players[index]->sciezka!=nullptr){
        delete players[index]->sciezka;
        players[index]->sciezka = nullptr;
    }
    synchro<<"022 "<<index<<" "<<players[index]->moving<<" "<<players[index]->direction<<"\r";
    synchro<<"021 "<<index<<" "<<players[index]->xmap<<" "<<players[index]->ymap<<"\r";
    synchro<<"024 "<<index<<" "<<players[index]->respawn<<"\r";
}


void GameEngine::clear_map(){
    stringstream synchro2;
    eating = 0;
    synchro2<<"015 "<<eating<<"\r";
    round_next = Config::next_round_time-1;
    synchro2<<"013 "<<round_next<<"\r";
    game_c1 = 0;
    game_c2 = 0;
    game_c3 = 0;
    if(mode==MODE_ZOMBIE){
        game_c1 = Timer::s_to_cycles(Config::zombie_death_time) - 1;
    }
    synchro2<<"012 "<<mode<<"\r";
    synchro2<<"014 "<<game_c1<<" "<<game_c2<<" "<<game_c3<<"\r";
    //obiekty
    for(unsigned int i=0; i<items.size(); i++)
        delete items[i];
    items.clear();
    synchro2<<"042\r";
    if(network->server){
        network_send_to_clients(synchro2.str());
    }
}

void GameEngine::clear_all(){
    clear_map();
    for(unsigned int i=0; i<players.size(); i++){
        delete players[i];
    }
    players.clear();
    update_players_lists();
    stringstream synchro2;
    synchro2<<"032\r";
    if(network->server){
        network_send_to_clients(synchro2.str());
    }
}

void GameEngine::restart_map(){
    clear_map();
    insert_items();
    //gracze
    stringstream synchro2;
    for(unsigned int i=0; i<players.size(); i++){
        players[i]->score = 0;
        players[i]->moving = 0;
        players[i]->next_moving = 0;
        players[i]->respawn = 0;
        players[i]->direction = rand()%4;
        players[i]->next_direction = players[i]->direction;
        int x, y;
        if(players[i]->subclass==P_PACMAN){
            random_field(x,y," .o");
        }else{
            random_field(x,y,"-");
        }
        players[i]->xmap = x;
        players[i]->ymap = y;
        xymap_to_xy(players[i]);
        players[i]->move_abs(players[i]->x,players[i]->y);
        if(players[i]->sciezka!=nullptr){
            delete players[i]->sciezka;
            players[i]->sciezka = nullptr;
        }
        synchro2<<"021 "<<i<<" "<<players[i]->xmap<<" "<<players[i]->ymap<<"\r";
        synchro2<<"022 "<<i<<" "<<players[i]->moving<<" "<<players[i]->direction<<"\r";
        synchro2<<"023 "<<i<<" "<<players[i]->score<<"\r";
        synchro2<<"024 "<<i<<" "<<players[i]->respawn<<"\r";
    }
    if(network->server){
        network_send_to_clients(synchro2.str());
    }
}

void GameEngine::mode_init(){
    if(mode==MODE_CLASSIC){

    }
    if(mode==MODE_ZOMBIE){
        //zamiana wszystkich w pacmany
        for(unsigned int i=0; i<players.size(); i++){
            if(players[i]->subclass==P_GHOST){
                change_subclass(i,P_PACMAN);
            }
        }
        if(network->server){
            network_send_to_clients(synchro.str());
        }
    }
    restart_map();
}

void GameEngine::update_map(int client_id){
    stringstream synchro2;
    synchro2<<"015 "<<eating<<"\r";
    synchro2<<"013 "<<round_next<<"\r";
    synchro2<<"012 "<<mode<<"\r";
    synchro2<<"014 "<<game_c1<<" "<<game_c2<<" "<<game_c3<<"\r";
    //pauza
    if(pause){
        synchro2<<"010\r";
    }else{
        synchro2<<"011\r";
    }
    //położenie wszystkich itemów
    for(unsigned int i=0; i<items.size(); i++){
        synchro2<<"040 "<<items[i]->subclass<<" "<<items[i]->xmap<<" "<<items[i]->ymap<<"\r";
    }
    //dodanie graczy
    for(unsigned int i=0; i<players.size(); i++){
        synchro2<<"030 "<<players[i]->subclass<<" "<<(int)players[i]->color.r<<" "<<(int)players[i]->color.g<<" "<<(int)players[i]->color.b<<" "<<players[i]->name<<"\r";
        synchro2<<"021 "<<i<<" "<<players[i]->xmap<<" "<<players[i]->ymap<<"\r";//bezwzględne umieszczenie na mapie
        synchro2<<"022 "<<i<<" "<<players[i]->moving<<" "<<players[i]->direction<<"\r";
        synchro2<<"023 "<<i<<" "<<players[i]->score<<"\r";
        synchro2<<"024 "<<i<<" "<<players[i]->respawn<<"\r";
    }
    network_send_to_clients(synchro2.str(),client_id);
}

void GameEngine::language_refresh(){
    if(App::lang!=nullptr)
        delete App::lang;
    App::lang = new Language(Config::languages[Config::language_selected]);
}
