#ifndef NODE_H
#define NODE_H

#include <vector>

/**
 * Node class
 * Points in 3D. Elements carry the references of/pointers to node class variables
 */
class Node{

private:

    /**
     * Holds the element ID, which starts from 1
     */
    int _id;

    /**
     * x-coordinate of the node
     */
    double _x;

    /**
     * y-coordinate of the node
     */
    double _y;

    /**
     * z-coordinate of the node
     */
    double _z;

public:

    /**
     * Contructor for Node class
     * @param id ID of the node, starting from 1
     * @param x x-coordinate of the node
     * @param y y-coordinate of the node
     * @param z z-coordinate of the node
     */
    Node(int id, double x, double y, double z): _id(id), _x(x), _y(y), _z(z) {};

    /**
     * Member function for finding ID of a class Node instance
     * @return int ID of a class Node instance
     */
    int getID() const{ return _id;};

    /**
     * Member function for finding node coordinates
     * @return {x,y,z} coordinates of a node
     */
    std::vector<double> getPosition() const {

        std::vector<double> pos = {_x,_y,_z};
        return pos;
    };

    /**
     * Member function that updates the position of a node
     * Used for shape update, new position must be entered
     * @param newX The new x-coordinate
     * @param newY The new y-coordinate
     * @param newZ The new z-coordinate
     */
    void updatePosition(double newX, double newY, double newZ) {

        _x = newX;
        _y = newY;
        _z = newZ;
    };

    /**
     * Member function that translates a node by given amounts
     * Used for shape update, the translation values (deltas) must be given
     * @param deltaX The wanted translation in x-direction
     * @param deltaY The wanted translation in y-direction
     * @param deltaZ The wanted translation in z-direction
     */
    void moveNode(double deltaX, double deltaY, double deltaZ){

        _x += deltaX;
        _y += deltaY;
        _z += deltaZ;
    };
};
#endif
