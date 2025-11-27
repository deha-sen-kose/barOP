#include "../include/barOP/trussStructure.h"
#include "../include/barOP/material.h"
#include "../include/barOP/trussElement.h"
#include "../include/visualization/trussVis.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ostream>
#include <vector>

int main(){

    #ifdef NDEBUG
        std::cout << "Release build" << std::endl;
    #else
        std::cout << "Debug build" << std::endl;
    #endif

    TrussStructure struct1;

    Node& n1 = struct1.addNode(0, 0, 0);
    Node& n2 = struct1.addNode(40, 0, 0);
    Node& n3 = struct1.addNode(40, 30, 0);
    Node& n4 = struct1.addNode(0, 30, 0);



    double E = 29.5E6;
    Material& mat = struct1.addMaterial("E", E);
    double A = 1.0;
    TrussElement& el1 = struct1.addTrussElement(n1, n2, mat, A);
    TrussElement& el2 = struct1.addTrussElement(n3, n2, mat, A);
    TrussElement& el3 = struct1.addTrussElement(n1, n3, mat, A);
    TrussElement& el4 = struct1.addTrussElement(n4, n3, mat, A);



    std::vector<int> homDOF = {1,2,3,5,6,9,10,11,12};
    struct1.addBCs(homDOF);
    std::vector<int> forceDOF = {4,8};
    std::vector<double> forces = {20000,-25000};
    struct1.addForces(forceDOF, forces);

    Matrix<double> K = struct1.assembleStffMtx();

    std::vector<double> F = struct1.createForceVector();

    struct1.applyHomBCs(K, F);


    K.printOut();

    std::cout << '\n';
    for (size_t i = 0; i < F.size(); ++i){

        std::cout << F[i] << " ";

    };
    std::cout << std::endl;

    std::vector<double> u_red = struct1.solveTrussSystem(K, F);


    for (size_t i = 0; i < u_red.size(); ++i){

     std::cout << u_red[i] << " ";

   };

    std::cout << std::endl;

    std::vector<double> u = struct1.returnDispVector(u_red);

    for (size_t i = 0; i < u.size(); ++i){

     std::cout << u[i] << " ";

   };

    std::cout << std::endl;

    std::vector<double> sigma = struct1.computeStresses(u);

    for (size_t i = 0; i < sigma.size(); ++i){

     std::cout << sigma[i] << " ";

   };

    std::cout << std::endl;

    TrussVisualization vis(struct1);
    vis.addFixedJoints(struct1);
    vis.updateScalarField(struct1, u, "stress");
    vis.start();


    return EXIT_SUCCESS;
}
