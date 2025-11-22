#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

class Material{

private:
    std::string _matName;
    double _E;

public:
    Material(std::string mat, double E) : _matName(mat), _E(E) {};

    double getE() const{return _E;};

    std::string getName() const {return _matName;};

};
#endif
