#ifndef CLARABEL_DEFAULT_SETTINGS_H
#define CLARABEL_DEFAULT_SETTINGS_H

#include "ClarabelTypes.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

// ClarabelDefaultSettings types
typedef enum ClarabelDirectSolveMethods
{
    QDLDL,
    // MKL, (not supported in Rust yet)
    // CHOLMOD, (not supported in Rust yet)
} ClarabelDirectSolveMethods;

typedef struct ClarabelDefaultSettings_f64
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
    ClarabelDirectSolveMethods direct_solve_method;
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
} ClarabelDefaultSettings_f64;

typedef struct ClarabelDefaultSettings_f32
{
    uint32_t max_iter;
    double time_limit;
    bool verbose;
    float max_step_fraction;
    float tol_gap_abs;
    float tol_gap_rel;
    float tol_feas;
    float tol_infeas_abs;
    float tol_infeas_rel;
    float tol_ktratio;
    float reduced_tol_gap_abs;
    float reduced_tol_gap_rel;
    float reduced_tol_feas;
    float reduced_tol_infeas_abs;
    float reduced_tol_infeas_rel;
    float reduced_tol_ktratio;
    bool equilibrate_enable;
    uint32_t equilibrate_max_iter;
    float equilibrate_min_scaling;
    float equilibrate_max_scaling;
    float linesearch_backtrack_step;
    float min_switch_step_length;
    float min_terminate_step_length;
    bool direct_kkt_solver;
    enum ClarabelDirectSolveMethods direct_solve_method;
    bool static_regularization_enable;
    float static_regularization_constant;
    float static_regularization_proportional;
    bool dynamic_regularization_enable;
    float dynamic_regularization_eps;
    float dynamic_regularization_delta;
    bool iterative_refinement_enable;
    float iterative_refinement_reltol;
    float iterative_refinement_abstol;
    uint32_t iterative_refinement_max_iter;
    float iterative_refinement_stop_ratio;
    bool presolve_enable;
} ClarabelDefaultSettings_f32;

#ifdef CLARABEL_USE_FLOAT
typedef ClarabelDefaultSettings_f32 ClarabelDefaultSettings;
#else
typedef ClarabelDefaultSettings_f64 ClarabelDefaultSettings;
#endif

// ClarabelDefaultSettings APIs

// ClarabelDefaultSettings::default
ClarabelDefaultSettings_f64 clarabel_DefaultSettings_f64_default(void);

ClarabelDefaultSettings_f32 clarabel_DefaultSettings_f32_default(void);

static inline ClarabelDefaultSettings clarabel_DefaultSettings_default(void)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSettings_f32_default();
#else
    return clarabel_DefaultSettings_f64_default();
#endif
}

#endif /* CLARABEL_DEFAULT_SETTINGS_H */