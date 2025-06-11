#include "utils.h"

#include <clarabel.hpp>
#include <Eigen/Eigen>
#include <vector>
#include <iostream>

using namespace clarabel;
using namespace std;
using namespace Eigen;

int main(void)
{
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

    // Build solver
    DefaultSolver<double> solver(P, q, A, b, cones, settings);

    // redirect progress output to a file 
    std::cout << "Printing to file ...." << std::endl;
    solver.print_to_file("clarabel_example_print_stream.txt");
    solver.solve();

    // redirect progress output to a buffer
    std::cout << "Printing to buffer ...." << std::endl;
    solver.print_to_buffer();
    solver.solve();

    std::cout << "Recovering from buffer ...." << std::endl;
    std::string bufstr = solver.get_print_buffer();
    std::cout << bufstr << std::endl;

    // restore the buffer to stdout
    std::cout << "Printing to stdout ...." << std::endl;
    solver.print_to_stdout();
    solver.solve();

    return 0;
}
