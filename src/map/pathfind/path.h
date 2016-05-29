#ifndef PATH_H
#define PATH_H

#include <vector>

using namespace std;

/**
 * \brief Ścieżka - zbiór kolejnych punktów prowadzących z jednego do drugiego miejsca na mapie
 */
class Path {
public:
    ///utworzenie nowej ścieżki
    Path();

    ///usunięcie ścieżki
    ~Path();

    /// lista kolejnych punktów od startu do mety (włącznie) jako tablica int[2]: 0 - x, 1 - y
    vector<int*> points;

    /**
     * obliczenie długości ścieżki
     * @return długość ścieżki w jednostkach mapy
     */
    int length();
};


#endif
