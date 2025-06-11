#include <clarabel.hpp>
#include <Eigen/Eigen>
#include <Eigen/src/Core/Matrix.h>
#include <cmath>
#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;
using namespace clarabel;
using namespace Eigen;

class DimensionChecksTest : public ::testing::Test
{
  protected:
    SparseMatrix<double> P, A;
    Vector<double, 4> q = Vector<double, 4>::Zero();
    Vector<double, 6> b = Vector<double, 6>::Zero();

    vector<SupportedConeT<double>> cones = {
        ZeroConeT<double>(1),
        NonnegativeConeT<double>(2),
        NonnegativeConeT<double>(3)
    };

    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    DimensionChecksTest()
    {
        P = MatrixXd::Zero(4, 4).sparseView();
        P.makeCompressed();

        A = MatrixXd::Zero(6, 4).sparseView();
        A.makeCompressed();
    }
};

TEST_F(DimensionChecksTest, Working)
{
    // This example should work because dimensions are
    // all compatible. All following checks vary one
    // of these sizes to test dimension checks
    DefaultSolver<double> solver(P, q, A, b, cones, settings);
}

TEST_F(DimensionChecksTest, BadP)
{
    P = MatrixXd::Zero(3, 3).sparseView();
    P.makeCompressed();
    
    ASSERT_THROW(DefaultSolver<double> solver(P, q, A, b, cones, settings), std::invalid_argument);
}

TEST_F(DimensionChecksTest, BadARows)
{
    A = MatrixXd::Zero(5, 4).sparseView();
    A.makeCompressed();

    ASSERT_THROW(DefaultSolver<double> solver(P, q, A, b, cones, settings), std::invalid_argument);
}

TEST_F(DimensionChecksTest, BadACols)
{
    A = MatrixXd::Zero(6, 3).sparseView();
    A.makeCompressed();

    ASSERT_THROW(DefaultSolver<double> solver(P, q, A, b, cones, settings), std::invalid_argument);
}

TEST_F(DimensionChecksTest, PNotSquare)
{
    P = MatrixXd::Zero(4, 3).sparseView();
    P.makeCompressed();

    ASSERT_THROW(DefaultSolver<double> solver(P, q, A, b, cones, settings), std::invalid_argument);
}

TEST_F(DimensionChecksTest, BadCones)
{
    cones = {
        ZeroConeT<double>(1),
        NonnegativeConeT<double>(2),
        NonnegativeConeT<double>(4)
    };

    ASSERT_THROW(DefaultSolver<double> solver(P, q, A, b, cones, settings), std::invalid_argument);
}