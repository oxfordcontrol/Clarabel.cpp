#include "utils.h"

#include <clarabel.hpp>
#include <Eigen/Eigen>
#include <vector>

using namespace clarabel;
using namespace std;
using namespace Eigen;

// NB: this example requires that the solver be built with -DCLARABEL_FEATURE_FAER_SPARSE

int main(void)
{

#ifndef FEATURE_FAER_SPARSE

    printf("This example requires faer-rs support.\n");
    return 1;

#else


    /* From dense matrix:
     * [[6., 0.],
     *  [0., 4.]]
     */
    MatrixXd P_dense(2, 2);
    P_dense <<
        6., 0.,
        0., 4.;

    SparseMatrix<double> P = P_dense.sparseView();
    P.makeCompressed();

    Vector<double, 2> q = { -1., -4. };

    MatrixXd A_dense(5, 2);
    A_dense <<
         1., -2., // <-- LHS of equality constraint (lower bound)
         1.,  0., // <-- LHS of inequality constraint (upper bound)
         0.,  1., // <-- LHS of inequality constraint (upper bound)
        -1.,  0., // <-- LHS of inequality constraint (lower bound)
         0., -1.; // <-- LHS of inequality constraint (lower bound)

    SparseMatrix<double> A = A_dense.sparseView();
    A.makeCompressed();

    Vector<double, 5> b = { 0., 1., 1., 1., 1. };

    vector<SupportedConeT<double>> cones
    {
        ZeroConeT<double>(1),
        NonnegativeConeT<double>(4),
    };

    // Settings
    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();
    settings.direct_solve_method = ClarabelDirectSolveMethods::FAER;

    // Build solver
    DefaultSolver<double> solver(P, q, A, b, cones, settings);

    // Solve
    solver.solve();

    // Get solution
    DefaultSolution<double> solution = solver.solution();
    utils::print_solution(solution);

    return 0;

#endif // FEATURE_FAER_SPARSE
}
