#include "path.h"

template<typename T>
Path<T>::Path() {
}

template<typename T>
Path<T>::~Path() {
    for (unsigned int i = 0; i < points.size(); i++) {
        delete[] points.at(i);
    }
}

template<typename T>
T Path<T>::length() {
    if (points.size() <= 1)
        return 0;
    return points.size() - 1;
}


template
class Path<int>;
