#include "../include/barOP/barMesh.h"
#include "math/Matrix.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>
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

    if (coords.getSize()[0] == 0 || coords.getSize()[1] != dim){
      throw std::runtime_error("Coordinates are not 3D or do not exist! (addNodes) ");
    };

    _nodes.addRows(coords);

    _numNode += coords.getSize()[0];

};

void barMesh::deleteNodes(std::vector<size_t> & nodeIDs) {

    if (nodeIDs.size() == 0 || nodeIDs.size() > _numNode){
      throw std::invalid_argument("Node IDs do not exist or out of bounds! (addNodes)");
    };

    std::sort(nodeIDs.begin(), nodeIDs.end());
	nodeIDs.erase(std::unique(nodeIDs.begin(), nodeIDs.end()), nodeIDs.end());
    std::reverse(nodeIDs.begin(), nodeIDs.end());

    if (nodeIDs[0] >= _numNode){
      throw std::invalid_argument("Node IDs are out of bounds! (addNodes)");
    };

    _nodes.deleteRows(nodeIDs);
    _numNode -= nodeIDs.size();

    // Don't forget to check for affected elements!

};

void barMesh::deleteElems(std::vector<size_t>& elemIDs) {

	std::sort(elemIDs.begin(), elemIDs.end(), std::greater<size_t>());

	if (elemIDs.empty() || elemIDs.size() > _numEl){

	    throw std::runtime_error("Given element ids either empty or have more elements than mesh!");}

	else if(elemIDs.back() < 0 || elemIDs[0] >= _numEl){

	    throw std::runtime_error("Given element ids are out of bounds!");
	};

	_elements.deleteRows(elemIDs);
	_numEl -= elemIDs.size();

	// Don't forget to check for affected nodes!

};

void barMesh::connectNodes(Matrix<size_t>& nodeIDs){

    if(nodeIDs.getSize()[0] == 0 || nodeIDs.getSize()[0] > _numNode){
        throw std::invalid_argument("Given node IDs are out of bounds or do not exist! (barMesh/connectNodes)");
    };

	_elements.addRows(nodeIDs);
	_numEl += nodeIDs.getSize()[0];

};
