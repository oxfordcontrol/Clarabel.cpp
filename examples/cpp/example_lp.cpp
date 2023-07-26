#include "clarabel.h"
#include "utils.h"
#include <cstdint>
#include <vector>
#include <memory>

using namespace clarabel;
using namespace std;

int main(void)
{
    // 2 x 2 zero matrix
    uintptr_t P_colptr[] = { 0, 0, 0 };
    CscMatrix<double> P(
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
    CscMatrix<double> A(
        4,
        2,        // row, col
        A_colptr, // colptr
        A_rowptr, // rowval
        A_nzvals  // nzval
    );

    double b[4] = { 1.0, 1.0, 1.0, 1.0 };

    auto cone1 = NonnegativeConeT<double>(4);

    vector<SupportedConeT<double>> cones
    {
        NonnegativeConeT<double>(4),
        // Add more cones: ZeroConeT<double>(2)
    };
    //cones.push_back(NonnegativeConeT<double>(4));
    // Add more cones: // cones.push_back(ZeroConeT<double>(4));

    // SupportedConeT<double> *cones[1] =
    // {
    //     {.tag = SupportedConeT<double>::Tag::NonnegativeConeT, .nonnegative_cone_t = {._0 = 4 }}
    // };

    // Settings
    DefaultSettings<double> settings = DefaultSettingsBuilder<double>::default_settings()
        .equilibrate_enable(true)
        .equilibrate_max_iter(50)
        .build();

    // Build solver
    DefaultSolver<double> solver(
        &P, // P
        q,  // q
        &A, // A
        b,  // b
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