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

SparseMatrix<double> eq_constrained_A1() {
    // A =
    //[ 0. 1.  1.;
    //  0. 1. -1.]
    int colptr[] = { 0, 0, 2, 4 }; // start index per column + 1 for last col
    int rowval[] = { 0, 1, 0, 1 }; // nonzero row indices
    double nzval[] = { 1., 1., 1., -1. };
    return SparseMatrix<double>::Map(
        2 /*rows*/, 3 /*cols*/, 4 /*nonzeros*/, colptr, rowval, nzval);
}

SparseMatrix<double> eq_constrained_A2() {
    // A = [
    // 0    1.0   1.0;
    // 0    1.0  -1.0;
    //1.0   2.0  -1.0l
    //2.0  -1.0   3.0l
    //]
    int colptr[] = { 0, 2, 6, 10 }; // start index per column + 1 for last col
    int rowval[] = { 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}; // nonzero row indices
    double nzval[] = {1., 2., 1., 1., 2., -1., 1., -1., -1., 3. };
    return SparseMatrix<double>::Map(
        4 /*rows*/, 3 /*cols*/, 10 /*nonzeros*/, colptr, rowval, nzval);
}

TEST(BasicEqConstrainedTest, Feasible)
{
    SparseMatrix<double> P = MatrixXd::Identity(3, 3).sparseView();
    P.makeCompressed();

    Vector<double, 3> c = { 0., 0., 0. };

    SparseMatrix<double> A = eq_constrained_A1();

    Vector<double, 2> b = {2., 0.};

    vector<SupportedConeT<double>> cones = {ZeroConeT<double>(2)};

    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::Solved);

    // Compare the solution to the reference solution
    Vector3d ref_solution{ 0., 1., 1. };
    ASSERT_EQ(solution.x.size(), 3);
    ASSERT_TRUE(solution.x.isApprox(ref_solution, 1e-6));
}

TEST(BasicEqConstrainedTest, PrimalInfeasible)
{
    SparseMatrix<double> P = MatrixXd::Identity(3, 3).sparseView();
    P.makeCompressed();

    Vector<double, 3> c = { 0., 0., 0. };

    SparseMatrix<double> A = eq_constrained_A2();

    Vector<double, 4> b = {1., 1., 1., 1.};

    vector<SupportedConeT<double>> cones = {ZeroConeT<double>(4)};

    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::PrimalInfeasible);
}

TEST(BasicEqConstrainedTest, DualInfeasible)
{
    SparseMatrix<double> P = MatrixXd::Identity(3, 3).sparseView();
    P.makeCompressed();
    P.coeffRef(0,0) = 0.0;

    Vector<double, 3> c = { 1., 1., 1. };

    SparseMatrix<double> A = eq_constrained_A1();

    Vector<double, 2> b = {2., 0.};

    vector<SupportedConeT<double>> cones = {ZeroConeT<double>(2)};

    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::DualInfeasible);
}