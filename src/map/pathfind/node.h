#ifndef NODE_H
#define NODE_H

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
    Node* parent;
};

#endif
