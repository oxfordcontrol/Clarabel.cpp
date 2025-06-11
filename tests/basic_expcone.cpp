#include <clarabel.hpp>
#include <Eigen/Eigen>
#include <cmath>
#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;
using namespace clarabel;
using namespace Eigen;

class BasicExpConeTest : public testing::Test
{
    // produces data for the following exponential cone problem
    // max  x
    // s.t. y * exp(x / y) <= z
    //      y == 1, z == exp(5)
  protected:
    SparseMatrix<double> P, A;
    Vector<double, 3> c = {-1., 0., 0.};
    Vector<double, 5> b = {0., 0., 0., 1.0, exp(5.)};
    vector<SupportedConeT<double>> cones = {
        ExponentialConeT<double>(),
        ZeroConeT<double>(2),
    };
    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    BasicExpConeTest()
    {
        P = MatrixXd::Zero(3, 3).sparseView();
        P.makeCompressed();

        MatrixXd A1 = -MatrixXd::Identity(3,3);
        MatrixXd A2(2,3);
        A2 << 0, 1, 0, // y = 1
              0, 0, 1; // z = exp(5)

        MatrixXd A_dense = MatrixXd::Zero(5,3);
        A_dense << A1, A2;
        A = A_dense.sparseView();
        A.makeCompressed();
    }
};

TEST_F(BasicExpConeTest, Feasible)
{
    // solve the following exponential cone problem
    // max  x
    // s.t. y * exp(x / y) <= z
    //      y == 1, z == exp(5)
    //
    // This is just the default problem data above
    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::Solved);

    // Check the solution
    Vector3d ref_solution{ 5.0, 1.0, exp(5.0) };
    ASSERT_EQ(solution.x.size(), 3);
    ASSERT_TRUE(solution.x.isApprox(ref_solution, 1e-6));

    double ref_obj = -5.0;
    ASSERT_NEAR(solution.obj_val, ref_obj, 1e-6);
    ASSERT_NEAR(solution.obj_val_dual, ref_obj, 1e-6);
}

TEST_F(BasicExpConeTest, PrimalInfeasible)
{
    // solve the following exponential cone problem
    // max  x
    // s.t. y * exp(x / y) <= z
    //      y == 1, z == -1
    //
    // Same as default, but last element of b is different
    b[4] = -1.;

    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::PrimalInfeasible);
}


TEST_F(BasicExpConeTest, DualInfeasible)
{
    // solve the following exponential cone problem
    // max  x
    // s.t. y * exp(x / y) <= z
    //
    // Same as default, but no equality constraint

    Vector<double, 3> b = {0., 0., 0.};
    vector<SupportedConeT<double>> cones = {ExponentialConeT<double>()};
    MatrixXd A1 = -MatrixXd::Identity(3,3);
    SparseMatrix<double> A = A1.sparseView();
    A.makeCompressed();

    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::DualInfeasible);
}