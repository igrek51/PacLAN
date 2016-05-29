#include "node.h"

Node::Node(int x, int y){
    this->x = x;
    this->y = y;
    g = 0;
    f = 0;
    parent = nullptr;
}