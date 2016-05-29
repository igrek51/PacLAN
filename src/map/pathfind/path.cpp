#include "path.h"

template<typename T>
Path<T>::Path() {
}

template<typename T>
Path<T>::~Path() {
    for (auto it = points.begin(); it != points.end(); it++) {
        delete *it;
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
