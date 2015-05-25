/**
 * \file pathfind.h
 * Moduł znajdowania drogi w labiryncie
 * \see pathfind.cpp
 */
#ifndef PATHFIND_H
#define PATHFIND_H

#include <vector>

using namespace std;

/**
 * \brief Węzeł w labiryncie w algorytmie znajdowania drogi
 */
class Node {
public:
    /**
     * utworzenie nowego węzła
     * @param x położenie na mapie wzdłuż osi x
     * @param y położenie na mapie wzdłuż osi y
     */
    Node(int x, int y);
    /// położenie na mapie wzdłuż osi x
    int x;
    /// położenie na mapie wzdłuż osi y
    int y;
    /// odległość od startu po wyznaczonej drodze
    int g;
    /// suma g+h (heurystyki) - całkowita oszacowana droga od startu do mety
    int f;
    /// wskaźnik na węzeł będący rodzicem (lub NULL)
    Node *parent;
};

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

/**
 * \brief Klasa przechowująca obecne parametry mapy i znajdująca drogę pomiędzy dwoma punktami
 */
class Pathfind {
public:
    /// inicjalizacja algorytmu znajdowania drogi
    Pathfind();
    ~Pathfind();
    /// tablica mapy, zapis: map[wiersz][kolumna], map[y][x], 1 - można przejść, 0 - nie można
    char **map;
    /// liczba kolumn mapy
    int map_w;
    /// liczba wierszy mapy
    int map_h;
    /**
     * ustawia punkt startowy i punkt docelowy
     * @param start_x punkt startowy - współrzędna x
     * @param start_y punkt startowy - współrzędna y
     * @param end_x punkt końcowy - współrzędna x
     * @param end_y punkt końcowy - współrzędna y
     */
    void set_xy(int start_x, int start_y, int end_x, int end_y);
    /**
     * szuka drogi między startem a metą
     * @return wskaźnik do wyznaczonej ścieżki lub NULL w przypadku, gdy nie można bylo znaleźć drogi
     */
    Path* find_path();
private:
    ///współrzędna x punktu startowego
    int start_x;
    ///współrzędna y punktu startowego
    int start_y;
    ///współrzędna x punktu końcowego
    int end_x;
    ///współrzędna y punktu końcowego
    int end_y;
    /**
     * szuka węzła na liście
     * @param list lista , która ma być przeszukana
     * @param x współrzędna x punktu
     * @param y współrzędna y punktu
     * @return wskaźnik na węzeł (jeśli znajdował się na liście) lub NULL (jeśli nie znaleziono takiego punktu)
     */
    Node* find_in_list(vector<Node*> list, int x, int y);
    /**
     * liczy drogę do punktu startowego (dokładnie)
     * @param item wskaźnik na węzeł, dla którego ma być policzona droga
     * @return długość drogi do punktu startowego
     */
    int policz_g(Node *item);
    /**
     * oszacowanie drogi do punktu końcowego poprzez funkcję heurystyczną
     * @param item wskaźnik na węzeł, dla którego ma być policzona droga
     * @return przybliżona długość drogi do punktu końcowego
     */
    int policz_h(Node *item);
};

#endif
