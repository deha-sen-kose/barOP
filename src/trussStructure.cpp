#include "../include/barOP/trussStructure.h"
#include "math/Matrix.h"
#include <functional>
#include <stdexcept>
#include <vector>

// Getters
const std::vector<std::unique_ptr<Node>>& TrussStructure::getNodes() const { return _nodes; };
const std::vector<std::unique_ptr<TrussElement>>& TrussStructure::getElements() const { return _elements; };
const std::vector<Material>& TrussStructure::getMaterials() const {return _materials;};
const std::map<int, bool>& TrussStructure::getConditions() const {return _boundaryConditions;};
const std::map<int, double>& TrussStructure::getForces() const {return _forces;};

// ------- Nodes -------
Node& TrussStructure::addNode(double x, double y, double z) {

    double eps = 1E-1;
    for (size_t i = 0; i < _nodes.size(); ++i){
        for (size_t j = 0; j < 3; ++j){
            if (std::abs(_nodes[i]->getPosition()[0] - x) < eps &&
                std::abs(_nodes[i]->getPosition()[1] - y) < eps &&
                std::abs(_nodes[i]->getPosition()[2] - z) < eps){

                 throw std::invalid_argument("There exists a node in given location! (TrussStructure::addNode e<1E-1)");
                };
        };
    };

    int id;
    if (_nodes.empty()){
        id = 1;
    }
    else {
        id = _nodes.size() + 1;
    }

    //std::cout << '\n' <<"id: " << id << std::endl;

     // create in place and add directly
    _nodes.push_back(std::make_unique<Node>(id,x,y,z));
    return static_cast<Node&>(*_nodes.back());
};

// ------- Materials -------
Material& TrussStructure::addMaterial(std::string matName, double E)
{
    for (size_t i = 0; i < _materials.size(); ++i){
        if (_materials[i].getName() == matName){
            throw std::invalid_argument("Given Material Already Exists! (TrussStructure::addMaterial)");
        };
    };
    _materials.emplace_back(matName, E);
    return _materials.back();
};

// ------- Elements -------
TrussElement& TrussStructure::addTrussElement(Node& n1, Node& n2, Material& mat, double A) {

    int id = _elements.size() + 1;
    _elements.push_back(std::make_unique<TrussElement>(id, n1, n2, mat, A));
    return static_cast<TrussElement&>(*_elements.back());
};

// Add boundary conditions
void TrussStructure::addBCs(std::vector<int> dof) {

    for (size_t i = 0; i < dof.size(); ++i){

        _boundaryConditions.insert({dof[i],true});
    };
};

// Add forces
void TrussStructure::addForces(std::vector<int> dof, std::vector<double> forces){

    if (dof.size() != forces.size()){
        throw std::invalid_argument("Given DOF and forces have different sizes! (TrussStructure::addForces)");};

    for(size_t i = 0; i < dof.size(); ++i){

        _forces.insert({dof[i],forces[i]});
    };
};

// Assemble stiffness matrices
Matrix<double> TrussStructure::assembleStffMtx() const{

      size_t numNodes = _nodes.size();
      size_t numEl = _elements.size();
      size_t numDOF = numNodes*3;

      // std::cout << '\n' << "numDOF: " << numDOF << std::endl;

      Matrix<double> globalStffMtx(numDOF,numDOF,0.0);

      for (size_t i = 0; i < numEl; ++i){
          Matrix<double> elStffMtx = _elements[i]->computeGlobalStiffnessMtx();
          std::vector<int> DOFs = _elements[i]->getDOF();

          for (size_t j = 0 ; j < 6 ; ++j){
              for (size_t k = 0; k < 6; ++k){

                  globalStffMtx(DOFs[j]-1,DOFs[k]-1) = globalStffMtx(DOFs[j]-1,DOFs[k]-1) + elStffMtx(j,k);

              };
          };
      };
      return globalStffMtx;
};

// Create force vector
std::vector<double> TrussStructure::createForceVector() const{

    int numNode = _nodes.size();
    int numDOF = numNode*3;
    std::vector<double> forceVec(numDOF, 0.0);
    for (auto& f : _forces){

        forceVec[f.first-1] = f.second;

    };
    return forceVec;
};

// Apply boundary conditions
void TrussStructure::applyHomBCs(Matrix<double>& globStffMtx, std::vector<double>& forceVec) const{

    std::vector<size_t> homDOF;
    for (const auto& bc : _boundaryConditions){
        if (bc.second){
            homDOF.push_back(bc.first);
        };
    };

    std::sort(homDOF.begin(),homDOF.end(),std::greater<int>());

        for (size_t i = 0; i < homDOF.size(); i++){
            globStffMtx.deleteRow(homDOF[i]-1);
            globStffMtx.deleteColumn(homDOF[i]-1);

            forceVec.erase(forceVec.begin() + homDOF[i]-1);
        };
};

// Solve truss system
std::vector<double> TrussStructure::solveTrussSystem(Matrix<double>& K_red, std::vector<double> F_red) const{

    Matrix<double> L = K_red.cho();

    Matrix<double> L_inverse = L.L_inverse();

    std::vector<double> u = L_inverse.transpose().mVm(L_inverse.mVm(F_red));
    return u;
};

std::vector<double> TrussStructure::returnDispVector(std::vector<double>& u_red) const{

  int numNode = _nodes.size();
  int numDOF = numNode*3;

  std::vector<double> u(numDOF,0.0);

  int counter = 0;
  for (int i = 0; i < numDOF; ++i){
      if (_boundaryConditions.find(i+1) == _boundaryConditions.end()){

        // if key does not exist
        u[i] = u_red[counter];
        counter++;
      };
  };
  return u;
};

std::vector<double> TrussStructure::computeStrains(std::vector<double>& u) const{

    int numEl = _elements.size();
    std::vector<double> strains(numEl, 0.0);

    for (int i = 0; i < numEl; ++i){

        strains[i] = _elements[i]->computeElStrain(u);

    };

    return strains;
};

std::vector<double> TrussStructure::computeStresses(std::vector<double>& u) const{

    int numEl = _elements.size();
    std::vector<double> stresses(numEl, 0.0);

    for (int i = 0; i < numEl; ++i){

        stresses[i] = _elements[i]->computeElStress(u);

    };
    return stresses;
};
