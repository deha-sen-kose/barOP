#ifndef TRUSSSYSTEM_H
#define TRUSSSYSTEM_H

#include "barOP/material.h"
#include "trussElement.h"
#include "node.h"
#include <map>
#include <memory>
#include <vector>

class TrussStructure{

private:

    std::vector<std::unique_ptr<Node>> _nodes;
    std::vector<std::unique_ptr<TrussElement>> _elements;
    std::vector<Material> _materials;

    std::map<int, bool> _boundaryConditions;
    std::map<int, double> _forces;


public:

    TrussStructure() = default;

    ~TrussStructure() = default;

    Node& addNode(double x, double y, double z);

    Material& addMaterial(std::string matName, double E);

    TrussElement& addTrussElement(Node& n1, Node& n2, Material& mat, double A);

    void addBCs(std::vector<int> dof);

    void addForces(std::vector<int> dof, std::vector<double> forces);

    const std::vector<std::unique_ptr<Node>>& getNodes() const;
    const std::vector<std::unique_ptr<TrussElement>>& getElements() const;
    const std::vector<Material>& getMaterials() const;
    const std::map<int, bool>& getConditions() const;
    const std::map<int, double>& getForces() const;

    // Assemble Stiffness Matrices
    Matrix<double> assembleStffMtx() const;

    // Apply Boundary conditions
    void applyHomBCs(Matrix<double>& globStffMtx, std::vector<double>& forceVec) const;

    // Create force vector
    std::vector<double> createForceVector() const;

    // Solve truss system
    std::vector<double> solveTrussSystem(Matrix<double>& K_red, std::vector<double> F_red) const;

    // Get the full displacement vector for easier indexing
    std::vector<double> returnDispVector(std::vector<double>& u_red) const;

    // Compute the element strains
    std::vector<double> computeStrains(std::vector<double>& u) const;

    // Compute the element stresses
    std::vector<double> computeStresses(std::vector<double>& u) const;
};

#endif
