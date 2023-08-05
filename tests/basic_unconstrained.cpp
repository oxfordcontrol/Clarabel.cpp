#include <Clarabel>
#include <Eigen/Eigen>
#include <Eigen/src/Core/Matrix.h>
#include <Eigen/src/Core/util/Constants.h>
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

    double c[3] = { 1., 2., -3. };

    SparseMatrix<double> A = MatrixXd::Zero(0, 3).sparseView();
    A.makeCompressed();

    double b[0] = {};

    vector<SupportedConeT<double>> cones = {};

    DefaultSettings<double> settings = DefaultSettingsBuilder<double>::default_settings().build();

    clarabel::eigen::DefaultSolver<double> solver(P, c, A, b, cones, &settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::Solved);

    // Compare the solution to the reference solution
    VectorXd ref_solution(3);
    ref_solution << -1., -2., 3.;
    VectorXd actual_solution(3);
    for (int i = 0; i < 3; ++i)
    {
        actual_solution[i] = solution.x[i];
    }
    ASSERT_TRUE(actual_solution.isApprox(ref_solution, 1e-6));
}

TEST(BasicUnconstrainedTest, Infeasible)
{
    SparseMatrix<double> P = MatrixXd::Zero(3, 3).sparseView();
    P.makeCompressed();

    double c[3] = { 1., 0., 0. };

    SparseMatrix<double> A = MatrixXd::Zero(0, 3).sparseView();
    A.makeCompressed();

    double b[0] = {};

    vector<SupportedConeT<double>> cones = {};

    DefaultSettings<double> settings = DefaultSettingsBuilder<double>::default_settings().build();

    clarabel::eigen::DefaultSolver<double> solver(P, c, A, b, cones, &settings);
    solver.solve();

    ASSERT_EQ(solver.solution().status, SolverStatus::DualInfeasible);
}