#include "game_engine.h"
#include "../config.h"
#include "../log/log.h"
#include "../app.h"
#include "../utils.h"
#include "../players/pacman.h"
#include "../players/ghost.h"

void GameEngine::exec_cmd(string cmd){
    if(cmd.length()==0){
        cmd_in = cmd_last;
        return;
    }
    cmd_last = cmd;
    cmd_in = "";
    //podział na główne polecenie i parametry
    vector<string> cmds;
    unsigned int start=0;
    for(unsigned int i=0; i<=cmd.length(); i++){
        if(i==cmd.length() || cmd[i]==' '){
            if(i-start>0 && start<cmd.length()){
                cmds.push_back(cmd.substr(start,i-start));
            }
            start = i+1;
        }
    }
    stringstream ss;
    ss<<">> "<<cmd;
    Log::info(ss.str());
    ss_clear(ss);
    if(cmd=="exit"){
        App::exit = true;
    }else if(cmds[0]=="add"){ //dodawanie nowego gracza: add [pacman,ghost] (ai [0-5])
        if(cmds.size()<2){
            Log::info("[!] Blad: brak klasy gracza (pacman lub ghost)");
            Log::info("Wpisz \"add --help\" w celu uzyskania pomocy do polecenia.");
            return;
        }
        if(cmds[1]=="--help"){
            Log::info("Przykłady poprawnego użycia polecenia \"add\":");
            Log::info("Dodanie pacmana sterowanego klawiaturą: add pacman");
            Log::info("Dodanie pacmana AI o poziomie inteligencji 3: add pacman ai 3");
            Log::info("Dodanie duszka sterowanego klawiaturą: add ghost");
            Log::info("Dodanie duszka AI o poziomie inteligencji 5: add ghost ai 5");
            return;
        }
        int subclass;
        if(cmds[1]=="ghost"){
            subclass = P_GHOST;
        }else if(cmds[1]=="pacman"){
            subclass = P_PACMAN;
        }else{
            Log::info("[!] Blad: niepoprawna klasa gracza (pacman lub ghost)");
            Log::info("Wpisz \"add --help\" w celu uzyskania pomocy do polecenia.");
            return;
        }
        int controlby = P_KEYBOARD;
        int ai_level = -1;
        if(cmds.size()>=3){
            if(cmds[2]=="ai"){
                controlby = P_AI;
                if(cmds.size()>=4)
                    ai_level = atoi(cmds[3].c_str());
                if(!(ai_level>=0&&ai_level<=5))
                    ai_level = rand()%6;
            }else{
                Log::info("[!] Blad: niepoprawna składnia.");
                Log::info("Wpisz \"add --help\" w celu uzyskania pomocy do polecenia.");
                return;
            }
        }
        add_player(subclass, "", Graphics::rand_bcolor(), controlby, ai_level);
    }else if(cmds[0]=="kick"){ //wyrzucenie gracza: kick [0,1,2,...], kick [pacmans,ghosts,all]
        if(cmds.size()<2){
            Log::info("[!] Blad: poprawna skladnia: kick [0,1,2,...], kick [pacmans,ghosts,all]");
            return;
        }
        int subclass = -1;
        if(cmds[1]=="pacmans"){
            subclass = P_PACMAN;
        }else if(cmds[1]=="ghosts"){
            subclass = P_GHOST;
        }else if(cmds[1]=="all"){
            subclass = 3;
        }
        if(subclass!=-1){
            for(unsigned int i=0; i<players.size(); i++){
                if(subclass==3 || players[i]->subclass==subclass){
                    kick_player(i);
                    i--;
                }
            }
            update_players_lists();
            return;
        }
        int index = atoi(cmds[1].c_str());
        if(index<0 || index>=(int)players.size()){
            ss<<"[!] Błąd: Nie znaleziono gracza nr: "<<index;
            Log::info(ss.str());
            return;
        }
        kick_player(index);
    }else if(cmds[0]=="change"){ //zmiana na przeciwną klasę: change [0,1,...]
        if(cmds.size()==2){
            int index = atoi(cmds[1].c_str());
            if(index<0 || index>=(int)players.size()){
                ss<<"[!] Błąd: Nie znaleziono gracza nr: "<<index;
                Log::info(ss.str());
                return;
            }
            change_subclass(index);
            ss<<"Zmieniono klasę gracza: "<<index;
            Log::info(ss.str());
        }
    }else if(cmds[0]=="color"){ //zmiana koloru: color [player_id] [r] [g] [b]
        if(cmds.size()>=2){
            int index = atoi(cmds[1].c_str());
            if(index<0 || index>=(int)players.size()){
                ss<<"[!] Błąd: Nie znaleziono gracza nr: "<<index;
                Log::info(ss.str());
                return;
            }
            if(cmds.size()==5){
                int r = atoi(cmds[2].c_str());
                int g = atoi(cmds[3].c_str());
                int b = atoi(cmds[4].c_str());
                players[index]->color = Graphics::rgba(r,g,b);
            }else{
                players[index]->color = Graphics::rand_bcolor();
            }
            players[index]->reload_texture();
            ss<<"Zmieniono kolor gracza: "<<index;
            Log::info(ss.str());
            stringstream synchro2;
            synchro2<<"026 "<<index<<" "<<(int)players[index]->color.r<<" "<<(int)players[index]->color.g<<" "<<(int)players[index]->color.b<<"\r";
            network_send_to_clients(synchro2.str());
        }
    }else if(cmds[0]=="name"){ //zmiana nazwy gracza: name [player_id] [nowa nazwa]
        if(cmds.size()>=3){
            int index = atoi(cmds[1].c_str());
            if(index<0 || index>=(int)players.size()){
                ss<<"[!] Błąd: Nie znaleziono gracza nr: "<<index;
                Log::info(ss.str());
                return;
            }
            stringstream new_name;
            for(unsigned int i=2; i<cmds.size(); i++){
                if(i!=2) new_name<<" ";
                new_name<<cmds[i];
            }
            players[index]->name = new_name.str();
            ss<<"Zmieniono nazwę gracza: "<<index;
            Log::info(ss.str());
            stringstream synchro2;
            synchro2<<"027 "<<index<<" "<<players[index]->name<<"\r";
            network_send_to_clients(synchro2.str());
        }
    }else if(cmds[0]=="map"){
        if(cmds.size()==2){
            if(cmds[1]=="restart"){
                mode_init();
                ss<<"Zrestartowano mapę";
                Log::info(ss.str());
            }
        }
    }else if(cmds[0]=="players"){
        ss<<"Lista graczy ("<<players.size()<<"):";
        Log::info(ss.str());
        for(unsigned int i=0; i<players.size(); i++){
            ss_clear(ss);
            ss<<"Player "<<i<<".: "<<players[i]->name<<", ";
            if(players[i]->subclass==P_PACMAN) ss<<"Pacman";
            else ss<<"Duszek";
            ss<<", punkty: "<<players[i]->score<<", ";
            if(players[i]->controlby==P_KEYBOARD){
                ss<<"Klawiatura";
            }else if(players[i]->controlby==P_AI){
                ss<<"AI level "<<players[i]->ai_level;
            }else if(players[i]->controlby==P_LAN){
                ss<<"LAN - klient nr "<<players[i]->lan_id;
            }
            ss<<", kolor: ("<<(int)players[i]->color.r<<", "<<(int)players[i]->color.g<<", "<<(int)players[i]->color.b<<")";
            Log::info(ss.str());
        }
    }else if(cmds[0]=="mode"){
        if(cmds.size()==2){
            if(cmds[1]=="classic"){
                mode = MODE_CLASSIC;
            }else if(cmds[1]=="zombie"){
                mode = MODE_ZOMBIE;
            }
            mode_init();
        }
    }else if(cmds[0]=="myip"){
        ss<<"Mój adres IP: "<< network->myIP();
        Log::info(ss.str());
        ss_clear(ss);
        ss<<"Moja nazwa: "<< network->myName();
        Log::info(ss.str());
    }else if(cmds[0]=="clients"){
        if(network->server){
            ss<<"Połączeni klienci ("<<network->connections.size()-1<<"):";
            Log::info(ss.str());
            for(unsigned int i=1; i<network->connections.size(); i++){
                ss_clear(ss);
                ss<<"Klient "<<i<<".: "<< network->clientIP(i);
                Log::info(ss.str());
            }
        }else{
            Log::info("Połączenie serwera nie zostało utworzone.");
        }
    }else if(cmds[0]=="remote"){
        if(cmds.size()>=3){
            int receiver_index = atoi(cmds[1].c_str());
            string send_string = "001 ";
            for(unsigned int i=2; i<cmds.size(); i++){
                if(i>2) send_string += ' ';
                send_string += cmds[i];
            }
            send_string += '\n';
            if(receiver_index==0){
                network->addtask_send_to_server(string_to_char(send_string), send_string.length());
            }else{
                network->addtask_send_to_client(receiver_index, string_to_char(send_string), send_string.length());
            }
        }
    }else if(cmds[0]=="control"){ //ustaw sterowanie graczem przez klienta: control [player_id] [client_id]
        if(cmds.size()>=3){
            int player_id = atoi(cmds[1].c_str());
            if(player_id<0 || player_id>=(int)players.size()){
                ss<<"[!] Błąd: Nie znaleziono gracza nr: "<<player_id;
                Log::info(ss.str());
                return;
            }
            int client_id = atoi(cmds[2].c_str());
            if(client_id<0 || client_id>=(int)network->connections.size()){
                ss<<"[!] Błąd: Nie znaleziono klienta nr: "<<client_id;
                Log::info(ss.str());
                return;
            }
            players[player_id]->lan_id = client_id;
            if(client_id==0){
                players[player_id]->controlby = P_KEYBOARD;
            }else{
                players[player_id]->controlby = P_LAN;
            }
            stringstream ss;
            ss<<"Przypisano gracza "<<player_id<<" do klienta "<<client_id;
            Log::info(ss.str());
        }
    }else if(cmds[0]=="neww"){ //nowe okno aplikacji
        system("start PacLAN.exe");
    }else if(cmds[0]=="log"){ //otwórz dziennik
        system("gedit log.txt&");
    }else if(cmds[0]=="pause"){
        pause = !pause;
        if(network->server){
            if(pause){
                synchronize_players();
                network_send_to_clients("010");
            }else{
                network_send_to_clients("011");
                synchronize_players();
            }
        }
    }else if(cmds[0]=="grid"){
        Config::show_grid = !Config::show_grid;
    }else if(cmds[0]=="paths"){
        Config::show_paths = !Config::show_paths;
    }else if(cmds[0]=="names"){
        Config::show_names = !Config::show_names;
    }else if(cmds[0]=="fps"){
        Config::show_fps = !Config::show_fps;
    }else if(cmds[0]=="fullscreen"){
        graphics->fullscreen_toggle();
    }else if(cmds[0]=="help"){
        Log::info("Dostępne polecenia:");
        Log::info("players - lista graczy (wyświetla ID graczy, nazwy, statystyki)");
        Log::info("add [pacman,ghost] (ai [0-5]) - nowy gracz (wpisz \"add --help\" w celu uzyskania pomocy)");
        Log::info("kick [playerID], kick [pacmans,ghosts,all] - usuniecie gracza (grupy graczy)");
        Log::info("change [playerID] - zmiana klasy gracza o podanym ID");
        Log::info("name [playerID] [nowa_nazwa] - zmiana nazwy gracza");
        Log::info("color [playerID] ([r] [g] [b]) - zmiana koloru gracza (na losowy lub wybrany)");
        Log::info("mode [classic,zombie] - zmiana trybu gry");
        Log::info("map restart - restart mapki");
        Log::info("myip - mój adres IP i nazwa komputera");
        Log::info("clients - połączeni klienci (wyświetla numery ID klientów oraz adresy IP)");
        Log::info("control [playerID] [clientID] - przydziel wybranemu klientowi sterowanie wybranym graczem");
        Log::info("remote [0,clientID] [polecenie] - wykonaj zdalne polecenie na serwerze (ID=0) lub u klienta");
        Log::info("neww - uruchamia nową instancję aplikacji");
        Log::info("Inne polecenia: pause, fullscreen, names, paths, grid, fps, log");
    }else{
        Log::info("[!] Błąd: Brak polecenia: "+cmd);
    }
}

void GameEngine::cmd_input_char(SDL_Keycode key){
    if(key==SDLK_BACKSPACE){
        if(cmd_in.length()>0)
            cmd_in = cmd_in.substr(0,cmd_in.length()-1);
        return;
    }
    if(key==SDLK_ESCAPE){
        cmd_on = !cmd_on;
        return;
    }
    if(key==SDLK_RETURN || key==SDLK_KP_ENTER){
        exec_cmd(cmd_in);
        return;
    }
    if(key==SDLK_LSHIFT || key==SDLK_RSHIFT || key==SDLK_LCTRL || key==SDLK_RCTRL)
        return;
    if(key==SDLK_LEFT || key==SDLK_RIGHT)
        return;
    if(key==SDLK_UP){
        cmd_in = cmd_last;
        return;
    }
    if(key==SDLK_DOWN){
        cmd_in = "";
        return;
    }
    char c = key_to_char(key);
    if(c!=0){
        cmd_in += c;
    }
}

char GameEngine::key_to_char(SDL_Keycode key){
    if(key==SDLK_BACKSPACE || key==SDLK_DELETE) return 0;
    if(key==SDLK_ESCAPE) return 0;
    if(key==SDLK_TAB) return 0;
    if(key==SDLK_RETURN || key==SDLK_KP_ENTER) return 0;
    if(key==SDLK_F4 || key==SDLK_F11) return 0;
    if(key==SDLK_LALT || key==SDLK_RALT) return 0;
    if(key==SDLK_LSHIFT || key==SDLK_RSHIFT || key==SDLK_LCTRL || key==SDLK_RCTRL) return 0;
    if(key==SDLK_UP || key==SDLK_DOWN || key==SDLK_LEFT || key==SDLK_RIGHT) return 0;
    char c = (char)key;
    if(key>=SDLK_a && key<=SDLK_z){ //litery
        if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]){
            //duże litery
            c = (char)(key+'A'-'a');
        }else{
            //małe litery
            c = (char)(key);
        }
    }
    if(key==SDLK_SPACE)
        c = (char)SDLK_SPACE;
    if(key>=SDLK_0 && key<=SDLK_9)
        c = (char)(key-SDLK_0)+'0';
    if(key>=SDLK_KP_1 && key<=SDLK_KP_9){
        c = (char)(key-SDLK_KP_1)+'1';
    }
    if(key==SDLK_KP_9+1) //keypad 0
        c = '0';
    if(key==SDLK_MINUS && (keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]))
        c = '_';
    if(key==SDLK_KP_PLUS)
        c = '+';
    if(key==SDLK_KP_MINUS)
        c = '-';
    return c;
}

