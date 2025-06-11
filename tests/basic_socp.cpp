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

class BasicSOCPTest : public testing::Test
{
  protected:
    SparseMatrix<double> P, A;
    Vector<double, 3> c = {0.1, -2.0, 1.0};
    Vector<double, 9> b = {1., 1., 1., 1., 1., 1., 0., 0., 0. };
    vector<SupportedConeT<double>> cones = {
        NonnegativeConeT<double>(3),
        NonnegativeConeT<double>(3),
        SecondOrderConeT<double>(3),
    };
    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    BasicSOCPTest()
    {
        int colptr[] = { 0, 3, 6, 9 }; // start index per column + 1 for last col
        int rowval[] = { 0, 1, 2, 0, 1, 2, 0, 1, 2 }; // nonzero row indices
        double nzval[] = {1.4652521089139698,
                0.6137176286085666,
                -1.1527861771130112,
                0.6137176286085666,
                2.219109946678485,
                -1.4400420548730628,
                -1.1527861771130112,
                -1.4400420548730628,
                1.6014483534926371};
        P = SparseMatrix<double>::Map(
                3 /*rows*/, 3 /*cols*/, 9 /*nonzeros*/, colptr,rowval, nzval
            );
        MatrixXd I1 =  MatrixXd::Identity(3, 3);
        MatrixXd I2 = -MatrixXd::Identity(3, 3);
        MatrixXd I3 = 0.5*MatrixXd::Identity(3, 3);
        MatrixXd A_dense = MatrixXd::Zero(9, 3);
        A_dense << I1,
                   I2,
                   I3;
        A_dense *= 2.0;
        A = A_dense.sparseView();
        A.makeCompressed();
    }
};


TEST_F(BasicSOCPTest, Feasible)
{
    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::Solved);

    // Check the solution
    Vector3d ref_solution{ -0.5, 0.435603, -0.245459 };
    ASSERT_EQ(solution.x.size(), 3);
    ASSERT_TRUE(solution.x.isApprox(ref_solution, 1e-4));

    double ref_obj = -8.4590e-01;
    ASSERT_NEAR(solution.obj_val, ref_obj, 1e-4);
    ASSERT_NEAR(solution.obj_val_dual, ref_obj, 1e-4);
}

TEST_F(BasicSOCPTest, PrimalFeasible)
{
    b[6] = -10.;

    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::PrimalInfeasible);

    EXPECT_TRUE(isnan(solution.obj_val));
    EXPECT_TRUE(isnan(solution.obj_val_dual));
}