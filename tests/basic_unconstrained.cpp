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

TEST(BasicUnconstrainedTest, Feasible)
{
    SparseMatrix<double> P = MatrixXd::Identity(3, 3).sparseView();
    P.makeCompressed();

    Vector<double, 3> c = { 1., 2., -3. };

    SparseMatrix<double> A = MatrixXd::Zero(0, 3).sparseView();
    A.makeCompressed();

    Vector<double, 0> b = {};

    vector<SupportedConeT<double>> cones = {};

    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::Solved);

    // Compare the solution to the reference solution
    Vector3d ref_solution{ -1., -2., 3. };
    ASSERT_EQ(solution.x.size(), 3);
    ASSERT_TRUE(solution.x.isApprox(ref_solution, 1e-6));
}

TEST(BasicUnconstrainedTest, Infeasible)
{
    SparseMatrix<double> P = MatrixXd::Zero(3, 3).sparseView();
    P.makeCompressed();

    Vector<double, 3> c = { 1., 0., 0. };

    SparseMatrix<double> A = MatrixXd::Zero(0, 3).sparseView();
    A.makeCompressed();

    Vector<double, 0> b = {};

    vector<SupportedConeT<double>> cones = {};

    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    ASSERT_EQ(solver.solution().status, SolverStatus::DualInfeasible);
}