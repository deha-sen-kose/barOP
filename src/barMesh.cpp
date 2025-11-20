#include "../include/barOP/barMesh.h"
#include "math/Matrix.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <vector>

#define dim 3 // dimensionality
#define nnpe 2 // number of nodes per element

barMesh::barMesh() : _numEl(0), _numNode(0){

    Matrix<double> nodes(_numNode, dim, double{0});
    _nodes = nodes;
    //nodes.~Matrix();

    Matrix<size_t> elems(_numEl, nnpe, double{0});
    _elements = elems;
    //elems.~Matrix();

};

barMesh::barMesh(size_t numEl, size_t numNode, Matrix<double> & nodes,
	Matrix<size_t>& elements) : 	_numEl (numEl), _numNode (numNode),
                                _nodes (nodes), _elements (elements){};

barMesh::~barMesh(){

    _nodes.~Matrix();
    _elements.~Matrix();
};

const size_t barMesh::getNumEl(){
	return _numEl;
};

const size_t barMesh::getNumNode(){
	return _numNode;
};

const Matrix<double> barMesh::getNodes() {
	return _nodes;
};

const Matrix<size_t> barMesh::getElems() {
	return _elements;
};

const std::vector<size_t> barMesh::findElems(std::vector<size_t>& nodeIDs) {

    sort(nodeIDs.begin(), nodeIDs.end());

    if (nodeIDs.size() > _numNode || nodeIDs.size() == 0){

        throw std::invalid_argument("Given node vector has more nodes than mesh or has no ID! (getElems)");

    }else if (nodeIDs.back() >= _numNode) {

        throw std::invalid_argument("Given node ids are out of bounds of mesh node ids! (getElems)");

    };

	std::vector<size_t> elemIDs;

	for (size_t i = 0; i < _numEl; i++) {
		for (size_t j = 0; j < nodeIDs.size(); j++) {

			if (_elements(i,0) == nodeIDs[j] || _elements(i,1) == nodeIDs[j]) {

				elemIDs.push_back(i);

			}
		}
	};

	elemIDs.erase(std::unique(elemIDs.begin(), elemIDs.end()), elemIDs.end());

	return elemIDs;
};

const std::vector<size_t> barMesh::findNodes(size_t elemID) {

    if (elemID < _numEl){
        std::vector<size_t> nodeIDs(2);
        nodeIDs[0] = _elements(elemID,0);
        nodeIDs[1] = _elements(elemID,1);
        return nodeIDs;
        }
    else{
        throw std::invalid_argument("Element ID out of bounds! (findNodes)");
    };
};

void barMesh::addNodes(Matrix<double> & coords) {

    if (coords.getSize()[0] == 0 || coords.getSize()[1] != 3){
      throw std::runtime_error("Coordinates are not 3D or do not exist! (addNodes) ");
    };
	for (int i = 0; i < coords.getSize()[0]; i++) {

		//_nodes.addRow(); // implement add nodes
		_numNode += 1;

	};
};

void barMesh::deleteNodes(std::vector<size_t> & nodeIDs) {

    std::sort(nodeIDs.begin(), nodeIDs.end());
	nodeIDs.erase(std::unique(nodeIDs.begin(), nodeIDs.end()), nodeIDs.end());
    std::reverse(nodeIDs.begin(), nodeIDs.end());

    if (nodeIDs.empty() || nodeIDs.size() >= _numNode){
        throw std::runtime_error("Nodes to be deleted are either empty or outside of bounds!");
    }
    else if (nodeIDs.back() < 0 || nodeIDs[0] >= _numNode ) {
        throw std::runtime_error("Provided node IDs do not exist!");
    };

    for (int i = 0; i < nodeIDs.size() ; i++){

        std::cout << nodeIDs[i] << " ";
    };

   	// already sorted
	std::vector<int> elemIDs = findElems(nodeIDs);

	for (int j = 0; j < elemIDs.size() ; j++){

        std::cout << elemIDs[j] << " ";
    };

	std::reverse(elemIDs.begin(), elemIDs.end());

	for (int j = 0; j < elemIDs.size() ; j++){

        std::cout << elemIDs[j] << " ";
    };

	int newSize = _numNode - nodeIDs.size();
	std::vector<std::vector<float>> nodesTemp(newSize, {0,0,0});


	for (int i = 0; i < nodeIDs.size(); i++) {
	    for (int j = 0; j < _numNode; j++){

				if (nodeIDs[i] != j){
				    nodesTemp[j-i] = _nodes[i];
				}
		}
	};
	std::cout << '\n';
	for (int i = 0; i < nodesTemp.size(); i++){
	    for (int j = 0; j < 3 ; j++){

					std::cout << nodesTemp[i][j] << " ";
					}
	    std::cout << '\n';
	};

	_numNode = _numNode - nodeIDs.size();

	for (int i : elemIDs) {

		_elements.erase(_elements.begin() + i);

	};

	_numEl = _numEl - elemIDs.size();

};

void Bar3DMesh::deleteElems(std::vector<int>& elemIDs) {

	std::sort(elemIDs.begin(), elemIDs.end(), std::greater<int>());

	if (elemIDs.empty() || elemIDs.size() > _numEl){

	    throw std::runtime_error("Given element ids either empty or have more elements than mesh!");}

	else if(elemIDs.back() < 0 || elemIDs[0] >= _numEl){

	    throw std::runtime_error("Given element ids are out of bounds!");
	};

	std::vector<int> nodeIDs;

	for (int i : elemIDs) {

		_elements.erase(_elements.begin() + i);

		nodeIDs.push_back(_elements[i][0]);
		nodeIDs.push_back(_elements[i][1]);

	};

	_numEl = _numEl - elemIDs.size();


	std::sort(nodeIDs.begin(), nodeIDs.end());
	nodeIDs.erase(std::unique(nodeIDs.begin(), nodeIDs.end()), nodeIDs.end());

	std::reverse(nodeIDs.begin(), nodeIDs.end());

	for (int i : nodeIDs) {

		_nodes.erase(_nodes.begin() + i);

	};

	_numNode = _numNode - nodeIDs.size();

};

void barMesh::connectNodes(std::vector<int> & nodeIDs) {

	_elements.push_back(nodeIDs);
	_numEl += 1;

};
