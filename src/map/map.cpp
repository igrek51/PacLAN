#include <fstream>
#include "map.h"
#include "../log/log.h"

Map::Map(SDL_Texture* background, string gridfile) {
    this->background = background;
    this->grid = nullptr;
    if (!load_grid(gridfile)) {
        delete this;
    }
}

Map::~Map() {
    if (grid != nullptr) {
        for (int i = 0; i < grid_w; i++) {
            delete[] grid[i];
        }
        delete[] grid;
    }
}

bool Map::load_grid(string gridfile) {
    fstream plik;
    plik.open(gridfile.c_str(), fstream::in | fstream::binary);
    if (!plik.good()) {
        Log::criticalError("Blad odczytu pliku: " + gridfile);
        return false;
    }
    plik >> grid_x_pos >> grid_y_pos;
    plik >> grid_space;
    plik >> grid_w >> grid_h;
    grid = new char* [grid_h];
    string linia;
    getline(plik, linia, '\n');
    for (int w = 0; w < grid_h; w++) {
        getline(plik, linia, '\n');
        if (linia.length() > 0) if (linia[0] == '\r') linia.erase(0, 1);
        if (linia.length() > 0) if (linia[linia.length() - 1] == '\r')
            linia.erase(linia.length() - 1, 1);
        if ((int) linia.length() < grid_w) {
            Log::criticalError("Nieprawidlowy format pliku mapy: " + gridfile);
            delete[] grid;
            plik.close();
            return false;
        }
        grid[w] = new char[grid_w];
        for (int k = 0; k < grid_w; k++) {
            grid[w][k] = linia[k];
        }
    }
    plik.close();
    return true;
}

char Map::get_grid(int x, int y) {
    char defau = '0'; //brak mapy
    if (x < 0) return defau;
    if (x >= grid_w) return defau;
    if (y < 0) return defau;
    if (y >= grid_h) return defau;
    return grid[y][x];
}
