#include "../include/barOP/trussStructure.h"
#include "../include/visualization/trussVis.h"
#include <cstdio>
#include <cstdlib>
#include <vector>

int main(){

    TrussStructure t1;

    Node& n1 = t1.addNode(0.000000 ,0.000000 ,0.000000);
    Node& n2 = t1.addNode(10.000000, 5.000000, 0.000000);
    Node& n3 = t1.addNode(10.000000, 0.000000, 0.000000);
    Node& n4 = t1.addNode(20.000000, 8.000000, 0.000000);
    Node& n5 = t1.addNode(20.000000, 0.000000, 0.000000);
    Node& n6 = t1.addNode(30.000000, 9.000000, 0.000000);
    Node& n7 = t1.addNode(30.000000, 0.000000, 0.000000);
    Node& n8 = t1.addNode(40.000000, 8.000000, 0.000000);
    Node& n9 = t1.addNode(40.000000, 0.000000, 0.000000);
    Node& n10 = t1.addNode(50.000000, 5.000000, 0.000000);
    Node& n11 = t1.addNode(50.000000, 0.000000, 0.000000);
    Node& n12 = t1.addNode(60.000000, 0.000000, 0.000000);

    Material& elemMat = t1.addMaterial("mat1", 1000.00);
    TrussElement& e1 = t1.addTrussElement(n1, n3, elemMat, 2.00);
    TrussElement& e2 = t1.addTrussElement(n3, n5, elemMat, 2.00);
    TrussElement& e3 = t1.addTrussElement(n5, n7, elemMat, 2.00);
    TrussElement& e4 = t1.addTrussElement(n7, n9, elemMat, 2.00);
    TrussElement& e5 = t1.addTrussElement(n9, n11, elemMat, 2.00);
    TrussElement& e6 = t1.addTrussElement(n11, n12, elemMat, 2.00);
    TrussElement& e7 = t1.addTrussElement(n1, n2, elemMat, 10.00);
    TrussElement& e8 = t1.addTrussElement(n2, n4, elemMat, 10.00);
    TrussElement& e9 = t1.addTrussElement(n4, n6, elemMat, 10.00);
    TrussElement& e10 = t1.addTrussElement(n6, n8, elemMat, 10.00);
    TrussElement& e11 = t1.addTrussElement(n8, n10, elemMat, 10.00);
    TrussElement& e12 = t1.addTrussElement(n10, n12, elemMat, 10.00);
    TrussElement& e13 = t1.addTrussElement(n2, n3, elemMat, 3.00);
    TrussElement& e14 = t1.addTrussElement(n4, n5, elemMat,3.00);
    TrussElement& e15 = t1.addTrussElement(n6, n7, elemMat,3.00);
    TrussElement& e16 = t1.addTrussElement(n8, n9, elemMat, 3.00);
    TrussElement& e17 = t1.addTrussElement(n10, n11, elemMat, 3.00);
    TrussElement& e18 = t1.addTrussElement(n2, n5, elemMat, 1.00);
    TrussElement& e19 = t1.addTrussElement(n4, n7, elemMat, 1.00);
    TrussElement& e20 = t1.addTrussElement(n7, n8, elemMat, 1.00);
    TrussElement& e21 = t1.addTrussElement(n9, n10, elemMat, 1.00);

    std::vector<int> homdof = {1,2,3,6,9,12,15,18,21,24,27,30,33,35,36};
    t1.addBCs(homdof);
    std::vector<int> forceDof = {8,14,20,26,32};
    std::vector<double> forces = {-10, -10, -16, -10, -10};
    t1.addForces(forceDof, forces);

    std::vector<double> u = t1.solveTrussSystem();

    std::vector<double> sigma = t1.computeStresses(u);

    TrussVisualization vis(t1);
    vis.addFixedJoints(t1);
    vis.updateScalarField(t1, u, "stress");
    vis.start();


    return EXIT_SUCCESS;
}
