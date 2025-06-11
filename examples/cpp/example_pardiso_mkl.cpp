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

#ifndef FEATURE_PARDISO_MKL
    printf("This example requires MKL Pardiso support.\n");
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

    //settings.direct_solve_method = ClarabelDirectSolveMethods::PARDISO_PANUA;    
    settings.direct_solve_method = ClarabelDirectSolveMethods::PARDISO_MKL;    

    settings.pardiso_verbose = true;
    settings.pardiso_iparm[1] = 0; // tries minimum degree ordering instead of default 

    //set the number of threads
    settings.max_threads = 16;

    // Build solver
    DefaultSolver<double> solver(P, q, A, b, cones, settings);

    // Solve
    solver.solve();

    // Get solution
    DefaultSolution<double> solution = solver.solution();
    utils::print_solution(solution);

    DefaultInfo<double> info = solver.info();

    printf("primal residual = %e\n", info.res_primal);
    printf("dual residual   = %e\n", info.res_dual);
    printf("# of threads    = %d\n", info.linsolver.threads);
    printf("KKT nonzeros    = %d\n", info.linsolver.nnzA);
    printf("factor nonzeros = %d\n", info.linsolver.nnzL);

    return 0;

#endif // FEATURE_FAER_SPARSE
}
