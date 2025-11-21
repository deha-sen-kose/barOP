#include "../include/barOP/barMesh.h"
#include "math/Matrix.h"
#include <cstddef>
#include <gtest/gtest.h>


TEST(barMeshTest, findElements)
{

	size_t nodeNumT = 4;
	size_t elNumT = 4;
	Matrix<double>
		nodesT =
				{ {0.0,0.0,0.0},{2.0,0.0,0.0},
				  {2.0,2.0,0.0},{0.0,2.0,0.0} };
	Matrix<size_t> elementsT = {{0, 1}, { 1,2 },
				                  { 0,2 }, { 2,3 }};

	barMesh mshT (elNumT, nodeNumT, nodesT, elementsT);

	std::vector<size_t> nodesIDsT1 = { 1,2 };
	std::vector<size_t> ans1 = { 0,1,2,3 };
	EXPECT_EQ(mshT.findElems(nodesIDsT1), ans1);

	std::vector<size_t> nodesIDsT2 = { 2,3 };
	std::vector<size_t> ans2 = { 1,2,3 };
	EXPECT_EQ(mshT.findElems(nodesIDsT2), ans2);

	std::vector<size_t> nodesIDsT3 = { 1,3 };
	std::vector<size_t> ans3 = { 0,1,3 };
	EXPECT_EQ(mshT.findElems(nodesIDsT3), ans3);

	std::vector<size_t> nodesIDsT4 = { 1,3,5,7,8 };
	EXPECT_ANY_THROW(mshT.findElems(nodesIDsT4));

	std::vector<size_t> nodesIDsT6 = { 8 };
	EXPECT_ANY_THROW(mshT.findElems(nodesIDsT6));
};

TEST(barMeshTest, findNodes)
{
   	size_t nodeNumT = 4;
	size_t elNumT = 4;
	Matrix<double>
		nodesT =
				{ {0.0,0.0,0.0},{2.0,0.0,0.0},
				  {2.0,2.0,0.0},{0.0,2.0,0.0} };
	Matrix<size_t> elementsT = {{0, 1}, { 1,2 },
				                  { 0,2 }, { 2,3 }};

	barMesh mshT (elNumT, nodeNumT, nodesT, elementsT);

	size_t elemID1 = 0;
	std::vector<size_t> ans1 = {0,1};
	int elemID2 = 1;
	std::vector<size_t> ans2 = {1,2};
	int elemID3 = 2;
	std::vector<size_t> ans3 = {0,2};
	int elemID4 = 3;
	std::vector<size_t> ans4 = {2,3};
	int elemID5 = -5;
	int elemID6 = 120;

	EXPECT_EQ(mshT.findNodes(elemID1), ans1);
	EXPECT_EQ(mshT.findNodes(elemID2), ans2);
	EXPECT_EQ(mshT.findNodes(elemID3), ans3);
	EXPECT_EQ(mshT.findNodes(elemID4), ans4);

	EXPECT_ANY_THROW(mshT.findNodes(elemID5));
	EXPECT_ANY_THROW(mshT.findNodes(elemID6));
};

TEST(barMeshTest, addNodes)
{
   	size_t nodeNumT = 4;
	size_t elNumT = 4;
	Matrix<double>
		nodesT =
				{ {0.0,0.0,0.0},{2.0,0.0,0.0},
				  {2.0,2.0,0.0},{0.0,2.0,0.0} };
	Matrix<size_t> elementsT = {{0, 1}, { 1,2 },
				                  { 0,2 }, { 2,3 }};

	barMesh mshT (elNumT, nodeNumT, nodesT, elementsT);

	Matrix<double> coords1 = {{6.0, 6.0, 5.0},
	                          {12.0, 12.0, 8.0},
							  {-5.0, -8.0, 11.0}};

	size_t nodeNumNew = mshT.getNumNode() + coords1.getSize()[0];
	mshT.addNodes(coords1);
	EXPECT_EQ(mshT.getNumNode(), nodeNumNew);
	EXPECT_EQ(mshT.getNumEl(), elNumT);

	EXPECT_DOUBLE_EQ(mshT.getElems()(0,0), 0);
	EXPECT_DOUBLE_EQ(mshT.getElems()(0,1), 1);
	EXPECT_DOUBLE_EQ(mshT.getElems()(1,0), 1);
	EXPECT_DOUBLE_EQ(mshT.getElems()(1,1), 2);
	EXPECT_DOUBLE_EQ(mshT.getElems()(2,0), 0);
	EXPECT_DOUBLE_EQ(mshT.getElems()(2,1), 2);
	EXPECT_DOUBLE_EQ(mshT.getElems()(3,0), 2);
	EXPECT_DOUBLE_EQ(mshT.getElems()(3,1), 3);

	Matrix<double> ans1 = { {0.0,0.0,0.0},{2.0,0.0,0.0},{2.0,2.0,0.0},{0.0,2.0,0.0},
						    {6.0, 6.0, 5.0}, {12.0, 12.0, 8.0},{-5.0, -8.0, 11.0}};

	Matrix<double> newNodes = mshT.getNodes();
	EXPECT_EQ(newNodes.getSize()[0], ans1.getSize()[0]);
	EXPECT_EQ(newNodes.getSize()[1], ans1.getSize()[1]);
	for (size_t i = 0; i < newNodes.getSize()[0]; ++i){
	    for (size_t j = 0; j < newNodes.getSize()[1]; ++j){
			EXPECT_EQ(newNodes(i,j),ans1(i,j));
		}
	}

	Matrix<double> coord2 = {{2.0,3.0,5.0,6.0},{5.0,6.0,7.0,8.0}};
	EXPECT_THROW(mshT.addNodes(coord2), std::runtime_error);

	Matrix<double> coord3;
	EXPECT_THROW(mshT.addNodes(coord3), std::runtime_error);
};

TEST(barMeshTest, deleteNodes)
{
    size_t nodeNumT = 4;
	size_t elNumT = 4;
	Matrix<double>
		nodesT =
				{ {0.0,0.0,0.0},{2.0,0.0,0.0},
				  {2.0,2.0,0.0},{0.0,2.0,0.0} };
	Matrix<size_t> elementsT = {{0, 1}, { 1,2 },
				                  { 0,2 }, { 2,3 }};

	barMesh mshT (elNumT, nodeNumT, nodesT, elementsT);

	std::vector<size_t> nodeIDs1;
	EXPECT_ANY_THROW(mshT.deleteNodes(nodeIDs1));

	std::vector<size_t> nodeIDs2 = {0,1,2,3,4};
	EXPECT_ANY_THROW(mshT.deleteNodes(nodeIDs2));

	std::vector<size_t> nodeIDs3 = {3,79};
	EXPECT_ANY_THROW(mshT.deleteNodes(nodeIDs3));

	std::vector<size_t> nodeIDs5 = {1,3};

	mshT.deleteNodes(nodeIDs5);

	Matrix<double> newNodes = mshT.getNodes();
	Matrix<double> ans5a =  { {0.0,0.0,0.0}, {2.0,2.0,0.0} };
	for (size_t i = 0; i < newNodes.getSize()[0]; ++i){
	    for (size_t j = 0; j < newNodes.getSize()[1]; ++j){
			EXPECT_EQ(newNodes(i,j), ans5a(i,j));
		}
	}

	EXPECT_EQ(mshT.getNumEl(), 2);
	EXPECT_EQ(mshT.getNumNode(), 2);

	Matrix<size_t> newElements = mshT.getElems();
	Matrix<size_t> ans5b = {{0,2}};
	for (size_t i = 0; i < newElements.getSize()[0]; ++i){
	    for (size_t j = 0; j < newElements.getSize()[1]; ++j){
			EXPECT_EQ(newElements(i,j), ans5b(i,j));
		}
	}
};

TEST(barMeshTest, deleteElems)
{
    size_t nodeNumT = 4;
	size_t elNumT = 4;
	Matrix<double>
		nodesT =
				{ {0.0,0.0,0.0},{2.0,0.0,0.0},
				  {2.0,2.0,0.0},{0.0,2.0,0.0} };
	Matrix<size_t> elementsT = {{0, 1}, { 1,2 },
				                  { 0,2 }, { 2,3 }};

	barMesh mshT (elNumT, nodeNumT, nodesT, elementsT);

	std::vector<size_t> elemIDs1;
	EXPECT_THROW(mshT.deleteElems(elemIDs1), std::runtime_error);
	std::vector<size_t> elemIDs2 = {1,2,3,4};
	EXPECT_THROW(mshT.deleteElems(elemIDs2), std::runtime_error);
	std::vector<size_t> elemIDs3 = {1,44};
	EXPECT_THROW(mshT.deleteElems(elemIDs3), std::runtime_error);
	std::vector<size_t> elemIDs4 = {0,1,2,3,4};
	EXPECT_THROW(mshT.deleteElems(elemIDs4), std::runtime_error);

	std::vector<size_t> elemIDs5 = {0,1};
	mshT.deleteElems(elemIDs5);
	EXPECT_EQ(mshT.getNumEl(), 2);
	EXPECT_EQ(mshT.getNumNode(), 3);
	Matrix<double> ans5a = {{0.0,0.0,0.0},{2.0,2.0,0.0},{0.0,2.0,0.0}};
	Matrix<double> newNodes = mshT.getNodes();
	for (size_t i = 0; i < newNodes.getSize()[0]; ++i){
	    for (size_t j = 0; j < newNodes.getSize()[1]; ++j){
			EXPECT_EQ(newNodes(i,j), ans5a(i,j));;
		};
	};

	Matrix<size_t> ans5b = {{ 0,2 }, { 2,3 }};
	Matrix<size_t> newElements = mshT.getElems();
	for (size_t i = 0; i < newElements.getSize()[0]; ++i){
	    for (size_t j = 0; j < newElements.getSize()[1]; ++j){
			EXPECT_EQ(newElements(i,j), ans5b(i,j));;
		}
	}
};
