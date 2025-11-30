#include "../include/barOP/trussStructure.h"
#include <gtest/gtest.h>
#include <vector>

TEST(TrussStructureTest, AddNodeAssignsIncreasingIDs)
{
    TrussStructure ts;

    Node& n1 = ts.addNode(0.0, 0.0, 0.0);
    Node& n2 = ts.addNode(1.0, 0.0, 0.0);

    const auto& nodes = ts.getNodes();

    ASSERT_EQ(nodes.size(), 2);
    EXPECT_EQ(nodes[0]->getID(), 1);
    EXPECT_EQ(nodes[1]->getID(), 2);

    EXPECT_EQ(nodes[1].get(), &n2);
}

TEST(TrussStructureTest, AddMaterialStoresCorrectly)
{
    TrussStructure ts;
    Material& m1 = ts.addMaterial("steel", 200e9);
    Material& m2 = ts.addMaterial("aluminum", 70e9);

    const auto& mats = ts.getMaterials();

    ASSERT_EQ(mats.size(), 2);
    EXPECT_EQ(mats[0].getName(), "steel");
    EXPECT_EQ(mats[0].getE(), 200e9);
    EXPECT_EQ(mats[1].getName(), "aluminum");
    EXPECT_EQ(mats[1].getE(), 70e9);
}

TEST(TrussStructureTest, AddTrussElementAssignsCorrectIDs)
{
    TrussStructure ts;
    Material& steel = ts.addMaterial("steel", 210e9);

    Node& n1 = ts.addNode(0,0,0);
    Node& n2 = ts.addNode(1,0,0);
    Node& n3 = ts.addNode(0,1,0);

    TrussElement& e1 = ts.addTrussElement(n1, n2, steel, 0.01);
    TrussElement& e2 = ts.addTrussElement(n2, n3, steel, 0.02);

    const auto& elems = ts.getElements();

    ASSERT_EQ(elems.size(), 2);
    EXPECT_EQ(elems[0]->getID(), 1);
    EXPECT_EQ(elems[1]->getID(), 2);

    EXPECT_EQ(&elems[0]->getNode1(), &n1);
    EXPECT_EQ(&elems[0]->getNode2(), &n2);
}

// ----------------------
// BC and force tests
// ----------------------

TEST(TrussStructureTest, AddBoundaryConditionsStoresCorrectValues)
{
    TrussStructure ts;

    ts.addBCs({1, 2, 5, 9});

    const auto& bc = ts.getConditions();

    ASSERT_EQ(bc.size(), 4);
    EXPECT_TRUE(bc.at(1));
    EXPECT_TRUE(bc.at(2));
    EXPECT_TRUE(bc.at(5));
    EXPECT_TRUE(bc.at(9));
}

TEST(TrussStructureTest, AddForcesStoresCorrectValues)
{
    TrussStructure ts;

    ts.addForces({3, 4, 6}, {10.0, -5.0, 20.0});

    const auto& F = ts.getForces();

    ASSERT_EQ(F.size(), 3);
    EXPECT_DOUBLE_EQ(F.at(3), 10.0);
    EXPECT_DOUBLE_EQ(F.at(4), -5.0);
    EXPECT_DOUBLE_EQ(F.at(6), 20.0);
}

// ----------------------
// Assembly tests
// ----------------------

TEST(TrussStructureTest, AssembleStiffnessReturnsCorrectSize)
{
    TrussStructure ts;
    Material& steel = ts.addMaterial("steel", 1e7);

    Node& n1 = ts.addNode(0,0,0);
    Node& n2 = ts.addNode(1,0,0);

    ts.addTrussElement(n1, n2, steel, 0.01);

    Matrix<double> K = ts.assembleStffMtx();

    EXPECT_EQ(K.getSize()[0], 6);
    EXPECT_EQ(K.getSize()[1], 6);
}

TEST(TrussStructureTest, ApplyHomBCsRemovesRowsAndCols)
{
    TrussStructure ts;
    Material& steel = ts.addMaterial("steel", 1e7);

    Node& n1 = ts.addNode(0,0,0);
    Node& n2 = ts.addNode(1,0,0);
    ts.addTrussElement(n1, n2, steel, 0.01);

    Matrix<double> K = ts.assembleStffMtx();
    std::vector<double> F = ts.createForceVector();

    ts.addBCs({1,2,3});  // fix node 1

    ts.applyHomBCs(K, F);

    // Expect removal of 3 DOF → 3x3 system
    EXPECT_EQ(K.getSize()[0], 3);
    EXPECT_EQ(K.getSize()[1], 3);
    EXPECT_EQ(F.size(), 3);
}

// ----------------------
// Solver tests (size-only)
// ----------------------

TEST(TrussStructureTest, CreateForceVectorHasCorrectSize)
{
    TrussStructure ts;
    Material& steel = ts.addMaterial("steel", 1e7);

    ts.addNode(0,0,0);
    ts.addNode(1,0,0);

    ts.addForces({4}, {100.0});

    auto F = ts.createForceVector();

    EXPECT_EQ(F.size(), 6);  // 2 nodes → 6 DOF
}

TEST(TrussStructureTest, ReturnDispVectorInsertsZerosForBCs)
{
    TrussStructure ts;
    Node& n1 = ts.addNode(0.0, 0.0, 0.0);
    Node& n2 = ts.addNode(1.0, 0.0, 0.0);

    ts.addBCs({1,2});  // DOF 1 and 2 fixed

    std::vector<double> u_red = {10, 20, 30, 40};  // for DOF 3,4,5,6
    std::vector<double> u_full = ts.returnDispVector(u_red);


    ASSERT_EQ(u_full.size(), 6);   // DOF = max index = 6
    EXPECT_DOUBLE_EQ(u_full[0], 0.0);
    EXPECT_DOUBLE_EQ(u_full[1], 0.0);
}

TEST(TrussStructureTest, StrainsVectorHasCorrectSize)
{
    TrussStructure ts;
    Material& steel = ts.addMaterial("steel", 200e9);

    Node& n1 = ts.addNode(0,0,0);
    Node& n2 = ts.addNode(1,0,0);
    ts.addTrussElement(n1, n2, steel, 0.01);

    std::vector<double> u = {0,0,0,  0.1,0,0}; // 6 DOF

    auto strains = ts.computeStrains(u);

    ASSERT_EQ(strains.size(), 1);
}

TEST(TrussStructureTest, StressesVectorHasCorrectSize)
{
    TrussStructure ts;
    Material& steel = ts.addMaterial("steel", 200e9);

    Node& n1 = ts.addNode(0,0,0);
    Node& n2 = ts.addNode(1,0,0);
    ts.addTrussElement(n1, n2, steel, 0.01);

    std::vector<double> u = {0,0,0,  0.1,0,0};

    auto stresses = ts.computeStresses(u);

    ASSERT_EQ(stresses.size(), 1);
}

TEST(TrussStructureTest, NumericExample3D)
{
    // Example taken from:
    // Carlos A. Felippa, 2004, Introduction to Finite Element Methods (IFEM)

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

    std::vector<double> disp_ans = {
    0.000000, 0.000000, 0.000000,
    0.809536, -1.775600, 0.000000,
    0.280000, -1.792260, 0.000000,
    0.899001, -2.291930, 0.000000,
    0.560000, -2.316600, 0.000000,
    0.847500, -2.385940, 0.000000,
    0.847500, -2.421940, 0.000000,
    0.795999, -2.291930, 0.000000,
    1.135000, -2.316600, 0.000000,
    0.885464, -1.775600, 0.000000,
    1.415000, -1.792260, 0.000000,
    1.695000, 0.000000, 0.000000};

    EXPECT_EQ(disp_ans.size(), u.size());

    for(size_t i = 0; i < disp_ans.size(); ++i){

        EXPECT_NEAR(disp_ans[i], u[i], 1e-4);
    }

    std::vector<double> stress_ans = {
    28.0000, 28.0000, 28.7500, 28.7500,28.0000,
    28.0000,-6.2610,-6.0030,-6.0300,-6.0300,-6.0030,
    -6.2610,3.3330,3.0830,4.0000,3.0830,3.3330,1.6770,
    3.2020,3.2020, 1.6770};

    std::vector<double> stress = t1.computeStresses(u);

    EXPECT_EQ(stress_ans.size(), stress.size());

    for(size_t i = 0; i < stress_ans.size(); ++i){

        EXPECT_NEAR(stress_ans[i], stress[i], 1e-3);
    }

}
