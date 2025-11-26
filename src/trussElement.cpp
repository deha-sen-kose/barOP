#include "../include/barOP/trussElement.h"
#include <vector>

TrussElement::TrussElement(int id, Node& node1, Node& node2, Material& Mat, double A) :
    Element(id, Mat), _node1(node1),
    _node2(node2), _A(A){};

std::vector<int> TrussElement::getDOF() const{

      int dof1 = 3*(_node1.getID())-2; // x
      int dof2 = 3*(_node1.getID())-1; // y
      int dof3 = 3*(_node1.getID());   // z


      int dof4 = 3*(_node2.getID())-2; // x
      int dof5 = 3*(_node2.getID())-1; // y
      int dof6 = 3*(_node2.getID());   // z

      std::vector<int> DOF = {dof1,dof2,dof3,dof4,dof5,dof6};

      return DOF;
  };

Node& TrussElement::getNode1() const {

    return _node1;
};

Node& TrussElement::getNode2() const {

    return _node2;
};


Matrix<double> TrussElement::computeGlobalStiffnessMtx() const{

      std::vector<double> pos1 = _node1.getPosition();
      std::vector<double> pos2 = _node2.getPosition();

      double x1 = pos1[0];
      double y1 = pos1[1];
      double z1 = pos1[2];

      double x2 = pos2[0];
      double y2 = pos2[1];
      double z2 = pos2[2];

      double L = std::sqrt(std::pow(x2-x1,2) + std::pow(y2-y1,2) + std::pow(z2-z1,2));

      double cx = (x2-x1)/L;
      double cy = (y2-y1)/L;
      double cz = (z2-z1)/L;

      Matrix<double> elLocalStffMtx = {{1, -1},
                                       {-1, 1}};

      double scalar = _Material.getE() * _A / L;

      elLocalStffMtx = elLocalStffMtx*scalar;

      Matrix<double> transformationMtx = {{cx, cy, cz, 0, 0, 0},
                                          {0, 0, 0, cx, cy, cz}};

    Matrix<double> elGlobalStffMtx = transformationMtx.transpose()*elLocalStffMtx*transformationMtx;


    return elGlobalStffMtx;
};
