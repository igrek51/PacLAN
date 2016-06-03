#include "language.h"
#include "log/log.h"

Language::Language(string lang){
    this->lang = lang;
    if(lang=="Polski"){
        player_name = "Nazwa gracza: ";
        player_class = "Klasa gracza: ";
        player_class_pacman = "Pacman";
        player_class_ghost = "Duszek";
        player_color = "Kolor gracza: ";
        host_game = "Załóż nową grę: ";
        mode_classic = "Tryb Classic";
        mode_zombie = "Tryb Zombie";
        connect_to = "Dołącz do: ";
        language = "Język: ";
        exit = "Wyjście";
        controls = "Sterowanie:";
        controls_1 = "Strzałki - ruch gracza";
        controls_2 = "Spacja - zatrzymanie gracza";
        controls_3 = "Enter - pauza";
        controls_4 = "` - wiersz poleceń";
        controls_5 = "F11 - pełny ekran";
        controls_6 = "1,2,3,4,5 - szybkie dodanie duszków AI";

        pause = "PAUZA";
        eating_ghosts = "Zjadanie duszków: ";
        round_finished = "Koniec rundy: ";
        next_round = "Następna runda za: ";
        next_death = "Kolejna śmierć za: ";
        pacmans = "Pacmany";
        ghosts = "Duszki";
    }else if(lang=="English"){
        player_name = "Player name: ";
        player_class = "Player class: ";
        player_class_pacman = "Pacman";
        player_class_ghost = "Ghost";
        player_color = "Player color: ";
        host_game = "Host new game: ";
        mode_classic = "Classic mode";
        mode_zombie = "Zombie mode";
        connect_to = "Connect to: ";
        language = "Language: ";
        exit = "Exit";
        controls = "Controls:";
        controls_1 = "Arrows - move player";
        controls_2 = "Space - stop player";
        controls_3 = "Enter - pause";
        controls_4 = "` - console";
        controls_5 = "F11 - fullscreen";
        controls_6 = "1,2,3,4,5 - quick adding AI players";

        pause = "PAUSE";
        eating_ghosts = "Eating ghosts: ";
        round_finished = "Round finished: ";
        next_round = "Next round: ";
        next_death = "Next death: ";
        pacmans = "Pacmans";
        ghosts = "Ghosts";
    }
    Log::debug("Wczytano ustawienia językowe.");
}

Language::~Language(){

}