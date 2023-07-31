#include "utils.h"

#include <Clarabel>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <cstdint>
#include <memory>
#include <vector>

using namespace clarabel;
using namespace std;
using namespace Eigen;

int main(void)
{
    MatrixXd P_dense = MatrixXd::Zero(2, 2);
    SparseMatrix<double> P = P_dense.sparseView();
    P.makeCompressed();
    
    double q[2] = {1.0, -1.0};

    // a 2-d box constraint, separated into 4 inequalities.
    // A = [I; -I]
    MatrixXd A_dense(4, 2);
    A_dense <<
         1.,  0.,
         0.,  1.,
        -1.,  0.,
         0., -1.;
        
    SparseMatrix<double> A = A_dense.sparseView();
    A.makeCompressed();
    
    double b[4] = { 1.0, 1.0, 1.0, 1.0 };

    vector<SupportedConeT<double>> cones
    {
        NonnegativeConeT<double>(4),
        // {.tag = SupportedConeT<double>::Tag::NonnegativeConeT, .nonnegative_cone_t = {._0 = 4 }}
    };

    // Settings
    DefaultSettings<double> settings = DefaultSettingsBuilder<double>::default_settings()
                                           .equilibrate_enable(true)
                                           .equilibrate_max_iter(50)
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