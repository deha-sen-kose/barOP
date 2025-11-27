#ifndef TRUSSELEMENT_H
#define TRUSSELEMENT_H

#include "math/Matrix.h"
#include "node.h"
#include "element.h"
#include <vector>

class TrussElement : public Element{

private:
  Node& _node1;
  Node& _node2;
  double _A;

public:

  TrussElement(int id, Node& node1, Node& node2, Material& Mat, double A);

  Node& getNode1() const;

  Node& getNode2() const;

  double computeLength() const;

  Matrix<double> computeTransformation() const;

  std::vector<int> getDOF() const override;

  Matrix<double> computeGlobalStiffnessMtx() const override;

  double computeElStrain(std::vector<double>& u) const;

  double computeElStress(std::vector<double>& u) const;
};



#endif
