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

SparseMatrix<double> create_A() {
    // A =
    //[ 0. 1.  1.;
    //  0. 1. -1.]
    int colptr[] = { 0, 1, 4, 5, 8, 9, 10, 13, 16 }; // start index per column + 1 for last col
    int rowval[] = { 24, 7, 10, 22, 8, 12, 15, 25, 9, 13, 18, 21, 26, 0, 23, 27 }; // nonzero row indices
    double nzval[] = {             -1.0,
            -M_SQRT2,
            -1.0,
            -1.0,
            -M_SQRT2,
            -M_SQRT2,
            -1.0,
            -1.0,
            -M_SQRT2,
            -M_SQRT2,
            -M_SQRT2,
            -1.0,
            -1.0,
            -1.0,
            -1.0,
            -1.0};
    return SparseMatrix<double>::Map(
        28 /*rows*/, 8 /*cols*/, 16 /*nonzeros*/, colptr, rowval, nzval);
}

class SDPChordalTest : public ::testing::Test
{
  protected:
    SparseMatrix<double> P, A;
    
    Vector<double, 28> b = {0.0,
                            3.0,
                            2. * M_SQRT2,
                            2.0,
                            M_SQRT2,
                            M_SQRT2,
                            3.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            0.0,};
    Vector<double, 8> c = {-1.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0};
    vector<SupportedConeT<double>> cones = {
        NonnegativeConeT<double>(1),
        PSDTriangleConeT<double>(6),
        PowerConeT<double>(0.3333333333333333),
        PowerConeT<double>(0.5),
    };

    SDPChordalTest()
    {
        MatrixXd P_dense = MatrixXd::Zero(8,8);
        P = P_dense.sparseView();
        P.makeCompressed();

        A = create_A();

    }
};

TEST_F(SDPChordalTest, Feasible)
{
    DefaultSettings<double> settings = DefaultSettingsBuilder<double>::default_settings()
        .verbose(true)
        .chordal_decomposition_enable(true)
        .chordal_decomposition_compact(true)
        .chordal_decomposition_complete_dual(true)
        .chordal_decomposition_merge_method(ClarabelCliqueMergeMethods::CLIQUE_GRAPH)
        .max_iter(50)
        .build();

    Vector<bool, 2> compacts = {false, true};
    Vector<bool, 2> complete_duals = {false, true};
    Vector<ClarabelCliqueMergeMethods, 3> merge_methods = {ClarabelCliqueMergeMethods::CLIQUE_GRAPH,
                                                                 ClarabelCliqueMergeMethods::PARENT_CHILD,
                                                                 ClarabelCliqueMergeMethods::NONE};

    for (auto &compact : compacts){
        for (auto &complete_dual : complete_duals){
            for (auto &merge_method : merge_methods){
                settings.chordal_decomposition_compact = compact;
                settings.chordal_decomposition_complete_dual = complete_dual;
                settings.chordal_decomposition_merge_method = merge_method;
                DefaultSolver<double> solver(P, c, A, b, cones, settings);
                solver.solve();

                DefaultSolution<double> solution = solver.solution();
                ASSERT_EQ(solution.status, SolverStatus::Solved);
            }
        }
    }
}
#endif