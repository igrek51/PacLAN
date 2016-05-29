#ifndef PATH_H
#define PATH_H

#include <deque>

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

    /// lista kolejnych punktów od startu do mety (włącznie) - zestaw: x, y
    deque<pair<T, T>*> points;

    /**
     * obliczenie długości ścieżki
     * @return długość ścieżki w jednostkach mapy
     */
    T length();
};

#endif
