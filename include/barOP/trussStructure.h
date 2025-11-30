#ifndef TRUSSSYSTEM_H
#define TRUSSSYSTEM_H

#include "material.h"
#include "trussElement.h"
#include "node.h"
#include <map>
#include <memory>
#include <vector>

/**
 * Class that handles a truss system
 * Holds unique pointers to truss elements and nodes
 * Initializes nodes and elements. Computes necessary values
 */
class TrussStructure{

private:

    /**
     * Private member variable
     * A vector that contains unique pointers to nodes
     */
    std::vector<std::unique_ptr<Node>> _nodes;

    /**
     * Private member variable
     * A vector that contains unique pointers to elements
     */
    std::vector<std::unique_ptr<TrussElement>> _elements;

    /**
     * Private member variable
     * A vector that contains materials
     */
    std::vector<Material> _materials;

    /**
     * Private member variable
     * A map that connects degrees of freedom to boolean type boundary conditions (fixed)
     */
    std::map<int, bool> _boundaryConditions;

    /**
     * Private member variable
     * A map that connects degrees of freedom to forces applied
     */
    std::map<int, double> _forces;


public:

    /**
     * Default class constructor
     */
    TrussStructure() = default;

    /**
     * Default class destructor
     */
    ~TrussStructure() = default;

    /**
     * Member function that adds a node to a TrussStructure instance
     * Node ids are generated inside the function, no manual ids are passed
     * @param x x-coordinate of the added node
     * @param y y-coordinate of the added node
     * @param z z-coordinate of the added node
     * @return Reference to a Node instance
     * @see Node
     */
    Node& addNode(double x, double y, double z);

    /**
     * Member function that creates a new material
     * Passes the name of the material and its Young's modulus
     * @param matName The name of the material, can be anything
     * @param E Young's modulus of the material
     * @return Reference to a Material instance
     * @see Material
     */
    Material& addMaterial(std::string matName, double E);

    /**
     * Member function that adds an element to an existing TrussStructure instance
     * Automatically gives ids to the elements, no manual argument passing
     * @param n1 Reference to the first node a truss has
     * @param n2 Reference to the second node a truss has
     * @param mat Material class instance that the truss is made out of
     * @param A Cross section area of the truss
     * @return Reference to a TrussElement instance
     * @see TrussElement
     * @see Material
     * @see Node
     */
    TrussElement& addTrussElement(Node& n1, Node& n2, Material& mat, double A);

    /**
     * Member function that adds a mapping for boundary conditions
     * Makes the given vector of degrees of freedom fixed
     * @param dof Vector of degrees of freedom desired to be fixed
     */
    void addBCs(std::vector<int> dof);

    /**
     * Member function that adds forces to the dof-force map
     * @param dof Vector that contains degree of freedom - must be same size with forces
     * @param forces Vector that constains forces applied to those degree of freedom - must be same size with dof
     */
    void addForces(std::vector<int> dof, std::vector<double> forces);

    /**
     * Member function that returns nodes of a truss system
     * @return A vector of pointers to the nodes of a truss system
     * @see Node
     */
    const std::vector<std::unique_ptr<Node>>& getNodes() const;

    /**
     * Member function that returns elements of a truss system
     * @return A vector of pointers to the elements of a truss system
     * @see TrussElement
     */
    const std::vector<std::unique_ptr<TrussElement>>& getElements() const;

    /**
     * Member function that returns materials of a truss system
     * @return A vector of the materials of a truss system
     * @see Material
     */
    const std::vector<Material>& getMaterials() const;

    /**
     * Member function that returns boundary condition map of a truss system
     * @return A map of dof-fixed boundary conditions of a truss system
     */
    const std::map<int, bool>& getConditions() const;

    /**
     * Member function that returns force map of a truss system
     * @return A map of dof-forces of a truss system
     */
    const std::map<int, double>& getForces() const;

    /**
     * Member function that assembles the stiffness matrices
     * The individual element matrices are computed inside, thus no input arguments
     * @return Global master stiffness matrix
     * @see Matrix
     */
    Matrix<double> assembleStffMtx() const;

    /**
     * Member function that handles homogeneous boundary conditions
     * Deletes the rows and columns of master stiffness matrix
     * Deletes the rows of master force vector
     * @param globStffMtx Master stiffness matrix
     * @param forceVec Master force vector
     * @see Matrix
     */
    void applyHomBCs(Matrix<double>& globStffMtx, std::vector<double>& forceVec) const;

    /**
     * Member function for computing master force vector
     * @return Master force vector
     */
    std::vector<double> createForceVector() const;

    /**
     * Member function that solves the LSE
     * Uses complete Cholesky algoritm to solve the system
     * @param K_red Reduced master stiffness matrix
     * @param F_red Reduced master force vector
     * @return Reduced displacement vector
     * @see Matrix
     */
    std::vector<double> solveTrussSystem(Matrix<double>& K_red, std::vector<double> F_red) const;

    /**
     * Member function that computes complete displacement vector
     * Adds zeros to the places where dof are fixed
     * @param u_red Reduced displacement vector
     * @return Complete displacement vector solution
     */
    std::vector<double> returnDispVector(std::vector<double>& u_red) const;

    /**
     * Member function that computes the engineering strains
     * Uses complete displacement vector to compute engineering strains of each truss element
     * @param u Complete displacement solution
     * @return Engineering strains
     */
    std::vector<double> computeStrains(std::vector<double>& u) const;

    /**
     * Member function that computes the element stresses
     * Uses complete displacement vector
     * @param u Complete displacement vector solution
     * @return Stresses in each truss element
     */
    std::vector<double> computeStresses(std::vector<double>& u) const;
};
#endif
