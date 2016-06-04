/**
 * \file node.h
 * \see node.cpp
 */
#ifndef NODE_H
#define NODE_H

/**
 * @brief Węzeł w labiryncie w algorytmie znajdowania drogi
 * @tparam T typ współrzędnych punktu
 */
template<typename T>
class Node {
public:
    /**
     * utworzenie nowego węzła
     * @param x położenie na mapie wzdłuż osi x
     * @param y położenie na mapie wzdłuż osi y
     */
    Node(T x, T y);

    ~Node();

    /// położenie na mapie wzdłuż osi x
    T x;
    /// położenie na mapie wzdłuż osi y
    T y;
    /// odległość od startu po wyznaczonej drodze
    T g;
    /// suma g+h (heurystyki) - całkowita oszacowana droga od startu do mety
    T f;
    /// wskaźnik na węzeł będący rodzicem (lub NULL)
    Node<T>* parent;
};

#endif
