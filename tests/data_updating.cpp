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

class DataUpdatingTest : public ::testing::Test
{
  protected:
    SparseMatrix<double> P, A;
    Vector<double, 2> q = { 1., 1. };
    Vector<double, 4> b = { 1., 1., 1., 1., };
    vector<SupportedConeT<double>> cones = {
        NonnegativeConeT<double>(2),
        NonnegativeConeT<double>(2)
    };
    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    DataUpdatingTest()
    {
        MatrixXd P_dense(2, 2);
        P_dense << 4., 1.,
            0., 2.;
        P = P_dense.sparseView();
        P.makeCompressed();

        MatrixXd A_dense(4, 2);
        A_dense <<
            -1., 0.,
            0., -1.,
            1., 0.,
            0., 1.;
        A = A_dense.sparseView();
        A.makeCompressed();
        settings.presolve_enable = false;
    }
};

TEST_F(DataUpdatingTest, update_P_csc)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change P and re-solve
    SparseMatrix<double> P2 = P;
    P2.valuePtr()[0] = 100.;

    // revised original solver
    solver1.update_P(P2);
    solver1.solve();

    //new solver
    DefaultSolver<double> solver2(P2, q, A, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_P_vector_form)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change P and re-solve
    SparseMatrix<double> P2 = P;
    P2.valuePtr()[0] = 100.;

    // revised original solver
    solver1.update_P(P2.valuePtr(), P2.nonZeros());
    solver1.solve();

    //new solver
    DefaultSolver<double> solver2(P2, q, A, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_P_vector_form_eigen)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // revised original solver
    VectorXd Pdata = Map<const VectorXd>(P.valuePtr(), P.nonZeros());
    Pdata[0] = 100.;
    solver1.update_P(Pdata);
    solver1.solve();

    //new solver
    SparseMatrix<double> P2 = P;
    P2.valuePtr()[0] = 100.;
    DefaultSolver<double> solver2(P2, q, A, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_P_partial)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change P and re-solve
    Vector<uintptr_t, 2> index = {1, 2 };
    Vector<double, 2> values = { 3., 5.};
    solver1.update_P(index.data(), values.data(), index.size());
    solver1.solve();

    //new solver
    SparseMatrix<double> P2 = P;
    P2.valuePtr()[1] = 3.;
    P2.valuePtr()[2] = 5.;
    DefaultSolver<double> solver2(P2, q, A, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_P_partial_eigen)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change P and re-solve
    Vector<uintptr_t, 2> index = {1, 2 };
    Vector<double, 2> values = { 3., 5.};
    solver1.update_P(index, values);
    solver1.solve();

    //new solver
    SparseMatrix<double> P2 = P;
    P2.valuePtr()[1] = 3.;
    P2.valuePtr()[2] = 5.;
    DefaultSolver<double> solver2(P2, q, A, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_A_csc)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change A and re-solve
    SparseMatrix<double> A2 = A;
    A2.valuePtr()[2] = -100.;

    // revised original solver
    solver1.update_A(A2);
    solver1.solve();

    //new solver
    DefaultSolver<double> solver2(P, q, A2, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}


TEST_F(DataUpdatingTest, update_A_vector_form)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change A and re-solve
    SparseMatrix<double> A2 = A;
    A2.valuePtr()[0] = -1000.;

    // revised original solver
    solver1.update_A(A2.valuePtr(), A2.nonZeros());
    solver1.solve();

    //new solver
    DefaultSolver<double> solver2(P, q, A2, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_A_vector_form_eigen)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // revised original solver
    VectorXd Adata = Map<const VectorXd>(A.valuePtr(), A.nonZeros());
    Adata[0] = -1000.;
    solver1.update_A(Adata);
    solver1.solve();

    //new solver
    SparseMatrix<double> A2 = A;
    A2.valuePtr()[0] = -1000.;
    DefaultSolver<double> solver2(P, q, A2, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_A_partial)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change A and re-solve
    Vector<uintptr_t, 2> index = {1, 2 };
    Vector<double, 2> values = { 0.5, -0.5};
    solver1.update_A(index.data(), values.data(), index.size());
    solver1.solve();

    //new solver
    SparseMatrix<double> A2 = A;
    A2.valuePtr()[1] = 0.5;
    A2.valuePtr()[2] = -0.5;
    DefaultSolver<double> solver2(P, q, A2, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_A_partial_eigen)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change A and re-solve
    Vector<uintptr_t, 2> index = {1, 2 };
    Vector<double, 2> values = { 0.5, -0.5};
    solver1.update_A(index, values);
    solver1.solve();

    //new solver
    SparseMatrix<double> A2 = A;
    A2.valuePtr()[1] = 0.5;
    A2.valuePtr()[2] = 0.5;
    DefaultSolver<double> solver2(P, q, A2, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}




TEST_F(DataUpdatingTest, update_q)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change q and re-solve
    Vector<double,2> q2 = q;
    q2[1] = 1000.;

    // revised original solver
    solver1.update_q(q2.data(), q2.size());
    solver1.solve();

    //new solver
    DefaultSolver<double> solver2(P, q2, A, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_q_eigen)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // revised original solver
    Vector<double,2> q2 = q;
    q2[1] = 1000.;
    solver1.update_q(q2);
    solver1.solve();

    //new solver
    DefaultSolver<double> solver2(P, q2, A, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_q_partial)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change q and re-solve
    Vector<double,1> values(1000.);
    Vector<uintptr_t,1>  index(1);
    solver1.update_q(index.data(), values.data(), index.size());
    solver1.solve();

    //new solver
    Vector<double,2> q2 = q;
    q2[1] = 1000.;
    DefaultSolver<double> solver2(P, q2, A, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_q_partial_eigen)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change q and re-solve
    Vector<double,1> values(1000.);
    Vector<uintptr_t,1>  index(1);
    solver1.update_q(index, values);
    solver1.solve();

    //new solver
    Vector<double,2> q2 = q;
    q2[1] = 1000.;
    DefaultSolver<double> solver2(P, q2, A, b, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}




TEST_F(DataUpdatingTest, update_b)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change b and re-solve
    Vector<double,4> b2 = b;
    b2[0] = 0.;

    // revised original solver
    solver1.update_b(b2.data(), b2.size());
    solver1.solve();

    //new solver
    DefaultSolver<double> solver2(P, q, A, b2, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_b_eigen)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // revised original solver
    Vector<double,4> b2 = b;
    b2[0] = 0.;
    solver1.update_b(b2);
    solver1.solve();

    //new solver
    DefaultSolver<double> solver2(P, q, A, b2, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_b_partial)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change b and re-solve
    Vector<double,2> values(0.,0.);
    Vector<uintptr_t,2>  index(1,3);
    solver1.update_b(index.data(), values.data(), index.size());
    solver1.solve();

    //new solver
    Vector<double,4> b2 = b;
    b2[1] = 0.;
    b2[3] = 0.;
    DefaultSolver<double> solver2(P, q, A, b2, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}

TEST_F(DataUpdatingTest, update_b_partial_eigen)
{
    DefaultSolver<double> solver1(P, q, A, b, cones, settings);
    solver1.solve();

    // change b and re-solve
    Vector<double,2> values(0.,0.);
    Vector<uintptr_t,2>  index(1,3);
    solver1.update_b(index, values);
    solver1.solve();

    //new solver
    Vector<double,4> b2 = b;
    b2[1] = 0.;
    b2[3] = 0.;
    DefaultSolver<double> solver2(P, q, A, b2, cones, settings);
    solver2.solve();

    // Check the solution
    auto diff = solver1.solution().x - solver2.solution().x;
    ASSERT_NEAR(diff.norm(), 0.0, 1e-6);
}





