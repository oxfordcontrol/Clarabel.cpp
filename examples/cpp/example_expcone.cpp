#include "utils.h"

#include <Clarabel>
#include <Eigen/Eigen>
#include <cmath>
#include <cstdint>
#include <vector>

using namespace clarabel;
using namespace std;
using namespace Eigen;

int main(void)
{
    SparseMatrix<double> P = MatrixXd::Zero(3, 3).sparseView();
    P.makeCompressed();

    double q[3] = { -1.0, 0.0, 0.0 };

    MatrixXd A_dense(5, 3);
    A_dense <<
        -1.,  0.,  0.,
         0., -1.,  0.,
         0.,  0., -1.,
         0.,  1.,  0.,
         0.,  0.,  1.;

    SparseMatrix<double> A = A_dense.sparseView();
    A.makeCompressed();

    double b[5] = { 0., 0., 0., 1., exp(5.0) };

    vector<SupportedConeT<double>> cones
    {
        ExponentialConeT<double>(),
        ZeroConeT<double>(2),
    };

    // Settings
    DefaultSettings<double> settings = DefaultSettingsBuilder<double>::default_settings()
                                           .verbose(true)
                                           .build();

    // Build solver
    clarabel::eigen::DefaultSolver<double> solver(P, q, A, b, cones, &settings);

    // Solve
    solver.solve();

    // Get solution
    DefaultSolution<double> solution = solver.solution();
    utils::print_solution(solution);

    return 0;
}