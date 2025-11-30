#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

/**
 * Material class
 * Saves a material name and its Young's Modulus in a class
 */
class Material{

private:

    /**
     * Member variable for material name
     */
    std::string _matName;

    /**
     * Young's modulus of the Material class instance
     */
    double _E;

public:

    /**
     * Constructor for Material class
     * @param mat String for naming the material, anything can be the name
     * @param E Young's Modulus
     */
    Material(std::string mat, double E) : _matName(mat), _E(E) {};

    /**
     * Member function to return Young's Modulus
     * @return Young's modulus
     */
    double getE() const{return _E;};

    /**
     * Returns the given material name
     * Mainly used for checking if one has entered same material for more than once
     * @return string material name
     */
    std::string getName() const {return _matName;};
};
#endif
