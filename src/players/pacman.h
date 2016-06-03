#ifndef PACMAN_H
#define PACMAN_H

class Pacman;
class Player;

#include "player.h"

/**
 * \brief Klasa gracza będącego pacmanem
 */
class Pacman : public Player {
public:
    /**
     * Utworzenie nowego pacmana na mapie
     * \param xmap współrzędna x w układzie mapy
     * \param ymap współrzędna y w układzie mapy
     * \param color kolor gracza (do wygenerowania tekstury)
     * \param name nazwa gracza
     * \param controlby określa, co steruje graczem: P_KEYBOARD - klawiatura, P_AI - sztuczna inteligencja, P_LAN - komunikaty sieciowe z serwera
     */
    Pacman(int xmap, int ymap, SDL_Color color, string name, int controlby, GameEngine* game_engine);
    /// usunięcie pacmana
    ~Pacman();
    /// wygenerowanie kolorowej tekstury gracza
    void reload_texture();
    /// wygenerowanie parametrów kolejnej klatki gracza
    void animate();
private:
    /// narysowanie gracza na ekranie
    void draw();
    /// aktualizacja tablicy określającej parametry wycięcia fragmentu klatki z tekstury
    void clip_table();
    /// wykonanie ruchu gracza przez sztuczną inteligencję
    void ai_control();
    /// w sztucznej inteligencji: histereza przy uciekaniu od duszków (tryb uciekania włączony lub wyłączony)
    int esc_histereza;
};

#endif
