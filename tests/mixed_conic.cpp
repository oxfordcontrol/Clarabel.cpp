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

TEST(MixedConicTest, Feasible)
{
    // solves a problem with a mix of symmetric and asymmetric
    // cones. This exercises the barrier methods and unit
    // initializations of the symmetric cones
    
    const int n = 3;
    MatrixXd I = MatrixXd::Identity(n, n);

    SparseMatrix<double> P = MatrixXd::Identity(n, n).sparseView();
    P.makeCompressed();

    Vector<double, 3> c = { 1., 1., 1. };

    MatrixXd A_dense = MatrixXd::Zero(5 * n, n);
    A_dense <<
        I,
        I,
        I,
        I,
        I;
    SparseMatrix<double> A = A_dense.sparseView();
    A.makeCompressed();

    Vector<double, 5 * n> b = Vector<double, 5 * n>::Zero();

    // put a 3 dimensional vector into the composition of multiple
    // cones, all with b = 0 on the RHS
    vector<SupportedConeT<double>> cones = {
        ZeroConeT<double>(3),
        NonnegativeConeT<double>(3),
        SecondOrderConeT<double>(3),
        PowerConeT<double>(0.5),
        ExponentialConeT<double>(),
    };

    DefaultSettings<double> settings = DefaultSettingsBuilder<double>::default_settings().build();

    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    ASSERT_EQ(solver.solution().status, SolverStatus::Solved);
    ASSERT_NEAR(solver.info().cost_primal, 0., 1e-8);
}