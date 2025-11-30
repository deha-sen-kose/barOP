#include "../include/barOP/trussElement.h"
#include <gtest/gtest.h>

TEST(TrussElementTest, ConstructorStoresReferences)
{
    Material mat("mat1",210e9);
    Node n1(1, 0.0, 0.0, 0.0);
    Node n2(2, 1.0, 0.0, 0.0);

    TrussElement elem(1, n1, n2, mat, 0.01);

    // Check references
    EXPECT_EQ(&elem.getNode1(), &n1);
    EXPECT_EQ(&elem.getNode2(), &n2);
}

TEST(TrussElementTest, ComputeLength)
{
    Material mat("mat1",210e9);
    Node n1(1, 0.0, 0.0, 0.0);
    Node n2(2, 3.0, 4.0, 12.0);

    TrussElement elem(1, n1, n2, mat, 0.01);

    double expected = std::sqrt(3.0*3.0 + 4.0*4.0 + 12.0*12.0);
    EXPECT_DOUBLE_EQ(elem.computeLength(), expected);
}

TEST(TrussElementTest, DOFValues)
{
    Material mat("mat1",210e9);
    Node n1(1, 0, 0, 0);
    Node n2(2, 1, 0, 0);

    TrussElement elem(5, n1, n2, mat, 0.01);

    std::vector<int> dof = elem.getDOF();

    ASSERT_EQ(dof.size(), 6);
    EXPECT_EQ(dof[0], 3*(n1.getID()-1) + 1); // u1
    EXPECT_EQ(dof[1], 3*(n1.getID()-1) + 2); // v1
    EXPECT_EQ(dof[2], 3*(n1.getID()-1) + 3); // w1
    EXPECT_EQ(dof[3], 3*(n2.getID()-1) + 1); // u2
    EXPECT_EQ(dof[4], 3*(n2.getID()-1) + 2); // v2
    EXPECT_EQ(dof[5], 3*(n2.getID()-1) + 3); // w2
}

TEST(TrussElementTest, ComputeStrain)
{
    Material mat("mat1",210e9);
    Node n1(1, 0, 0, 0);
    Node n2(2, 2, 0, 0);

    TrussElement elem(1, n1, n2, mat, 0.01);

    // Displacement vector (u1, v1, w1, u2, v2, w2)
    std::vector<double> u = {0.0, 0.0, 0.0, 0.2, 0.0, 0.0};

    // Expected strain = (u2 - u1) / L = 0.2 / 2 = 0.1
    double strain = elem.computeElStrain(u);

    EXPECT_NEAR(strain, 0.1, 1e-12);
}

TEST(TrussElementTest, ComputeStress)
{
    Material mat("mat1",200e9);  // E = 200 GPa
    Node n1(1, 0, 0, 0);
    Node n2(2, 4, 0, 0);

    TrussElement elem(1, n1, n2, mat, 0.01);

    // Displacement vector
    std::vector<double> u = {0.0, 0.0, 0.0, 0.4, 0.0, 0.0};

    // strain = (0.4 - 0.0) / 4 = 0.1
    // stress = E * strain = 200e9 * 0.1 = 20e9
    double stress = elem.computeElStress(u);

    EXPECT_NEAR(stress, 20e9, 1e-3);
}

TEST(TrussElementTest, TransformationMatrixShape)
{
    Material mat("mat1",210e9);
    Node n1(1, 0, 0, 0);
    Node n2(2, 1, 1, 0);

    TrussElement elem(1, n1, n2, mat, 0.01);

    Matrix<double> T = elem.computeTransformation();

    double cxy = 1/std::sqrt(2);
    double cz = 0.0;
    Matrix<double> ans = {{cxy, cxy, cz, 0, 0, 0},
                          {0 , 0, 0, cxy, cxy, cz}};

    EXPECT_TRUE(T.getSize()[0] == 2 && T.getSize()[1] == 6);

    for (size_t i = 0; i < ans.getSize()[0]; ++i){
        for (size_t j = 0; j < ans.getSize()[1]; ++j){
            EXPECT_DOUBLE_EQ(ans(i,j), T(i,j));
        }
    }
}

TEST(TrussElementTest, GlobalStiffnessMatrixHasCorrectSize)
{
    Material mat("mat1",210e9);
    Node n1(1, 0, 0, 0);
    Node n2(2, 1, 0, 0);

    TrussElement elem(1, n1, n2, mat, 0.01);

    Matrix<double> K = elem.computeGlobalStiffnessMtx();

    // Expected size: 6x6 (3 DOF per node)
    EXPECT_EQ(K.getSize()[0], 6);
    EXPECT_EQ(K.getSize()[1], 6);
}

TEST(TrussElementTest, GlobalElStiffnessMatrixComputation)
{
    // Example taken from:
    // Dr. Gul Ahmed Jokhio, n.d.
    // https://ocw.ump.edu.my/pluginfile.php/552/mod_resource/content/2/10_Space_Truss_Example.pdf

    Material mat("mat1",1E7);
    Node n1(1, 0.32, 1.5, 0.1848);
    Node n2(2, 0.0, 0.0, 0.0);

    double A = 0.0012566;
    TrussElement elem1(1, n1, n2, mat, A);

    EXPECT_NEAR(elem1.computeLength(), 1.5448, 1e-4);

    Matrix<double> Ke1 = elem1.computeGlobalStiffnessMtx();
    Matrix<double> Ke1_ans = {{349.0227, 1636.044, 201.5606, -349.023, -1636.04, -201.561},
                              {1636.044, 7668.957, 944.8155, -1636.04, -7668.96, -944.815},
                              {201.5606, 944.8155, 116.4013, -201.561, -944.815, -116.401},
                              {-349.023, -1636.04, -201.561, 349.0227, 1636.044, 201.5606},
                              {-1636.04, -7668.96, -944.815, 1636.044, 7668.957, 944.8155},
                              {-201.561, -944.815, -116.401, 201.5606, 944.8155, 116.4013}};


    // Expected size: 6x6 (3 DOF per node)
    EXPECT_EQ(Ke1.getSize()[0], 6);
    EXPECT_EQ(Ke1.getSize()[1], 6);

    for (size_t i = 0; i < 6; ++i){
        for (size_t j = 0; j < 6; ++j){
            EXPECT_NEAR(Ke1_ans(i,j), Ke1(i,j), 1);
        }
    }
}
