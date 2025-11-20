#ifndef BARMESH_H
#define BARMESH_H

#include "../include/math/Matrix.h"
#include <vector>

class barMesh {

private:

	size_t _numEl;
	size_t _numNode;
	Matrix<double> _nodes;
	Matrix<size_t> _elements;

public:

    barMesh();

	barMesh(size_t numEl, size_t numNode, Matrix<double>& nodes,
		      Matrix<size_t>& elements);

	~barMesh();

	const size_t getNumEl();
	const size_t getNumNode();
	const Matrix<double> getNodes();
	const Matrix<size_t> getElems();

	const std::vector<size_t> findElems(std::vector<size_t>& nodeIDs) ;
	const std::vector<size_t> findNodes(size_t elemID);

	void addNodes(Matrix<double> & coords);
	void deleteNodes(std::vector<int> & nodeIDs);

	void connectNodes(std::vector<int> & nodeIDs);
	void deleteElems(std::vector<int> & elemIDs);

};

#endif
