#include <gtest/gtest.h>
#include "../include/math/Matrix.h"
#include "../include/math/mathflb.h"

TEST(gaussSeidelTest, gaussSeidel2b2) {

    Matrix<double> A = {{16.0, 3.0},
                     {7.0,-11.0}};

    std::vector<double> b = {11.0,13.0};

    std::vector<double> expected = {0.812183, -0.664975};

    std::vector<double> result = gaussSeidel(A, b);                     

    

    for (size_t i = 0; i < 2; ++i) {
        EXPECT_NEAR(expected[i], result[i], 1E-6);            
    }
}

TEST(gaussSeidelTest, gaussSeidel3b3) {

    Matrix<double> A = {{4, 1, 2},
                     {3, 5, 1},
                     {1, 1, 3}};

    std::vector<double> b = {4,7,3};                 

    std::vector<double> expected = {0.5, 1.0, 0.5};

    std::vector<double> result = gaussSeidel(A, b);
    
    for (size_t i = 0; i < 2; ++i) {
        EXPECT_NEAR(expected[i], result[i], 1E-6);            
    }
    
}


TEST(ForwardSubstitutionTest, KnownLowerTriangularSystem) {
    Matrix<double> L(3,3,0.0);

    L(0,0)=2;

    L(1,0)=1; L(1,1)=3;

    L(2,0)=4; L(2,1)=2; L(2,2)=1;

    std::vector<double> b = {2, 5, 10};

    auto y = frwdSubstitution(L, b);

    // manually computed:
    // y0 = 2/2 = 1
    // y1 = (5 - 1*1)/3 = 4/3
    // y2 = (10 - 4*1 - 2*(4/3)) / 1 = 10 - 4 - 8/3 = 6 - 8/3 = 10/3

    EXPECT_NEAR(y[0], 1.0, 1e-9);
    EXPECT_NEAR(y[1], 4.0/3.0, 1e-9);
    EXPECT_NEAR(y[2], 10.0/3.0, 1e-9);
}

TEST(ForwardSubstitutionTest, SizeMismatchThrows) {
    Matrix<double> L(2,2,0.0);
    L(0,0)=1; L(1,1)=1;

    std::vector<double> b = {1,2,3}; // wrong size

    EXPECT_THROW(frwdSubstitution(L, b), std::exception);
}


TEST(ForwardSubstitutionTest, IdentityMatrix) {
    Matrix<double> L(3,3,0.0);
    for (int i = 0; i < 3; ++i)
        L(i,i) = 1.0;

    std::vector<double> b = {5.0, -2.0, 7.0};

    auto y = frwdSubstitution(L, b);

    EXPECT_EQ(y.size(), b.size());
    for (int i = 0; i < 3; ++i) {
        EXPECT_NEAR(y[i], b[i], 1e-12);
    }
}


TEST(ForwardSubstitutionTest, StrictLowerTriangularStructure) {
    Matrix<double> L(4,4,0.0);

    L(0,0)=1;
    L(1,0)=2; L(1,1)=1;
    L(2,0)=3; L(2,1)=4; L(2,2)=1;
    L(3,0)=5; L(3,1)=6; L(3,2)=7; L(3,3)=1;

    std::vector<double> b = {1,2,3,4};

    auto y = frwdSubstitution(L, b);

    EXPECT_EQ(y.size(), 4);
}


TEST(ForwardSubstitutionTest, ZeroDiagonalThrows) {
    Matrix<double> L(2,2,0.0);
    L(0,0)=1;
    L(1,0)=1;
    L(1,1)=0.0; // invalid

    std::vector<double> b = {1,1};

    EXPECT_THROW(frwdSubstitution(L, b), std::runtime_error);
}


TEST(ForwardSubstitutionTest, ResidualCheck) {
    Matrix<double> L(3,3,0.0);

    L(0,0)=2;

    L(1,0)=1; L(1,1)=3;

    L(2,0)=4; L(2,1)=2; L(2,2)=1;

    std::vector<double> y_true = {1.0, 2.0, 3.0};

    // compute b = L * y_true
    std::vector<double> b(3,0.0);
    for (int i=0;i<3;i++){
        for (int j=0;j<=i;j++){
            b[i] += L(i,j)*y_true[j];
        }
    }

    auto y = frwdSubstitution(L, b);

    for (int i = 0; i < 3; ++i) {
        EXPECT_NEAR(y[i], y_true[i], 1e-9);
    }
}

TEST(BackwardSubstitutionTest, IdentityMatrix) {
    Matrix<double> L(3,3,0.0);
    for (int i = 0; i < 3; ++i)
        L(i,i) = 1.0;

    std::vector<double> y = {4.0, -2.0, 7.0};

    auto x = bckwrdSubstitution(L, y);

    EXPECT_EQ(x.size(), y.size());
    for (int i = 0; i < 3; ++i) {
        EXPECT_NEAR(x[i], y[i], 1e-12);
    }
}


TEST(BackwardSubstitutionTest, KnownSystem) {
    Matrix<double> L(3,3,0.0);

    L(0,0)=2;

    L(1,0)=1; L(1,1)=3;

    L(2,0)=4; L(2,1)=2; L(2,2)=1;

    std::vector<double> y = {2, 5, 10};

    auto x = bckwrdSubstitution(L, y);

    EXPECT_NEAR(x[2], 10.0, 1e-9);
    EXPECT_NEAR(x[1], -5.0, 1e-9);
    EXPECT_NEAR(x[0], -16.5, 1e-9);
}


TEST(BackwardSubstitutionTest, UpperTriangularStructure) {
    Matrix<double> L(4,4,0.0);

    L(0,0)=1;
    L(1,0)=2; L(1,1)=1;
    L(2,0)=3; L(2,1)=4; L(2,2)=1;
    L(3,0)=5; L(3,1)=6; L(3,2)=7; L(3,3)=1;

    std::vector<double> y = {1,2,3,4};

    auto x = bckwrdSubstitution(L, y);

    EXPECT_EQ(x.size(), 4);
}


TEST(BackwardSubstitutionTest, ZeroDiagonalThrows) {
    Matrix<double> L(2,2,0.0);

    L(0,0)=1;
    L(1,0)=2;
    L(1,1)=0.0;

    std::vector<double> y = {1,1};

    EXPECT_THROW(bckwrdSubstitution(L, y), std::runtime_error);
}


TEST(BackwardSubstitutionTest, ConsistencyWithCholesky) {
    Matrix<double> A(3,3,0.0);

    A(0,0)=4; A(0,1)=2; A(0,2)=0;
    A(1,0)=2; A(1,1)=5; A(1,2)=1;
    A(2,0)=0; A(2,1)=1; A(2,2)=3;

    std::vector<double> x_true = {1.0, 2.0, 3.0};

    // b = A x_true
    std::vector<double> b(3,0.0);
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            b[i] += A(i,j)*x_true[j];
        }
    }

    // Cholesky factorization assumed via your solver pipeline
    auto x = choleskySolver(A, b);

    for (int i=0;i<3;i++){
        EXPECT_NEAR(x[i], x_true[i], 1e-8);
    }    
}


TEST(BackwardSubstitutionTest, ResidualCheck) {
    Matrix<double> L(3,3,0.0);

    L(0,0)=2;
    L(1,0)=1; L(1,1)=3;
    L(2,0)=4; L(2,1)=2; L(2,2)=1;

    std::vector<double> x_true = {1,2,3};

    // compute y = L^T x_true
    std::vector<double> y(3,0.0);

    for (int i=0;i<3;i++){
        for (int j=i;j<3;j++){
            y[i] += L(j,i)*x_true[j];
        }
    }

    auto x = bckwrdSubstitution(L, y);

    for (int i = 0; i < 3; ++i) {
        EXPECT_NEAR(x[i], x_true[i], 1e-9);
    }
}



TEST(CholeskySolverTest, Solves2x2System) {
    Matrix<double> A(2,2,0.0);
    A(0,0) = 4;  A(0,1) = 2;
    A(1,0) = 2;  A(1,1) = 3;

    std::vector<double> b = {6, 7};

    std::vector<double> x = choleskySolver(A, b);

    EXPECT_NEAR(x[0], 0.5, 1e-6);
    EXPECT_NEAR(x[1], 2.0, 1e-6);
}

TEST(CholeskySolverTest, IdentityMatrix) {
    Matrix<double> A(3,3,0.0);
    for(int i = 0; i < 3; ++i)
        A(i,i) = 1.0;

    std::vector<double> b = {3.0, -2.0, 5.0};

    auto x = choleskySolver(A, b);

    for(int i = 0; i < 3; ++i){
        EXPECT_NEAR(x[i], b[i], 1e-9);
    }
}

TEST(CholeskySolverTest, KnownSolution3x3) {
    Matrix<double> L(3,3,0.0);
    L(0,0)=2;
    L(1,0)=1; L(1,1)=2;
    L(2,0)=0; L(2,1)=1; L(2,2)=1;

    Matrix<double> A(3,3,0.0);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<3;k++){
                A(i,j) += L(i,k)*L(j,k);
            }
        }
    }

    std::vector<double> x_true = {1.0, 2.0, 3.0};

    std::vector<double> b(3,0.0);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            b[i] += A(i,j)*x_true[j];
        }
    }

    auto x = choleskySolver(A, b);

    for(int i=0;i<3;i++){
        EXPECT_NEAR(x[i], x_true[i], 1e-6);
    }
}

TEST(CholeskySolverTest, SizeMismatchThrows) {
    Matrix<double> A(2,2,0.0);
    std::vector<double> b = {1.0, 2.0, 3.0};

    EXPECT_THROW(choleskySolver(A, b), std::invalid_argument);
}

TEST(CholeskySolverTest, NonSPDMatrixProducesNaN) {
    Matrix<double> A(2,2,0.0);
    A(0,0)=1; A(0,1)=2;
    A(1,0)=2; A(1,1)=1; // not SPD

    std::vector<double> b = {1,1};

    auto x = choleskySolver(A, b);

    EXPECT_TRUE(std::isnan(x[0]) || std::isnan(x[1]));
}

TEST(CholeskySolverTest, ResidualIsSmall) {
    Matrix<double> A(2,2,0.0);
    A(0,0)=5; A(0,1)=1;
    A(1,0)=1; A(1,1)=3;

    std::vector<double> b = {6,5};

    auto x = choleskySolver(A, b);

    std::vector<double> Ax(2,0.0);
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            Ax[i] += A(i,j)*x[j];
        }
    }

    for(int i=0;i<2;i++){
        EXPECT_NEAR(Ax[i], b[i], 1e-6);
    }
}
