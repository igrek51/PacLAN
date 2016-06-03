#ifndef ITEM_H
#define ITEM_H

class GameEngine;
class Graphics;
class Item;

//rodzaj obiektu
#include "../logic/game_engine.h"

#define I_SMALLDOT 1 //mała kropka do zebrania
#define I_LARGEDOT 2 //duża kropka (bonusowa)

/**
 * \brief Obiekt do zebrania na mapie
 */
class Item{
public:
    /**
     * Utworzenie nowego obiektu na mapie
     * \param xmap współrzędna x w układzie mapy
     * \param ymap współrzędna y w układzie mapy
     * \param subclass liczba określająca rodzaj obiektu
     */
    Item(int xmap, int ymap, int subclass);
    ~Item();
    /// współrzędna x w układzie mapy
    int xmap;
    /// współrzędna y w układzie mapy
    int ymap;
    /// podklasa obiektu - liczba określająca rodzaj obiektu
    int subclass;
    /// rysowanie obiektu na ekranie
    void draw();
private:
    /// współrzędna x w układzie ekranu
    int x;
    /// współrzędna y w układzie ekranu
    int y;

    GameEngine* game_engine;
    Graphics* graphics;
};

#endif
