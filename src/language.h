#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <iostream>

using namespace std;

/**
 * \brief Ustawienia językowe - teksty interfejsu w różnych językach
 */
class Language{
private:
    string lang;
public:
    Language(string lang);
    ~Language();
    //  MENU
    string player_name;
    string player_class;
    string player_class_pacman;
    string player_class_ghost;
    string player_color;
    string host_game;
    string mode_classic;
    string mode_zombie;
    string connect_to;
    string language;
    string exit;
    string controls;
    string controls_1;
    string controls_2;
    string controls_3;
    string controls_4;
    string controls_5;
    string controls_6;
    //  GRA - RUNDA
    string pause;
    string eating_ghosts;
    string round_finished;
    string next_round;
    string next_death;
    string pacmans;
    string ghosts;

};

#endif
