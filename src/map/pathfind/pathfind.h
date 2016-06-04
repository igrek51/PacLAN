/**
 * \file pathfind.h
 * \see pathfind.cpp
 */
#ifndef PATHFIND_H
#define PATHFIND_H

#include <deque>
#include "path.h"
#include "node.h"

using namespace std;

/**
 * @brief Klasa przechowująca obecne parametry mapy i znajdująca drogę pomiędzy dwoma punktami
 * @tparam T typ współrzędnych punktu
 */
template<typename T>
class Pathfind {
public:
    /// inicjalizacja algorytmu znajdowania drogi
    Pathfind();

    ~Pathfind();

    /// tablica mapy, zapis: map[wiersz][kolumna], map[y][x], 1 - można przejść, 0 - nie można
    char** map;
    /// liczba kolumn mapy
    T map_w;
    /// liczba wierszy mapy
    T map_h;

    /**
     * ustawia punkt startowy i punkt docelowy
     * @param start_x punkt startowy - współrzędna x
     * @param start_y punkt startowy - współrzędna y
     * @param end_x punkt końcowy - współrzędna x
     * @param end_y punkt końcowy - współrzędna y
     */
    void set_xy(T start_x, T start_y, T end_x, T end_y);

    /**
     * szuka drogi między startem a metą
     * @return wskaźnik do wyznaczonej ścieżki lub NULL w przypadku, gdy nie można bylo znaleźć drogi
     */
    Path<T>* find_path();

private:
    ///współrzędna x punktu startowego
    T start_x;
    ///współrzędna y punktu startowego
    T start_y;
    ///współrzędna x punktu końcowego
    T end_x;
    ///współrzędna y punktu końcowego
    T end_y;

    /**
     * szuka węzła na liście
     * @param list lista , która ma być przeszukana
     * @param x współrzędna x punktu
     * @param y współrzędna y punktu
     * @return wskaźnik na węzeł (jeśli znajdował się na liście) lub NULL (jeśli nie znaleziono takiego punktu)
     */
    Node<T>* find_in_list(deque<Node<T>*> list, T x, T y);

    /**
     * liczy drogę do punktu startowego (dokładnie)
     * @param item wskaźnik na węzeł, dla którego ma być policzona droga
     * @return długość drogi do punktu startowego
     */
    T policz_g(Node<T>* item);

    /**
     * oszacowanie drogi do punktu końcowego poprzez funkcję heurystyczną
     * @param item wskaźnik na węzeł, dla którego ma być policzona droga
     * @return przybliżona długość drogi do punktu końcowego
     */
    T policz_h(Node<T>* item);
};

/**
 * @tparam T typ liczby
 * @param num liczba
 * @return wartość bezwzględna z liczby
 */
template<typename T>
T absolute(T num) {
    return num >= 0 ? num : -num;
}

#endif
