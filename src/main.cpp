#include "../include/barOP/barMesh.h"
#include <cstddef>
#include <iostream>
#include <vector>
#include "../include/math/Matrix.h"

int main() {

    /*
	std::vector<std::vector<float>> nodesx = {
		{0.f,0.f},
		{1.f,1.f}
	};

	std::vector<std::vector<int>> elemx = { {1,2} };

	Bar3DMesh msh (1, 2, nodesx, elemx);

	std::cout << msh.getNumEl() << '\n';

	std::vector<std::vector<float>> newNode = { { 2.f,2.f } };
	msh.addNodes(newNode);

	std::vector<std::vector<float>> nodezz = msh.getNodes();

	for (int i = 0; i < nodezz.size(); i++) {
		for (int j = 0; j < nodezz[i].size(); j++) {

			std::cout << nodezz[i][j] << " ";

		}

		std::cout << '\n';
	}

	for (int b = 0; b < nodezz[1].size(); b++) {

		std::cout << nodezz[msh.getNumNode()-1][b] << " ";
	}

	std::cout << msh.getNumEl() << " " << msh.getNumNode() << '\n';

	std::vector<int> nodeIDs = {2};
	msh.deleteNodes(nodeIDs);

	std::cout << '\n';

	for (int i = 0; i < msh.getNodes().size(); i++) {
		for (int j = 0; j < msh.getNodes()[0].size(); j++) {

			std::cout << msh.getNodes()[i][j] << " ";

		}

		std::cout << '\n';
	}
	*/



	// MATRIX LIBRARY
	//

	Matrix<float> M(5,5);

	for (size_t i = 0; i < 5 ; ++i){

	    M(i,0) = i + 1.0;
		M(i,1) = i + 1.0;
		M(i,2) = i + 1.0;
		M(i,3) = i + 1.0;
		M(i,4) = i + 1.0;

	}

	//M.printOut();

	//std::cout << M(2,2);

	//std::vector<size_t> r = {2,3};
	//M.deleteRows(r);

	//M.printOut();
	//std::cout << M.getSize()[0] << " " << M.getSize()[1];
	std::cout << "Allocations before any matrix: "
              << Matrix<int>::allocations << std::endl;  // should be 0

    {
        Matrix<int> A(2,3);
        Matrix<int> B(3,3);

        std::cout << "Allocations after creating A and B: "
                  << Matrix<int>::allocations << std::endl; // should be 5 (2 + 3 rows)


	Matrix<float> M2(5,5,1.f);

    Matrix<float> M3 = M + M2;
    M3.printOut();

    Matrix<float> M4 = M-M2;
    M4.printOut();

    Matrix<float> M5 = M*M2;
    M5.printOut();

    Matrix<int> M6(2,2);
    M6(0,0) = 1;
    M6(0,1) = 2;
    M6(1,0) = 3;
    M6(1,1) = 4;

    M6.printOut();

    std::vector<int> vec1= {5,6};

    std::vector<int> result = M6.mVm(vec1);

    for(size_t i = 0; i < vec1.size(); ++i){

      std::cout << vec1[i] << " " ;

    };

    for(size_t i = 0; i < result.size(); ++i){

      std::cout << result[i] << " " ;

    };

    }

    // A and B go out of scope, destructor called
    std::cout << "Allocations after A and B destroyed: "
              << Matrix<int>::allocations << std::endl; // should be 0

	return EXIT_SUCCESS;

}
