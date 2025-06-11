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

TEST(BasicGenPowerConeTest, Feasible)
{
    // solve the following power cone problem
    // max  x1^0.6 y^0.4 + x2^0.1
    // s.t. x1, y, x2 >= 0
    //      x1 + 2y  + 3x2 == 3
    // which is equivalent to
    // max z1 + z2
    // s.t. (x1, y, z1) in K_pow(0.6)
    //      (x2, 1, z2) in K_pow(0.1)
    //      x1 + 2y + 3x2 == 3

    // x = (x1, y, z1, x2, y2, z2)

    SparseMatrix<double> P = MatrixXd::Zero(6, 6).sparseView();
    P.makeCompressed();
    
    Vector<double, 6> c = { 0., 0., -1., 0., 0., -1. };

    // Assembling A
    MatrixXd A1_dense = -MatrixXd::Identity(6, 6);
    MatrixXd A2_dense(2,6);
    A2_dense << 1., 2., 0., 3., 0., 0., //
                0., 0., 0., 0., 1., 0.; //

    MatrixXd A_dense = MatrixXd::Zero(8,6);
    A_dense << A1_dense, A2_dense;
    SparseMatrix<double> A = A_dense.sparseView();
    A.makeCompressed();

    // Assembling b
    Vector<double, 8> b = {0., 0., 0., 0., 0., 0., 3., 1.};
    // Assembling cones

    Vector<double, 2> cone1 = {0.6, 0.4};
    Vector<double, 2> cone2 = {0.1, 0.9};
    vector<SupportedConeT<double>> cones = {GenPowerConeT<double>(cone1, 1), 
                                            GenPowerConeT<double>(cone2, 1), 
                                            ZeroConeT<double>(2)};

    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::Solved);

    // Compare the solution to the reference solution
    double ref_obj = -1.8458;
    ASSERT_NEAR(solution.obj_val, ref_obj, 1e-3);
}