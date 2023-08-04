#include "utils.h"

#include <Clarabel>
#include <Eigen/Eigen>
#include <vector>

using namespace clarabel;
using namespace std;
using namespace Eigen;

int main()
{
    MatrixXd P_dense(2, 2);
    P_dense <<
        0., 0.,
        0., 2.;

    SparseMatrix<double> P = P_dense.sparseView();
    P.makeCompressed();
    double q[2] = { 0., 0. };

    /* From dense matrix:
     * [[ 0.,  0.],
     *  [-2.,  0.],
     *  [ 0., -1.]]
     */
    MatrixXd A_dense(3, 2);
    A_dense <<
         0.,  0.,
        -2.,  0.,
         0., -1.;

    SparseMatrix<double> A = A_dense.sparseView();
    A.makeCompressed();

    double b[3] = { 1., -2., -2. };

    vector<SupportedConeT<double>> cones
    {
        SecondOrderConeT<double>(3),
    };

    // Settings
    DefaultSettings<double> settings = DefaultSettingsBuilder<double>::default_settings().build();

    // Build solver
    clarabel::eigen::DefaultSolver<double> solver(P, q, A, b, cones, &settings);

    // Solve
    solver.solve();

    // Get solution
    DefaultSolution<double> solution = solver.solution();
    utils::print_solution(solution);

    return 0;
}