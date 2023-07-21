#ifndef CLARABEL_H
#define CLARABEL_H

/* Generated with cbindgen:0.24.5 */

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually.
 * This file is only used to track the changes in the Rust wrapper.
 * Use the headers in ../../include instead.
 */

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

namespace clarabel {

enum class ClarabelDirectSolveMethods {
  QDLDL,
};

enum class ClarabelSolverStatus {
  ClarabelUnsolved,
  ClarabelSolved,
  ClarabelPrimalInfeasible,
  ClarabelDualInfeasible,
  ClarabelAlmostSolved,
  ClarabelAlmostPrimalInfeasible,
  ClarabelAlmostDualInfeasible,
  ClarabelMaxIterations,
  ClarabelMaxTime,
  ClarabelNumericalError,
  ClarabelInsufficientProgress,
};

template<typename T>
struct ClarabelDefaultSettings {
  uint32_t max_iter;
  double time_limit;
  bool verbose;
  T max_step_fraction;
  T tol_gap_abs;
  T tol_gap_rel;
  T tol_feas;
  T tol_infeas_abs;
  T tol_infeas_rel;
  T tol_ktratio;
  T reduced_tol_gap_abs;
  T reduced_tol_gap_rel;
  T reduced_tol_feas;
  T reduced_tol_infeas_abs;
  T reduced_tol_infeas_rel;
  T reduced_tol_ktratio;
  bool equilibrate_enable;
  uint32_t equilibrate_max_iter;
  T equilibrate_min_scaling;
  T equilibrate_max_scaling;
  T linesearch_backtrack_step;
  T min_switch_step_length;
  T min_terminate_step_length;
  bool direct_kkt_solver;
  ClarabelDirectSolveMethods direct_solve_method;
  bool static_regularization_enable;
  T static_regularization_constant;
  T static_regularization_proportional;
  bool dynamic_regularization_enable;
  T dynamic_regularization_eps;
  T dynamic_regularization_delta;
  bool iterative_refinement_enable;
  T iterative_refinement_reltol;
  T iterative_refinement_abstol;
  uint32_t iterative_refinement_max_iter;
  T iterative_refinement_stop_ratio;
  bool presolve_enable;
};

using ClarabelDefaultSolver = void;

template<typename T>
struct ClarabelCscMatrix {
  uintptr_t m;
  uintptr_t n;
  const uintptr_t *colptr;
  const uintptr_t *rowval;
  const T *nzval;
};

template<typename T>
struct ClarabelSupportedConeT {
  enum class Tag {
    ZeroConeT,
    NonnegativeConeT,
    SecondOrderConeT,
    ExponentialConeT,
    PowerConeT,
#if defined(FEATURE_SDP)
    PSDTriangleConeT,
#endif
  };

  struct ZeroConeT_Body {
    uintptr_t _0;
  };

  struct NonnegativeConeT_Body {
    uintptr_t _0;
  };

  struct SecondOrderConeT_Body {
    uintptr_t _0;
  };

  struct ExponentialConeT_Body {

  };

  struct PowerConeT_Body {
    T _0;
  };

#if defined(FEATURE_SDP)
  struct PSDTriangleConeT_Body {
    uintptr_t _0;
  };
#endif

  Tag tag;
  union {
    ZeroConeT_Body zero_cone_t;
    NonnegativeConeT_Body nonnegative_cone_t;
    SecondOrderConeT_Body second_order_cone_t;
    ExponentialConeT_Body exponential_cone_t;
    PowerConeT_Body power_cone_t;
#if defined(FEATURE_SDP)
    PSDTriangleConeT_Body psd_triangle_cone_t;
#endif
  };
};

using ClarabelDefaultSolver_f32 = void;

template<typename T>
struct DefaultSolution {
  T *x;
  uintptr_t x_length;
  T *z;
  uintptr_t z_length;
  T *s;
  uintptr_t s_length;
  ClarabelSolverStatus status;
  T obj_val;
  double solve_time;
  uint32_t iterations;
  T r_prim;
  T r_dual;
};

extern "C" {

ClarabelDefaultSettings<double> clarabel_DefaultSettingsBuilder_f64_default();

ClarabelDefaultSettings<float> clarabel_DefaultSettingsBuilder_f32_default();

ClarabelDefaultSolver *clarabel_DefaultSolver_f64_new(const ClarabelCscMatrix<double> *P,
                                                      const double *q,
                                                      const ClarabelCscMatrix<double> *A,
                                                      const double *b,
                                                      uintptr_t n_cones,
                                                      const ClarabelSupportedConeT<double> *cones,
                                                      const ClarabelDefaultSettings<double> *settings);

ClarabelDefaultSolver_f32 *clarabel_DefaultSolver_f32_new(const ClarabelCscMatrix<float> *P,
                                                          const float *q,
                                                          const ClarabelCscMatrix<float> *A,
                                                          const float *b,
                                                          uintptr_t n_cones,
                                                          const ClarabelSupportedConeT<float> *cones,
                                                          const ClarabelDefaultSettings<float> *settings);

void clarabel_DefaultSolver_f64_solve(ClarabelDefaultSolver *solver);

void clarabel_DefaultSolver_f32_solve(ClarabelDefaultSolver_f32 *solver);

void clarabel_DefaultSolver_f64_free(ClarabelDefaultSolver *solver);

void clarabel_DefaultSolver_f32_free(ClarabelDefaultSolver_f32 *solver);

DefaultSolution<double> clarabel_DefaultSolver_f64_solution(ClarabelDefaultSolver *solver);

DefaultSolution<float> clarabel_DefaultSolver_f32_solution(ClarabelDefaultSolver_f32 *solver);

} // extern "C"

} // namespace clarabel

#endif // CLARABEL_H
