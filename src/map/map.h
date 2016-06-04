/**
 * \file map.h
 * \see map.cpp
 */
#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

using namespace std;

/**
 * \brief Mapa gry o określonym rozmiarze zawierająca przeszkody i specyficzne pola
 */
class Map {
public:
    /**
     * tworzy nową mapę
     * @param background tekstura tła mapy
     * @param gridfile plik określający rozmiar mapy oraz jej zawartość
     */
    Map(SDL_Texture *background, string gridfile);
    /// usunięcie mapy
    ~Map();
    /// tekstura tła mapy
    SDL_Texture *background;
    ///ilość kolumn mapy
    int grid_w;
    ///ilość wierszy mapy
    int grid_h;
    /// zawartosć mapy - grid[wiersz][kolumna]
    /// Możliwe znaki:
    /// x - ściana
    /// ' ' - puste pole, możliwość ruchu
    /// '-' - miejsce tylko dla duszków, respawn duszków
    /// . - mała kropka do zebrania
    /// o - kropka bonusowa
    char **grid;
    /// położenie początku siatki wzduż osi x na ekranie
    int grid_x_pos;
    /// położenie początku siatki wzduż osi y na ekranie
    int grid_y_pos;
    /// odległość między kratkami mapy
    double grid_space;
    /**
     * zwraca zawartość kratki (z przekraczaniem rozmiarów mapy)
     * @param x położenie wzdłuż osi x na mapie
     * @param y położenie wzdłuż osi y na mapie
     * @return zwraca zawartość pola mapy, jeśli położenie przekracza rozmiar mapy, zwraca '0'
     */
    char get_grid(int x, int y);
private:
    /**
     * załadowanie mapy z pliku
     * @param gridfile nazwa pliku
     * @return jeśli pomyślnie wczytano mapę, zwraca true, w przeciwnym razie - false
     */
    bool load_grid(string gridfile);
};

#endif
