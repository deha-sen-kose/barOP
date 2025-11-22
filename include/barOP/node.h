#ifndef NODE_H
#define NODE_H

#include <vector>

class Node{

private:
    int _id;
    double _x,_y,_z;

public:
    Node(int id, double x, double y, double z): _id(id), _x(x), _y(y), _z(z) {};

    int getID() const{ return _id;};

    std::vector<double> getPosition() const {

        std::vector<double> pos = {_x,_y,_z};
        return pos;
    };

    void updatePosition(double newX, double newY, double newZ) {

        _x = newX;
        _y = newY;
        _z = newZ;
    };

    void moveNode(double deltaX, double deltaY, double deltaZ){

        _x += deltaX;
        _y += deltaY;
        _z += deltaZ;
    };

    };

#endif
