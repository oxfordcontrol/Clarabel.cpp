#ifdef FEATURE_SDP
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

class BasicSDPTest : public testing::Test
{
  protected:
    SparseMatrix<double> P, A;
    Vector<double, 6> c = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    Vector<double, 6> b = {-3., 1., 4., 1., 2., 5. };
    vector<SupportedConeT<double>> cones = {
        PSDTriangleConeT<double>(3),
    };
    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    BasicSDPTest()
    {
        MatrixXd P_dense = MatrixXd::Identity(6,6);
        P = P_dense.sparseView();
        P.makeCompressed();

        MatrixXd A_dense = MatrixXd::Identity(6,6);
        A = A_dense.sparseView(); 
        A.makeCompressed();
    }
};


TEST_F(BasicSDPTest, Feasible)
{
    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::Solved);

    // Check the solution
    Vector<double, 6>  ref_solution = {-3.0729833267361095,
                                        0.3696004167288786,
                                        -0.022226685581313674,
                                        0.31441213129613066,
                                        -0.026739700851545107,
                                        -0.016084530571308823};
    ASSERT_EQ(solution.x.size(), 6);
    ASSERT_TRUE(solution.x.isApprox(ref_solution, 1e-4));

    double ref_obj = 4.840076866013861;
    ASSERT_NEAR(solution.obj_val, ref_obj, 1e-6);
}

TEST_F(BasicSDPTest, EmptyCone)
{
    vector<SupportedConeT<double>> empty_cone = {
        PSDTriangleConeT<double>(0),
    };

    cones.insert(cones.end(), empty_cone.begin(), empty_cone.end());

    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::Solved);

    // Check the solution
    Vector<double, 6>  ref_solution = {-3.0729833267361095,
                                        0.3696004167288786,
                                        -0.022226685581313674,
                                        0.31441213129613066,
                                        -0.026739700851545107,
                                        -0.016084530571308823,};
    ASSERT_EQ(solution.x.size(), 6);
    ASSERT_TRUE(solution.x.isApprox(ref_solution, 1e-4));

    double ref_obj = 4.840076866013861;
    ASSERT_NEAR(solution.obj_val, ref_obj, 1e-6);
}

TEST_F(BasicSDPTest, PrimalInfeasible)
{
    MatrixXd I1 = MatrixXd::Identity(6, 6);
    MatrixXd I2 = -MatrixXd::Identity(6, 6);
    MatrixXd A_dense = MatrixXd::Zero(12, 6);
    A_dense << I1,
             I2;
    A = A_dense.sparseView();
    A.makeCompressed();

    vector<SupportedConeT<double>> extra_cone = {
        PSDTriangleConeT<double>(3),
    };
    cones.insert(cones.end(), extra_cone.begin(), extra_cone.end());

    Vector<double, 12> b = {-3., 1., 4., 1., 2., 5. , 0., 0., 0., 0., 0., 0.};

    DefaultSolver<double> solver(P, c, A, b, cones, settings);
    solver.solve();

    DefaultSolution<double> solution = solver.solution();
    ASSERT_EQ(solution.status, SolverStatus::PrimalInfeasible);
}
#endif