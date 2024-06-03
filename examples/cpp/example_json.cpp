// #define FEATURE_SDP
#include "utils.h"
#include <cstdio>
#include <Clarabel>

using namespace clarabel;
using namespace std;

#define STRING(string) #string
#define TO_STRING(string) STRING(string)

int main(void)
{
#ifndef FEATURE_SERDE

    printf("This example requires JSON serde support.\n");
    return 1;

#else

    std::string rootdir  = TO_STRING(EXAMPLES_ROOT_DIR);
    std::string filepath = "/data/hs35.json";
    std::string filename = rootdir + filepath; 
    cout << "Read from file: " << filename << endl;

    DefaultSolver<double> solver = DefaultSolver<double>::read_from_file(filename);
    solver.solve();

    // write it back to a file 
    // std::string outpath = "/data/output.json";
    // std::string filename_out = rootdir + outpath;
    // solver.write_to_file(filename_out);

    return 0;

#endif // FEATURE_SERDE
    
}