#ifndef ELEMENT_H
#define ELEMENT_H

#include "material.h"
#include "../math/Matrix.h"

class Element{

protected:
    int _id;
    Material& _Material;

public:
    Element(int id, Material& M) : _id(id), _Material(M){};
    virtual ~Element() = default;

    virtual Matrix<double> computeGlobalStiffnessMtx() const = 0;
    virtual std::vector<int> getDOF() const = 0;

    int getID() const {return _id;};
    Material& getMaterial() const {return _Material;};

};
#endif
