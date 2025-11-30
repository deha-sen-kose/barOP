#ifndef ELEMENT_H
#define ELEMENT_H

#include "material.h"
#include "../math/Matrix.h"

/**
 * Base Element class from which different types of elements inherit.
 * Each element carries its ID and material property.
 */
class Element{

protected:
    /**
    * int ID of the element, starting from 1
    */
    int _id;
    /**
    * double Young's Modulus of the element
    */
    Material& _Material;

public:
    /**
     * Element class constructor
     * @param id ID of the element
     * @param M An instance of the Material class
     * @see Material
     */
    Element(int id, Material& M) : _id(id), _Material(M){};

    /**
     * Pure virtual destructor
     */
    virtual ~Element() = default;

    /**
     * Pure virtual function that computes element stiffness matrix in global coordinates
     * @return Matrix class instance, element stiffness matrix in global coordinates
     * @see Matrix
     */
    virtual Matrix<double> computeGlobalStiffnessMtx() const = 0;

    /**
     * Pure virtual function to find the degrees of freedom of an element
     * @return std::vector<int> of degrees of freedom
     */
    virtual std::vector<int> getDOF() const = 0;

    /**
     * Function for finding the ID of an Element instance
     * @return ID of the element instance
     */
    int getID() const {return _id;};

    /**
     * Function for finding the Material class instance linked to an Element instance
     * @return Material class instance
     * @see Material
     */
    Material& getMaterial() const {return _Material;};
};
#endif
