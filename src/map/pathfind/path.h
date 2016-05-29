#ifndef PATH_H
#define PATH_H

#include <vector>

using namespace std;

/**
 * \brief Ścieżka - zbiór kolejnych punktów prowadzących z jednego do drugiego miejsca na mapie
 */
template<typename T>
class Path {
public:
    ///utworzenie nowej ścieżki
    Path();

    ///usunięcie ścieżki
    ~Path();

    //TODO wykorzystanie pair<int, int>

    /// lista kolejnych punktów od startu do mety (włącznie) jako tablica int[2]: 0 - x, 1 - y
    vector<T*> points;

    /**
     * obliczenie długości ścieżki
     * @return długość ścieżki w jednostkach mapy
     */
    T length();

    //TODO length i size
};

#endif
