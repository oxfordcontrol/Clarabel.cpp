#ifndef SOLVER_SETTINGS_H
#define SOLVER_SETTINGS_H

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

typedef enum DirectSolveMethods
{
    QDLDL,
    MKL,
    CHOLMOD,
} DirectSolveMethods;

typedef struct DefaultSettings_f64
{
    uint32_t max_iter;
    double time_limit;
    bool verbose;
    double max_step_fraction;
    double tol_gap_abs;
    double tol_gap_rel;
    double tol_feas;
    double tol_infeas_abs;
    double tol_infeas_rel;
    double tol_ktratio;
    double reduced_tol_gap_abs;
    double reduced_tol_gap_rel;
    double reduced_tol_feas;
    double reduced_tol_infeas_abs;
    double reduced_tol_infeas_rel;
    double reduced_tol_ktratio;
    bool equilibrate_enable;
    uint32_t equilibrate_max_iter;
    double equilibrate_min_scaling;
    double equilibrate_max_scaling;
    double linesearch_backtrack_step;
    double min_switch_step_length;
    double min_terminate_step_length;
    bool direct_kkt_solver;
    DirectSolveMethods direct_solve_method;
    bool static_regularization_enable;
    double static_regularization_constant;
    double static_regularization_proportional;
    bool dynamic_regularization_enable;
    double dynamic_regularization_eps;
    double dynamic_regularization_delta;
    bool iterative_refinement_enable;
    double iterative_refinement_reltol;
    double iterative_refinement_abstol;
    uint32_t iterative_refinement_max_iter;
    double iterative_refinement_stop_ratio;
    bool presolve_enable;
} DefaultSettings_f64;

DefaultSettings_f64 DefaultSettingsBuilder_f64_default(void);

#endif