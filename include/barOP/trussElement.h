#ifndef TRUSSELEMENT_H
#define TRUSSELEMENT_H

#include "../math/Matrix.h"
#include "node.h"
#include "element.h"
#include <vector>

/**
 * Derived TrussElement class from base class Element
 * Holds the references to two nodes a truss element has
 * Contains the cross section area of the element
 * @see Element
 */
class TrussElement : public Element{

private:

    /**
     * Reference to the first node of a truss element
     */
     Node& _node1;

     /**
      * Reference to the second node of a truss element
      */
     Node& _node2;

     /**
      * Cross section area of a truss element
      */
     double _A;

public:

    /**
     * Constructor for derived-class TrussElement
     * @param id The ID of the truss element, starting from 1
     * @param node1 A reference to the first node that a truss element has
     * @param node2 A reference to the second node that a truss element has
     * @param Mat Material of the element passes as an instance of Material class
     * @param A Cross section area of the truss element
     * @see Material
     */
    TrussElement(int id, Node& node1, Node& node2, Material& Mat, double A);

    /**
     * Member function to return the reference to the first node
     * @return Node class instance reference
     */
    Node& getNode1() const;

    /**
    * Member function to return the reference to the second node
    * @return Node class instance reference
    */
    Node& getNode2() const;

    /**
     * Member function that computes a truss elements length
     * @return Length of a truss element
     */
    double computeLength() const;

    /**
     * Member function that computes the 3D transformation matrix of a truss element
     * Used for transforming element matrices from local coord. to global coord.
     * @return Transformation matrix
     * @see Matrix
     */
    Matrix<double> computeTransformation() const;

    /**
     * Member function that returns the degrees of freedom of a truss element
     * @return Degrees of freedom of a truss element (u1,v1,w1,u2,v2,w2)
     */
    std::vector<int> getDOF() const override;

    /**
     * Member function that computes the truss element stiffness matrix
     * @return truss element stiffness matrix is in global coordinates
     */
    Matrix<double> computeGlobalStiffnessMtx() const override;

    /**
     * Member function for computing engineering strains
     * @return Engineering straing of a deformed truss element
     */
    double computeElStrain(std::vector<double>& u) const;

    /**
     * Member function for computing stresses
     * @return Stress ocurred in a deformed truss element
     */
    double computeElStress(std::vector<double>& u) const;
};
#endif
