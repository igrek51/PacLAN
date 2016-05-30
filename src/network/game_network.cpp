#include "../logic/game_engine.h"
#include "../app.h"
#include "../graphics/animations/death_animation.h"
#include "../utils.h"

//TODO utworzyć osobną klasę (z nagłówkiem .h)
//TODO obserwator lub event dispatcher

void GameEngine::network_packet_process(int from, string packet){
    //cmd_output("Przetwarzam pakiet: "+packet);
    if(packet.length()==0) return;
    //podział na elementy pakietu
    vector<string> p;
    unsigned int start=0;
    for(unsigned int i=0; i<=packet.length(); i++){
        if(i==packet.length() || packet[i]==' '){
            if(i-start>0 && start<packet.length()){
                p.push_back(packet.substr(start,i-start));
            }
            start = i+1;
        }
    }
    if(p.size()==0) return;
    //  POŁĄCZENIE
    if(p[0]=="001"){ // polecenie zdalne: 001 [polecenie zdalne]
        string cmd = "";
        for(unsigned int i=1; i<p.size(); i++){
            if(i>1) cmd += ' ';
            cmd += p[i];
        }
        cmd_output("Polecenie zdalne: "+cmd);
        exec_cmd(cmd);
    }else if(p[0]=="002"){ // wiadomość tekstowa: 002 [wiadomosc tekstowa]
        string message = "";
        for(unsigned int i=1; i<p.size(); i++){
            if(i>1) message += ' ';
            message += p[i];
        }
        stringstream ss;
        ss<<"Wiadomość od "<<from<<": "<<message;
        cmd_output(ss.str());
    }else if(p[0]=="003"){ // synchronizacja zegara
        cmd_output("Zsynchronizowano timer");
    }else if(p[0]=="004"){ // pomyślnie połączono z serwerem
        menu = 0;
        stringstream synchro2;
        //prośba o aktualizację obecnej listy graczy i całej mapki
        synchro2<<"103\r";
        //prośba o dopisanie do listy graczy i przyporządkowanie do klienta
        synchro2<<"100 "<<menu_subclass<<" "<<(int)menu_color.r<<" "<<(int)menu_color.g<<" "<<(int)menu_color.b<<" "<<menu_name<<"\r";
        network_send_to_server(synchro2.str());
        cmd_output("Pomyślnie połączono z serwerem.");
    }else if(p[0]=="010"){ //   PARAMETRY GRY: pazua - zatrzymanie
        pause = true;
        for(unsigned int i=0; i<players.size(); i++){
            players[i]->moving = 0;
        }
    }else if(p[0]=="011"){ // pauza - kontynuuj
        pause = false;
    }else if(p[0]=="012"){ // zmiana trybu gry: 012 [mode]
        if(p.size()>=2){
            int mode_i = atoi(p[1].c_str());
            mode = mode_i;
        }
    }else if(p[0]=="013"){ // czas do następnej rundy: 013 [round_next]
        if(p.size()>=2){
            round_next = atoi(p[1].c_str());
        }
    }else if(p[0]=="014"){ // pomocnicze liczniki trybu gry: 014 [game_c1] [game_c2] [game_c3]
        if(p.size()>=2){
            game_c1 = atoi(p[1].c_str());
            game_c2 = atoi(p[2].c_str());
            game_c3 = atoi(p[3].c_str());
        }
    }else if(p[0]=="015"){ // ustaw czas zjadania duszków (lub wyłącz): 015 [eating]
        if(p.size()>=2){
            eating = atoi(p[1].c_str());
        }
    }else if(p[0]=="020"){ //   GRACZ: przesuń playera płynnie: 020 [player_id] [xmap] [ymap]
        if(p.size()>=4){
            int player_i = atoi(p[1].c_str());
            if(player_i<0 || player_i>=(int)players.size()) return;
            int xmap = atoi(p[2].c_str());
            int ymap = atoi(p[3].c_str());
            players[player_i]->xmap = xmap;
            players[player_i]->ymap = ymap;
            xymap_to_xy(players[player_i]);
            players[player_i]->move();
            players[player_i]->can_move = 0; //synchronizacja cykli playera
        }
    }else if(p[0]=="021"){ // przesuń playera bezwzględnie: 021 [index_playera] [xmap] [ymap]
        if(p.size()>=4){
            int player_i = atoi(p[1].c_str());
            if(player_i<0 || player_i>=(int)players.size()) return;
            int xmap = atoi(p[2].c_str());
            int ymap = atoi(p[3].c_str());
            players[player_i]->xmap = xmap;
            players[player_i]->ymap = ymap;
            xymap_to_xy(players[player_i]);
            players[player_i]->move_abs(players[player_i]->x,players[player_i]->y);
            players[player_i]->can_move = 0;
        }
    }else if(p[0]=="022"){ // zmień kierunek i ruch: 022 [index_playera] [moving] [direction]
        if(p.size()>=4){
            int player_i = atoi(p[1].c_str());
            if(player_i<0 || player_i>=(int)players.size()) return;
            int moving = atoi(p[2].c_str());
            int direction = atoi(p[3].c_str());
            players[player_i]->moving = moving%2;
            players[player_i]->direction = direction%4;
            players[player_i]->next_moving = players[player_i]->moving;
            players[player_i]->next_direction = players[player_i]->direction;
        }
    }else if(p[0]=="023"){ // zmień punkty wyniku gracza: 023 [player_id] [score]
        if(p.size()>=3){
            int player_i = atoi(p[1].c_str());
            if(player_i<0 || player_i>=(int)players.size()) return;
            int score = atoi(p[2].c_str());
            players[player_i]->score = score;
            sort_players();
        }
    }else if(p[0]=="024"){ // ustaw licznik respawnu: 024 [index_playera] [respawn]
        if(p.size()>=3){
            int player_i = atoi(p[1].c_str());
            if(player_i<0 || player_i>=(int)players.size()) return;
            int respawn = atoi(p[2].c_str());
            players[player_i]->respawn = respawn;
        }
    }else if(p[0]=="025"){ // zmiana klasy gracza: 025 [numer_gracza] [subclass]
        if(p.size()>=3){
            int player_i = atoi(p[1].c_str());
            if(player_i<0 || player_i>=(int)players.size()) return;
            int subclass_i = atoi(p[2].c_str());
            change_subclass(player_i,subclass_i);
        }
    }else if(p[0]=="026"){ // zmiana koloru gracza: 026 [numer_gracza] [r] [g] [b]
        if(p.size()>=5){
            int player_i = atoi(p[1].c_str());
            if(player_i<0 || player_i>=(int)players.size()) return;
            int color_r = atoi(p[2].c_str());
            int color_g = atoi(p[3].c_str());
            int color_b = atoi(p[4].c_str());
            players[player_i]->color = Graphics::rgba(color_r,color_g,color_b);
            players[player_i]->reload_texture();
        }
    }else if(p[0]=="027"){ // zmiana nazwy gracza: 027 [numer_gracza] [nowa nazwa]
        if(p.size()>=3){
            int player_i = atoi(p[1].c_str());
            if(player_i<0 || player_i>=(int)players.size()) return;
            stringstream new_name;
            for(unsigned int i=2; i<p.size(); i++){
                if(i!=2) new_name<<" ";
                new_name<<p[i];
            }
            players[player_i]->name = new_name.str();
        }
    }else if(p[0]=="028"){ // zmień następny kierunek i ruch: 028 [index_playera] [next_moving] [next_direction]
        if(p.size()>=4){
            int player_i = atoi(p[1].c_str());
            if(player_i<0 || player_i>=(int)players.size()) return;
            int next_moving = atoi(p[2].c_str());
            int next_direction = atoi(p[3].c_str());
            players[player_i]->next_moving = next_moving;
            players[player_i]->next_direction = next_direction;
        }
    }else if(p[0]=="030"){ //   LISTA GRACZY: dodaj nowego gracza: 030 [subclass] [color.r] [color.g] [color.b] [nazwa gracza]
        if(p.size()>=6){
            int subclass = atoi(p[1].c_str());
            int color_r = atoi(p[2].c_str());
            int color_g = atoi(p[3].c_str());
            int color_b = atoi(p[4].c_str());
            stringstream name;
            for(unsigned int i=5; i<p.size(); i++){
                if(i>5) name<<' ';
                name<<p[i];
            }
            add_player(subclass, name.str(), Graphics::rgba(color_r,color_g,color_b), P_LAN);
        }
    }else if(p[0]=="031"){ // usuń gracza: 031 [player_id]
        if(p.size()>=2){
            int player_id = atoi(p[1].c_str());
            kick_player(player_id);
        }
    }else if(p[0]=="032"){ // usuń wszystkich graczy: 032
        while(players.size()>0){
            kick_player(0);
        }
    }else if(p[0]=="040"){ //   ITEMY: utworzenie itemu: 040 [item_type] [x] [y]
        if(p.size()==4){
            int item_i = atoi(p[1].c_str());
            int x = atoi(p[2].c_str());
            int y = atoi(p[3].c_str());
            items.push_back(new Item(x,y,item_i));
        }
    }else if(p[0]=="041"){ // usunięcie itemu: 041 [item_type] [x] [y]
        if(p.size()>=4){
            int item_type = atoi(p[1].c_str());
            int xmap = atoi(p[2].c_str());
            int ymap = atoi(p[3].c_str());
            for(unsigned int item_index=0; item_index<items.size(); item_index++){
                if(items[item_index]->subclass==item_type && items[item_index]->xmap==xmap && items[item_index]->ymap==ymap){
                    delete items[item_index];
                    items.erase(items.begin()+item_index);
                    break;
                }
            }
        }
    }else if(p[0]=="042"){ // usunięcie wszystkich itemów: 042
        for(unsigned int i=0; i<items.size(); i++)
            delete items[i];
        items.clear();
    }else if(p[0]=="050"){ //   ANIMACJE: utworzenie animacji śmierci: 050 [player_index] [x] [y]
        if(p.size()>=4){
            int player_index = atoi(p[1].c_str());
            int x = atoi(p[2].c_str());
            int y = atoi(p[3].c_str());
            //utworzenie animacji
            int *clip = new int [4];
            for(int k=0; k<4; k++)
                clip[k] = players[player_index]->clip[k];
            SDL_Texture *to_copy;
            if(eating>0 && players[player_index]->subclass==P_GHOST){
                to_copy = App::graphics->tex("ghost_eatme");
            }else{
                to_copy = players[player_index]->texture;
            }
            App::graphics->animations.push_back(new DeathAnimation(x,y,to_copy,players[player_index]->color,clip));
        }
    }else if(p[0]=="100"){ // KOMUNIKATY OD KLIENTA: prośba dodania nowego gracza od klienta i przyporządkowania do klienta: 100 [subclass] [color.r] [color.g] [color.b] [nazwa gracza]
        if(p.size()>=6){
            if(App::network->server){ //odpowiedź serwera
                int subclass = atoi(p[1].c_str());
                int color_r = atoi(p[2].c_str());
                int color_g = atoi(p[3].c_str());
                int color_b = atoi(p[4].c_str());
                stringstream name;
                for(unsigned int i=5; i<p.size(); i++){
                    if(i>5) name<<' ';
                    name<<p[i];
                }
                if(from<=0 || from>=(int)App::network->connections.size()) return;
                //dodaj gracza u wszystkich
                Player *nowy = add_player(subclass, name.str(), Graphics::rgba(color_r,color_g,color_b), P_LAN);
                nowy->lan_id = from;
                //zsycnhronizuj położenie graczy u klienta
                synchronize_players(from);
                stringstream ss;
                ss<<"Przypisano gracza "<<name.str()<<" do klienta "<<from;
                cmd_output(ss.str());
            }
        }
    }else if(p[0]=="101"){ // sygnał zmiany ruchu od klienta: 101 [next_moving]
        if(p.size()>=2){
            if(App::network->server){ //odpowiedź serwera
                if(from<=0 || from>=(int)App::network->connections.size()) return;
                int next_moving = atoi(p[1].c_str());
                stringstream synchro2;
                //szukaj gracza, który jest sterowany - możliwość wielu
                for(unsigned int i=0; i<players.size(); i++){
                    if(players[i]->lan_id == from){
                        players[i]->next_moving = next_moving;
                        synchro2<<"028 "<<i<<" "<<players[i]->next_moving<<" "<<players[i]->next_direction<<"\r";
                    }
                }
                //wysłanie informacji zamiaru ruchu pozostałym klientom
                if(synchro2.str().length()==0) return;
                packet += '\n';
                for(unsigned int i=1; i<App::network->connections.size(); i++){
                    //if((int)i==from) continue;
                    App::network->addtask_send_to_client(i, string_to_char(synchro2.str()), synchro2.str().length());
                }
            }
        }
    }else if(p[0]=="102"){ // sygnał zmiany kierunku od klienta (moving=1): 102 [next_direction]
        if(p.size()>=2){
            if(App::network->server){ //odpowiedź serwera
                if(from<=0 || from>=(int)App::network->connections.size()) return;
                int next_direction = atoi(p[1].c_str());
                stringstream synchro2;
                //szukaj gracza, który jest sterowany - możliwość wielu
                for(unsigned int i=0; i<players.size(); i++){
                    if(players[i]->lan_id == from){
                        players[i]->next_moving = 1;
                        players[i]->next_direction = next_direction;
                        synchro2<<"028 "<<i<<" "<<players[i]->next_moving<<" "<<players[i]->next_direction<<"\r";
                    }
                }
                //wysłanie informacji zamiaru ruchu pozostałym klientom
                if(synchro2.str().length()==0) return;
                packet += '\n';
                for(unsigned int i=1; i<App::network->connections.size(); i++){
                    //if((int)i==from) continue;
                    App::network->addtask_send_to_client(i, string_to_char(synchro2.str()), synchro2.str().length());
                }
            }
        }
    }else if(p[0]=="103"){ // prośba o aktualizację całej mapki i graczy do wybranego klienta
        if(App::network->server){ //odpowiedź serwera
            update_map(from);
        }
    }else if(p[0]=="201"){ // klient został rozłączony: 201 [client_id]
        if(p.size()>=2){
            if(App::network->server){ //odpowiedź serwera
                int client_id = atoi(p[1].c_str());
                if(client_id<=0) return;
                //usunięcie graczy sterowanych przez klienta
                for(unsigned int i=0; i<players.size(); i++){
                    if(players[i]->controlby==P_LAN && players[i]->lan_id==client_id){
                        kick_player(i);
                        i--;
                    }
                }
                //przesunięcie indeksów lan_id
                for(unsigned int i=0; i<players.size(); i++){
                    if(players[i]->controlby==P_LAN && players[i]->lan_id>client_id){
                        players[i]->lan_id--;
                    }
                }
                stringstream ss;
                ss<<"Rozłączono klienta nr "<<client_id;
                cmd_output(ss.str());
            }
        }
    }else{
        cmd_output("[!] Nieprawidłowy format pakietu");
    }
}

void GameEngine::network_packets_split(int from, string packet){
    unsigned int start=0;
    for(unsigned int i=0; i<=packet.length(); i++){
        if(packet[i]=='\r' || i==packet.length()){
            if(i-start>0 && start<packet.length()){
                network_packet_process(from, packet.substr(start,i-start));
            }
            start = i+1;
        }
    }
}

void GameEngine::network_send_to_clients(string packet, int to){
    if(packet.length()==0) return;
    if(App::network->server){
        packet += '\n';
        if(to==-1){
            for(unsigned int i=1; i<App::network->connections.size(); i++){
                App::network->addtask_send_to_client(i, string_to_char(packet), packet.length());
            }
        }else{
            if(to<=0 || to>=(int)App::network->connections.size()) return;
            App::network->addtask_send_to_client(to, string_to_char(packet), packet.length());
        }
    }
}

void GameEngine::network_send_to_server(string packet){
    if(packet.length()==0) return;
    if(App::network->client){
        packet += '\n';
        App::network->addtask_send_to_server(string_to_char(packet), packet.length());
    }
}
