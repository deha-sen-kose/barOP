#include "../include/math/Matrix.h"
#include <gtest/gtest.h>

TEST(MatrixLibTest, copyConstCopiesDimensionsCorrectly)
{
    Matrix<int>::allocations = 0;
    {
    Matrix<int> A = {{1,2,3}, {4,5,6}};

    Matrix<int> B(A);

    EXPECT_EQ(B.getSize()[0], 2);
    EXPECT_EQ(B.getSize()[1], 3);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, copyConstCopiesValuesCorrectly)
{
    Matrix<int>::allocations = 0;
    {
    Matrix<int> A = {{1,2,3}, {4,5,6}};
    Matrix<int> B(A);

    EXPECT_EQ(B(0,0), 1);
    EXPECT_EQ(B(0,1), 2);
    EXPECT_EQ(B(0,2), 3);
    EXPECT_EQ(B(1,0), 4);
    EXPECT_EQ(B(1,1), 5);
    EXPECT_EQ(B(1,2), 6);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, copyConstPerformsDeepCopy)
{
    Matrix<int>::allocations = 0;
    {
    Matrix<int> A = {{1,2}, {3,4}};
    Matrix<int> B(A);

    // modify B only
    B(0,0) = 99;
    B(1,1) = 88;

    // A must remain unchanged
    EXPECT_EQ(A(0,0), 1);
    EXPECT_EQ(A(1,1), 4);

    // B contains new values
    EXPECT_EQ(B(0,0), 99);
    EXPECT_EQ(B(1,1), 88);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, copyConstCopiesEmptyMatrix)
{
    Matrix<int>::allocations = 0;
    {
    Matrix<int> A;            // assume default constructor makes size=0x0
    Matrix<int> B(A);

    EXPECT_EQ(B.getSize()[0], 0);
    EXPECT_EQ(B.getSize()[1], 0);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, copyConstHandlesSelfAssignment)
{
    Matrix<int>::allocations = 0;
    {
    Matrix<int> A = {{10,20}, {30,40}};

    // Requires operator=, not copy ctor, but important to test
    A = A;

    EXPECT_EQ(A(0,0), 10);
    EXPECT_EQ(A(0,1), 20);
    EXPECT_EQ(A(1,0), 30);
    EXPECT_EQ(A(1,1), 40);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, copyAllocationCountIncreases)
{
    Matrix<int>::allocations = 0;
    {

    Matrix<int> A = {{1,2}, {3,4}};
    Matrix<int> B(A);

    EXPECT_EQ(Matrix<int>::allocations, 4);   // one per row
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, ListConstructsCorrectly)
{

    Matrix<int>::allocations = 0;
    {
    Matrix<int> M = {{0, 1}, {2, 3}};

    // Test dimensions
    EXPECT_EQ(M.getSize()[0], 2);
    EXPECT_EQ(M.getSize()[1], 2);

    // Test contents
    EXPECT_EQ(M(0,0), 0);
    EXPECT_EQ(M(0,1), 1);
    EXPECT_EQ(M(1,0), 2);
    EXPECT_EQ(M(1,1), 3);


    auto make_bad_matrix = []() {
        Matrix<int> M1{{1,2}, {3}};
    };

    EXPECT_THROW(make_bad_matrix(), std::runtime_error);

    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, mVm)
{

    Matrix<int>::allocations = 0;
    {
    Matrix<int> M(2,2);
    M(0,0) = 1;
    M(0,1) = 2;
    M(1,0) = 3;
    M(1,1) = 4;

    std::vector<int> vec1= {5,6};

    std::vector<int> result = M.mVm(vec1);
    std::vector<int> ans = {17,39};
    EXPECT_EQ(result, ans);

    std::vector<int> vec2= {5,6,8};
    EXPECT_THROW(M.mVm(vec2), std::invalid_argument);

    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, deleteColumns)
{
    Matrix<int>::allocations = 0;
    {

    Matrix<int> M(4,4);
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 3; M(0,3) = 4;
    M(1,0) = 1; M(1,1) = 2; M(1,2) = 3; M(1,3) = 4;
    M(2,0) = 1; M(2,1) = 2; M(2,2) = 3; M(2,3) = 4;
    M(3,0) = 1; M(3,1) = 2; M(3,2) = 3; M(3,3) = 4;

    std::vector<size_t> colID1 = {0,2};
    Matrix<int> M1(M);

    M1.deleteColumns(colID1);
    EXPECT_EQ(M1.getSize()[0], 4);
    EXPECT_EQ(M1.getSize()[1], 2);

    Matrix<int> M1ans(4,2);
    M1ans(0,0) = 2; M1ans(0,1) = 4;
    M1ans(1,0) = 2; M1ans(1,1) = 4;
    M1ans(2,0) = 2; M1ans(2,1) = 4;
    M1ans(3,0) = 2; M1ans(3,1) = 4;


    for (size_t i = 0; i < 4; ++i){
        for (size_t j = 0; j < 2; ++j){
            EXPECT_EQ(M1(i,j), M1ans(i,j));
        }
    }

    std::vector<size_t> colID2 = {0,2,8};
    EXPECT_THROW(M.deleteColumns(colID2), std::out_of_range);

    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, deleteRows)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<int> M(4,4);
    M(0,0) = 1; M(0,1) = 1; M(0,2) = 1; M(0,3) = 1;
    M(1,0) = 2; M(1,1) = 2; M(1,2) = 2; M(1,3) = 2;
    M(2,0) = 3; M(2,1) = 3; M(2,2) = 3; M(2,3) = 3;
    M(3,0) = 4; M(3,1) = 4; M(3,2) = 4; M(3,3) = 4;

    std::vector<size_t> rowID1 = {0,2};
    Matrix<int> M1(M);

    M1.deleteRows(rowID1);
    EXPECT_EQ(M1.getSize()[0], 2);
    EXPECT_EQ(M1.getSize()[1], 4);

    Matrix<int> M1ans(2,4);
    M1ans(0,0) = 2; M1ans(0,1) = 2; M1ans(0,2) = 2; M1ans(0,3) = 2;
    M1ans(1,0) = 4; M1ans(1,1) = 4; M1ans(1,2) = 4; M1ans(1,3) = 4;



    for (size_t i = 0; i < 2; ++i){
        for (size_t j = 0; j < 4; ++j){
            EXPECT_EQ(M1(i,j), M1ans(i,j));
        }
    }

    std::vector<size_t> rowID2 = {0,2,8};
    EXPECT_THROW(M.deleteRows(rowID2), std::out_of_range);

    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, operatorEqCopiesDimensions)
{
    Matrix<int>::allocations = 0;
    {
    Matrix<int> A = {{1,2,3}, {4,5,6}};
    Matrix<int> B = {{9,9,9}, {9,9,9}};

    B = A;

    EXPECT_EQ(B.getSize()[0], 2);
    EXPECT_EQ(B.getSize()[1], 3);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, operatorEqCopiesValues)
{
    Matrix<int>::allocations = 0;
    {
    Matrix<int> A = {{1,2}, {3,4}};
    Matrix<int> B = {{0,0}, {0,0}};

    B = A;

    EXPECT_EQ(B(0,0), 1);
    EXPECT_EQ(B(0,1), 2);
    EXPECT_EQ(B(1,0), 3);
    EXPECT_EQ(B(1,1), 4);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, operatorEqDeepCopy)
{
    Matrix<int>::allocations = 0;
    {
    Matrix<int> A = {{1,2}, {3,4}};
    Matrix<int> B = {{0,0}, {0,0}};

    B = A;

    // Change B only
    B(0,0) = 99;
    B(1,1) = 88;

    // A remains the same
    EXPECT_EQ(A(0,0), 1);
    EXPECT_EQ(A(1,1), 4);

    // B got the new values
    EXPECT_EQ(B(0,0), 99);
    EXPECT_EQ(B(1,1), 88);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, operatorEqSelfAssignment)
{
    Matrix<int>::allocations = 0;
    {
    Matrix<int> A = {{10,20}, {30,40}};

    A = A;   // must not change or crash

    EXPECT_EQ(A(0,0), 10);
    EXPECT_EQ(A(0,1), 20);
    EXPECT_EQ(A(1,0), 30);
    EXPECT_EQ(A(1,1), 40);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, operatorEqAlloc)
{
    Matrix<int>::allocations = 0;
    {

    Matrix<int> A = {{1,2}, {3,4}, {6,3}};
    Matrix<int> B = {{5,6}, {7,8}};

    B = A;

    EXPECT_EQ(Matrix<int>::allocations, 6);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, operatorCross)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<int> A(2, 3);
    A(0,0) = 1; A(0,1) = 2; A(0,2) = 3;
    A(1,0) = 4; A(1,1) = 5; A(1,2) = 6;

    Matrix<int> B(3, 2);
    B(0,0) = 7;  B(0,1) = 8;
    B(1,0) = 9;  B(1,1) = 10;
    B(2,0) = 11; B(2,1) = 12;

    Matrix<int> C(2, 2);
    C(0,0) = 58; C(0,1) = 64;
    C(1,0) = 139; C(1,1) = 154;

    Matrix<int> result = A*B;
    for (size_t i = 0; i < 2; ++i){
        for (size_t j = 0; j < 2; ++j){
            EXPECT_EQ(C(i,j), result(i,j));
        }
    }

    EXPECT_ANY_THROW(A*C);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, operatorMinus)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<int> A(3, 3);
        A(0,0)=5; A(0,1)=7; A(0,2)=2;
        A(1,0)=0; A(1,1)=3; A(1,2)=1;
        A(2,0)=4; A(2,1)=6; A(2,2)=8;

        Matrix<int> B(3, 3);
        B(0,0)=1; B(0,1)=2; B(0,2)=3;
        B(1,0)=4; B(1,1)=0; B(1,2)=1;
        B(2,0)=2; B(2,1)=1; B(2,2)=5;

        Matrix<int> C = A - B;
        Matrix<int> C_expected(3,3);
        C_expected(0,0)=4;  C_expected(0,1)=5;  C_expected(0,2)=-1;
        C_expected(1,0)=-4; C_expected(1,1)=3;  C_expected(1,2)=0;
        C_expected(2,0)=2;  C_expected(2,1)=5;  C_expected(2,2)=3;

        // Check every element
        for (size_t i = 0; i < 3; ++i){
            for (size_t j = 0; j < 3; ++j){
                EXPECT_EQ(C(i,j), C_expected(i,j));
            }
        }
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, operatorPlus)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<int> A(3, 3);
        A(0,0)=1; A(0,1)=2; A(0,2)=3;
        A(1,0)=4; A(1,1)=5; A(1,2)=6;
        A(2,0)=7; A(2,1)=8; A(2,2)=9;

        Matrix<int> B(3, 3);
        B(0,0)=9;  B(0,1)=8;  B(0,2)=7;
        B(1,0)=6;  B(1,1)=5;  B(1,2)=4;
        B(2,0)=3;  B(2,1)=2;  B(2,2)=1;

        Matrix<int> C = A + B;

        Matrix<int> C_expected(3,3);
        C_expected(0,0)=10; C_expected(0,1)=10; C_expected(0,2)=10;
        C_expected(1,0)=10; C_expected(1,1)=10; C_expected(1,2)=10;
        C_expected(2,0)=10; C_expected(2,1)=10; C_expected(2,2)=10;

        // Check every element
        for (size_t i = 0; i < 3; ++i){
            for (size_t j = 0; j < 3; ++j){
                EXPECT_EQ(C(i,j), C_expected(i,j));
            }
        }
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, transposeSize)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<double> A(2, 3);

        // Fill matrix
        A(0,0) = 1; A(0,1) = 2; A(0,2) = 3;
        A(1,0) = 4; A(1,1) = 5; A(1,2) = 6;

        Matrix<double> T = A.transpose();

        // Dimensions should swap
        EXPECT_EQ(T.getSize()[0], 3);
        EXPECT_EQ(T.getSize()[1], 2);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, transposeValues)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<int> A(3, 2);
    A(0,0) = 10; A(0,1) = 20;
    A(1,0) = 30; A(1,1) = 40;
    A(2,0) = 50; A(2,1) = 60;

    Matrix<int> T = A.transpose();

    // Check a few values
    EXPECT_EQ(T(0,0), 10);
    EXPECT_EQ(T(1,0), 20);
    EXPECT_EQ(T(0,1), 30);
    EXPECT_EQ(T(1,1), 40);
    EXPECT_EQ(T(0,2), 50);
    EXPECT_EQ(T(1,2), 60);

    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, transposeSquare)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<float> A(2, 2);
    A(0,0) = 1.5f; A(0,1) = 2.5f;
    A(1,0) = 3.5f; A(1,1) = 4.5f;

    Matrix<float> T = A.transpose();

    EXPECT_FLOAT_EQ(T(0,0), 1.5f);
    EXPECT_FLOAT_EQ(T(1,0), 2.5f);
    EXPECT_FLOAT_EQ(T(0,1), 3.5f);
    EXPECT_FLOAT_EQ(T(1,1), 4.5f);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, transposeOneElement)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<double> A(1, 1);
    A(0,0) = 42.0;

    Matrix<double> T = A.transpose();

    EXPECT_EQ(T.getSize()[0], 1);
    EXPECT_EQ(T.getSize()[1], 1);
    EXPECT_DOUBLE_EQ(T(0,0), 42.0);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, transposeVector)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<int> A(1, 4);
    A(0,0) = 7;
    A(0,1) = 8;
    A(0,2) = 9;
    A(0,3) = 10;

    Matrix<int> T = A.transpose();

    // T should be 4x1
    ASSERT_EQ(T.getSize()[0], 4);
    ASSERT_EQ(T.getSize()[1], 1);

    EXPECT_EQ(T(0,0), 7);
    EXPECT_EQ(T(1,0), 8);
    EXPECT_EQ(T(2,0), 9);
    EXPECT_EQ(T(3,0), 10);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, choSPD_2x2)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<double> A(2,2);
    A(0,0)=4;  A(0,1)=2;
    A(1,0)=2;  A(1,1)=3;

    Matrix<double> L = A.cho();

    EXPECT_DOUBLE_EQ(L(0,0), 2.0);
    EXPECT_DOUBLE_EQ(L(0,1), 0.0);
    EXPECT_DOUBLE_EQ(L(1,0), 1.0);
    EXPECT_DOUBLE_EQ(L(1,1), 1.4142135623730951);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, choSPD_3x3_Strict)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<double> A(3,3);
    A(0,0)=4;   A(0,1)=12;  A(0,2)=-16;
    A(1,0)=12;  A(1,1)=37;  A(1,2)=-43;
    A(2,0)=-16; A(2,1)=-43; A(2,2)=98;

    Matrix<double> L = A.cho();

    EXPECT_DOUBLE_EQ(L(0,0), 2.0);
    EXPECT_DOUBLE_EQ(L(0,1), 0.0);
    EXPECT_DOUBLE_EQ(L(0,2), 0.0);

    EXPECT_DOUBLE_EQ(L(1,0), 6.0);
    EXPECT_DOUBLE_EQ(L(1,1), 1.0);
    EXPECT_DOUBLE_EQ(L(1,2), 0.0);

    EXPECT_DOUBLE_EQ(L(2,0), -8.0);
    EXPECT_DOUBLE_EQ(L(2,1), 5.0);
    EXPECT_DOUBLE_EQ(L(2,2), 3.0);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, choIdentityMatrix3x3)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<double> A(3,3);
    A(0,0)=1; A(0,1)=0; A(0,2)=0;
    A(1,0)=0; A(1,1)=1; A(1,2)=0;
    A(2,0)=0; A(2,1)=0; A(2,2)=1;

    Matrix<double> L = A.cho();

    EXPECT_DOUBLE_EQ(L(0,0), 1.0);
    EXPECT_DOUBLE_EQ(L(0,1), 0.0);
    EXPECT_DOUBLE_EQ(L(0,2), 0.0);

    EXPECT_DOUBLE_EQ(L(1,0), 0.0);
    EXPECT_DOUBLE_EQ(L(1,1), 1.0);
    EXPECT_DOUBLE_EQ(L(1,2), 0.0);

    EXPECT_DOUBLE_EQ(L(2,0), 0.0);
    EXPECT_DOUBLE_EQ(L(2,1), 0.0);
    EXPECT_DOUBLE_EQ(L(2,2), 1.0);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, choDiagonalSPD)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<double> A(3,3);
    A(0,0)=4;  A(0,1)=0;  A(0,2)=0;
    A(1,0)=0;  A(1,1)=9;  A(1,2)=0;
    A(2,0)=0;  A(2,1)=0;  A(2,2)=16;

    Matrix<double> L = A.cho();

    EXPECT_DOUBLE_EQ(L(0,0), 2.0);
    EXPECT_DOUBLE_EQ(L(0,1), 0.0);
    EXPECT_DOUBLE_EQ(L(0,2), 0.0);

    EXPECT_DOUBLE_EQ(L(1,0), 0.0);
    EXPECT_DOUBLE_EQ(L(1,1), 3.0);
    EXPECT_DOUBLE_EQ(L(1,2), 0.0);

    EXPECT_DOUBLE_EQ(L(2,0), 0.0);
    EXPECT_DOUBLE_EQ(L(2,1), 0.0);
    EXPECT_DOUBLE_EQ(L(2,2), 4.0);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, cho_4x4)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<double> K(4,4);
    K(0,0)=4.718;  K(0,1)=0.282; K(0,2)=0.2; K(0,3)=0.326;
    K(1,0)=0.282; K(1,1)=4.135;  K(1,2)=0.052;  K(1,3)=0.0;
    K(2,0)=0.2; K(2,1)=0.052;  K(2,2)=4.002;  K(2,3)=0.129;
    K(3,0)=0.326;  K(3,1)=0.0; K(3,2)=0.129; K(3,3)=4.219;

    Matrix<double> L = K.cho();

    EXPECT_NEAR(L(0,0), 2.172, 1E-3);
    EXPECT_NEAR(L(0,1), 0.0, 1E-3);
    EXPECT_NEAR(L(0,2), 0.0, 1E-3);
    EXPECT_NEAR(L(0,3), 0.0, 1E-3);

    EXPECT_NEAR(L(1,0), 0.130, 1E-3);
    EXPECT_NEAR(L(1,1), 2.029, 1E-3);
    EXPECT_NEAR(L(1,2), 0.0, 1E-3);
    EXPECT_NEAR(L(1,3), 0.0, 1E-3);

    EXPECT_NEAR(L(2,0), 0.092, 1E-3);
    EXPECT_NEAR(L(2,1), 0.020, 1E-3);
    EXPECT_NEAR(L(2,2), 1.998, 1E-3);
    EXPECT_NEAR(L(2,3), 0.0, 1E-3);

    EXPECT_NEAR(L(3,0), 0.150, 1E-3);
    EXPECT_NEAR(L(3,1), -0.009, 1E-3);
    EXPECT_NEAR(L(3,2), 0.058, 1E-3);
    EXPECT_NEAR(L(3,3), 2.048, 1E-3);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, BasicAddRows)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<int> A(2,3);
    A(0,0)=1; A(0,1)=2; A(0,2)=3;
    A(1,0)=4; A(1,1)=5; A(1,2)=6;

    Matrix<int> B(2,3);
    B(0,0)=7;  B(0,1)=8;  B(0,2)=9;
    B(1,0)=10; B(1,1)=11; B(1,2)=12;

    A.addRows(B);

    EXPECT_EQ(A.getSize()[0], 4);
    EXPECT_EQ(A.getSize()[1], 3);

    EXPECT_EQ(A(0,0), 1); EXPECT_EQ(A(1,2), 6);
    EXPECT_EQ(A(2,0), 7); EXPECT_EQ(A(3,2), 12);

    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, AddEmptyMatrix)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<int> A(2,3);
    A(0,0)=1; A(0,1)=2; A(0,2)=3;
    A(1,0)=4; A(1,1)=5; A(1,2)=6;

    Matrix<int> B(0,3); // empty rows

    A.addRows(B);

    EXPECT_EQ(A.getSize()[0], 2);
    EXPECT_EQ(A.getSize()[1], 3);
    EXPECT_EQ(A(1,2), 6); // original rows remain intact

    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, SelfAddition)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<int> A(2,2);
    A(0,0)=1; A(0,1)=2;
    A(1,0)=3; A(1,1)=4;

    A.addRows(A); // appending itself

    EXPECT_EQ(A.getSize()[0], 4);
    EXPECT_EQ(A.getSize()[1], 2);

    // Original rows preserved
    EXPECT_EQ(A(0,0), 1); EXPECT_EQ(A(0,1), 2);
    EXPECT_EQ(A(1,0), 3); EXPECT_EQ(A(1,1), 4);

    // Appended rows same as original
    EXPECT_EQ(A(2,0), 1); EXPECT_EQ(A(2,1), 2);
    EXPECT_EQ(A(3,0), 3); EXPECT_EQ(A(3,1), 4);

    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, LargeMatrixAddition)
{

    Matrix<int>::allocations = 0;
    {

    const int ROWS = 1000, COLS = 500;

    Matrix<int> A(ROWS, COLS);
    Matrix<int> B(ROWS, COLS);

    // Fill A and B with deterministic values
    for(int i=0;i<ROWS;++i){
        for(int j=0;j<COLS;++j){
            A(i,j) = i + j;
            B(i,j) = i - j;
        }
    }
    A.addRows(B);

    EXPECT_EQ(A.getSize()[0], 2*ROWS);
    EXPECT_EQ(A.getSize()[1], COLS);

    // Check a few random values
    EXPECT_EQ(A(0,0), 0);
    EXPECT_EQ(A(ROWS,0), 0); // first row of B appended
    EXPECT_EQ(A(ROWS-1,COLS-1), ROWS-1 + COLS-1);
    EXPECT_EQ(A(2*ROWS-1,COLS-1), ROWS-1 - (COLS-1));

    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};

TEST(MatrixLibTest, ColumnMismatchThrows)
{

    Matrix<int>::allocations = 0;
    {

    Matrix<int> A(2,3);
    Matrix<int> B(2,4); // wrong columns
    EXPECT_THROW(A.addRows(B), std::invalid_argument);
    }
    EXPECT_EQ(Matrix<int>::allocations, 0);
};
