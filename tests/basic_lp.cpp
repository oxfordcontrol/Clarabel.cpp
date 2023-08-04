#include <Clarabel>
#include <Eigen/Eigen>
#include <cmath>
#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;
using namespace clarabel;
using namespace Eigen;

class BasicLPTest : public ::testing::Test
{
  protected:
    SparseMatrix<double> P, A;
    double c[3] = { 3., -2., 1. };
    double b[6] = { 1., 1., 1., 1., 1., 1. };
    vector<SupportedConeT<double>> cones = {
        NonnegativeConeT<double>(3),
        NonnegativeConeT<double>(3)
    };

    BasicLPTest()
    {
        P = MatrixXd::Zero(3, 3).sparseView();
        P.makeCompressed();

        // A = [I; -I]
        MatrixXd I1 = MatrixXd::Identity(3, 3);
        MatrixXd I2 = -MatrixXd::Identity(3, 3);
        MatrixXd A_dense = MatrixXd::Zero(6, 3);
        A_dense << I1,
            I2;
        A_dense *= 2.0;
        A = A_dense.sparseView();
        A.makeCompressed();
    }
};

TEST_F(BasicLPTest, Feasible)
{
    DefaultSettings<double> settings = DefaultSettingsBuilder<double>::default_settings().build();

    clarabel::eigen::DefaultSolver<double> solver(P, c, A, b, cones, &settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::Solved);

    // Check the solution
    VectorXd ref_solution(3);
    ref_solution << -0.5, 0.5, -0.5;

    ASSERT_EQ(solution.x_length, 3);
    VectorXd actual_solution(3);
    for (int i = 0; i < 3; ++i)
    {
        cout << solution.x[i] << endl;
        actual_solution[i] = solution.x[i];
    }
    ASSERT_TRUE(actual_solution.isApprox(ref_solution, 1e-8));

    double ref_obj = -3.0;
    ASSERT_NEAR(solver.info().cost_primal, ref_obj, 1e-8);
}

TEST_F(BasicLPTest, PrimalInfeasible)
{
    A.valuePtr()[1] = 1.0; // swap lower bound on first variable to redundant upper bound

    c[0] = 1.0;
    c[1] = 0.0;
    c[2] = 0.0;

    DefaultSettings<double> settings = DefaultSettingsBuilder<double>::default_settings().build();

    clarabel::eigen::DefaultSolver<double> solver(P, c, A, b, cones, &settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::DualInfeasible);
}

TEST_F(BasicLPTest, InfeasibleIllCond)
{
    double *A_nzval = A.valuePtr();
    A_nzval[0] = numeric_limits<double>::epsilon();
    A_nzval[1] = -numeric_limits<double>::epsilon();

    c[0] = 1.0;
    c[1] = 0.0;
    c[2] = 0.0;

    DefaultSettings<double> settings = DefaultSettingsBuilder<double>::default_settings().build();

    clarabel::eigen::DefaultSolver<double> solver(P, c, A, b, cones, &settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::DualInfeasible);
}