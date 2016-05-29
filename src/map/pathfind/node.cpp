#include "node.h"

template<typename T>
Node<T>::Node(T x, T y) {
    this->x = x;
    this->y = y;
    g = 0;
    f = 0;
    parent = nullptr;
}

template<typename T>
Node<T>::~Node() {
}


template
class Node<int>;
