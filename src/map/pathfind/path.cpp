#include "path.h"

Path::Path() {
}

Path::~Path() {
    for (unsigned int i = 0; i < points.size(); i++) {
        delete[] points.at(i);
    }
}

int Path::length() {
    if (points.size() <= 1)
        return 0;
    return points.size() - 1;
}