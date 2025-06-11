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

class GetInfoTest : public ::testing::Test
{
  protected:
    SparseMatrix<double> P, A;
    Vector<double, 2> c = { 1., 1. };
    Vector<double, 6> b = { -1., 0., 0., 1., 0.7, 0.7 };
    vector<SupportedConeT<double>> cones = {
        NonnegativeConeT<double>(3),
        NonnegativeConeT<double>(3)
    };
    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    GetInfoTest()
    {
        MatrixXd P_dense(2, 2);
        P_dense << 4., 1.,
            1., 2.;
        P = P_dense.sparseView();
        P.makeCompressed();

        MatrixXd A_dense(6, 2);
        A_dense <<
            -1., -1.,
            -1., 0.,
            0., -1.,
            1., 1.,
            1., 0.,
            0., 1.;
        A = A_dense.sparseView();
        A.makeCompressed();
    }
};


TEST_F(GetInfoTest, Feasible)
{
    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();
    auto info = solver.info();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::Solved);

    // check the linear solver reporting 
    ASSERT_EQ(info.linsolver.name, ClarabelDirectSolveMethods::QDLDL);
    ASSERT_EQ(info.linsolver.threads, 1);
    ASSERT_EQ(info.linsolver.direct, true);
    ASSERT_EQ(info.linsolver.nnzA, 17);
    ASSERT_EQ(info.linsolver.nnzL, 9);
}