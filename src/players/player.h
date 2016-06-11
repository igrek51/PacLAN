/**
 * \file player.h
 * \see player.cpp
 */
#ifndef PLAYER_H
#define PLAYER_H

//rodzaj postaci
#define P_PACMAN 0
#define P_GHOST 1
//kierunek ruchu
#define P_RIGHT 0
#define P_UP 1
#define P_LEFT 2
#define P_DOWN 3
//sterowany za pomocą
#define P_KEYBOARD 0
#define P_AI 1
#define P_LAN 2

#include <SDL2/SDL.h>
#include <string>

class Player;
class GameEngine;
class Graphics;

#include "../map/pathfind/pathfind.h"

using namespace std;

/**
 * \brief Klasa gracza (pacmana lub duszka)
 */
class Player {
public:
    /**
     * Utworzenie nowego gracza na mapie
     * \param xmap współrzędna x w układzie mapy
     * \param ymap współrzędna y w układzie mapy
     * \param color kolor gracza (do wygenerowania tekstury)
     * \param name nazwa gracza
     * \param controlby określa, co steruje graczem: P_KEYBOARD - klawiatura, P_AI - sztuczna inteligencja, P_LAN - komunikaty sieciowe z serwera
     * \param game_engine silnik logiki gry
     * \param graphics silnik grafiki
     */
    Player(int xmap, int ymap, SDL_Color color, string name, int controlby, GameEngine* game_engine, Graphics* graphics);

    /// usunięcie gracza
    virtual ~Player();

    /// nazwa gracza - podpis nad graczem
    string name;
    /// podklasa gracza: P_PACMAN - pacman, P_GHOST - duszek
    int subclass;
    /// liczba punktów gracza
    int score;
    /// czas do odrodzenia się
    int respawn;

    //  POŁOŻENIE
public:
    /// położenie gracza wzdłuż osi x na tablicy mapy
    int xmap;
    /// położenie gracza wzdłuż osi y na tablicy mapy
    int ymap;
    /// wartość zadana położenia wzdłuż osi x na ekranie
    int x;
    /// wartość zadana położenia wzdłuż osi y na ekranie
    int y;
    /// aktualne położenie wzdłuż osi x na ekranie
    double a_x;
    /// aktualne położenie wzdłuż osi y na ekranie
    double a_y;

    //  RUCH
public:
    /// aktualny ruch gracza: 0 - brak ruchu, 1 - ruch
    int moving;
    /// aktualny kierunek gracza: P_LEFT, P_RIGHT, P_UP lub P_DOWN
    int direction;
    /// licznik pojawienia się możliwości ruchu (0 - możliwa zmiana kierunku)
    int can_move;
    /// preskaler licznika możliwości ruchu - definiuje szybkość gracza
    int can_move_n;
    /// poprzedni kierunek gracza
    int prev_direction;
    /// poprzedni stan ruchu gracza
    int prev_moving;

    /// wykonywanie ruchu względnego do punktu - oszacowanie przyrostów współrzędnychw każdej klatce
    void move();

    /**
     * bezwzględny skok do punktu
     * @param absx współrzędna x na ekranie
     * @param absy współrzędna y na ekranie
     */
    void move_abs(int absx, int absy);

    /// wykonanie płynnego ruchu do zadanej wartości (na podstawie wyznaczonych przyrostów)
    void move_smooth();

    /// wykonanie ruchu w losowym kierunku przez gracza
    void move_random();

private:
    /// przyrost położenia wzdłuż osi x w ciągu 1 klatki (w celu wykonywania płynnych ruchów)
    double a_dx;
    /// przyrost położenia wzdłuż osi y w ciągu 1 klatki (w celu wykonywania płynnych ruchów)
    double a_dy;

    //  ANIMACJA
public:
    /// kolor gracza
    SDL_Color color;
    /// tekstura gracza
    SDL_Texture* texture;
    /// numer obrazka w animacji
    int a_sprite;
    /// licznik do preskalera zmiany animacji
    int a_cycle;
    /// tablica aktualnej części obrazka wyciętej z całej tekstury: element 0 - indeks ramki wzdłuż osi x, element 1 - liczba wszystkich ramek wzdłuż osi x, element 2 - indeks ramki wzdłuż osi y, element 3 - liczba wszystkich ramek wzdłuż osi y
    int* clip;

    /**
     * narysowanie gracza w określonym miejscu na ekranie
     * @param texture2 tekstura do narysowania
     * @param x położenie wzdłuż osi x
     * @param y położenie wzdłuż osi y
     */
    void draw_sprite_at(SDL_Texture* texture2, int x, int y);

    /**
     * narysowanie gracza w jego położeniu na ekranie
     * @param texture2
     */
    void draw_sprite(SDL_Texture* texture2);

    /// narysowanie nad graczem jego nazwy
    void draw_name();

    /// wygenerowanie kolorowej tekstury gracza
    virtual void reload_texture() = 0;

    /// wygenerowanie parametrów kolejnej klatki gracza
    virtual void animate() = 0;

    /// narysowanie gracza na ekranie
    virtual void draw() = 0;

private:
    /// aktualizacja tablicy określającej parametry wycięcia fragmentu klatki z tekstury
    virtual void clip_table() = 0;

    //  STEROWANIE
public:
    /// określa, co steruje graczem: P_KEYBOARD - klawiatura, P_AI - sztuczna inteligencja, P_LAN - komunikaty sieciowe z serwera
    int controlby;
    /// poziom sztucznej inteligencji: od 0 do 5
    int ai_level;
    /// numer klienta, który obsługuje gracza: numer większy od 0
    int lan_id;
    /// następny planowany stan ruchu gracza
    int next_moving;
    /// następny planowany kierunek gracza
    int next_direction;
    /// wyznaczona scieżka podążania graczy sterowanych przez sztuczną inteligencję do określonych punktów
    Path<int>* sciezka;

    /// wykonanie ruchu gracza przez sztuczną inteligencję
    virtual void ai_control() = 0; //sterowanie przez sztuczną inteligencję komputera

protected:
    GameEngine* game_engine;
    Graphics* graphics;
};

#endif
