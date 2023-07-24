#include "clarabel.h"
#include "utils.h"
#include <cstdint>

using namespace clarabel;

int main(void)
{
    // 2 x 2 zero matrix
    uintptr_t P_colptr[] = { 0, 0, 0 };
    ClarabelCscMatrix<double> P(
        2,
        2,
        P_colptr,
        nullptr,
        nullptr
    );

    double q[2] = {1.0, -1.0};

    // a 2-d box constraint, separated into 4 inequalities.
    // A = [I; -I]
    uintptr_t A_colptr[] = { 0, 2, 4 };
    uintptr_t A_rowptr[] = { 0, 2, 1, 3 };
    double A_nzvals[] = { 1.0, -1.0, 1.0, -1.0 };
    ClarabelCscMatrix<double> A(
        4,
        2,        // row, col
        A_colptr, // colptr
        A_rowptr, // rowval
        A_nzvals  // nzval
    );

    double b[4] = {1.0, 1.0, 1.0, 1.0};

    ClarabelSupportedConeT<double> cones[1] =
    {
        // NonnegativeConeT(4)
        {.tag = ClarabelSupportedConeT<double>::Tag::NonnegativeConeT, .nonnegative_cone_t = {._0 = 4 }}
    };

    // Settings
    ClarabelDefaultSettings<double> settings = DefaultSettingsBuilder::default_settings<double>();
    settings.equilibrate_enable = true;
    settings.equilibrate_max_iter = 50;

    // Build solver
    DefaultSolver<double> solver(
        &P, // P
        q, // q
        &A, // A
        b, // b
        1, // n_cones
        cones,
        &settings
    );

    // Solve
    solver.solve();

    // Get solution
    DefaultSolution<double> solution = solver.solution();
    utils::print_solution(solution);

    return 0;
}