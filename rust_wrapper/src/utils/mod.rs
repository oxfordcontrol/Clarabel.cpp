// Submodules: csc_matrix.rs, supported_cones_T.rs
mod csc_matrix;
#[allow(non_snake_case)]
mod supported_cones_T;

// Re-export the submodule members
pub use csc_matrix::*;
pub use supported_cones_T::*;

// Uses
use crate::solver::implementations::default::settings::*;
use clarabel::algebra::FloatT;

/// Convert from the C struct to the Rust struct by copying the values
pub fn get_solver_settings_from_c<T: FloatT>(
    value: &ClarabelDefaultSettings<T>,
) -> clarabel::solver::implementations::default::DefaultSettings<T> {
    clarabel::solver::implementations::default::DefaultSettings::<T> {
        max_iter: value.max_iter,
        time_limit: value.time_limit,
        verbose: value.verbose,
        max_step_fraction: value.max_step_fraction,
        tol_gap_abs: value.tol_gap_abs,
        tol_gap_rel: value.tol_gap_rel,
        tol_feas: value.tol_feas,
        tol_infeas_abs: value.tol_infeas_abs,
        tol_infeas_rel: value.tol_infeas_rel,
        tol_ktratio: value.tol_ktratio,
        reduced_tol_gap_abs: value.reduced_tol_gap_abs,
        reduced_tol_gap_rel: value.reduced_tol_gap_rel,
        reduced_tol_feas: value.reduced_tol_feas,
        reduced_tol_infeas_abs: value.reduced_tol_infeas_abs,
        reduced_tol_infeas_rel: value.reduced_tol_infeas_rel,
        reduced_tol_ktratio: value.reduced_tol_ktratio,
        equilibrate_enable: value.equilibrate_enable,
        equilibrate_max_iter: value.equilibrate_max_iter,
        equilibrate_min_scaling: value.equilibrate_min_scaling,
        equilibrate_max_scaling: value.equilibrate_max_scaling,
        linesearch_backtrack_step: value.linesearch_backtrack_step,
        min_switch_step_length: value.min_switch_step_length,
        min_terminate_step_length: value.min_terminate_step_length,
        direct_kkt_solver: value.direct_kkt_solver,
        direct_solve_method: match value.direct_solve_method {
            ClarabelDirectSolveMethods::QDLDL => String::from("qdldl"),
            // Not supported yet
            //ClarabelDirectSolveMethods::MKL => String::from("mkl"),
            //ClarabelDirectSolveMethods::CHOLMOD => String::from("cholmod"),
        },
        static_regularization_enable: value.static_regularization_enable,
        static_regularization_constant: value.static_regularization_constant,
        static_regularization_proportional: value.static_regularization_proportional,
        dynamic_regularization_enable: value.dynamic_regularization_enable,
        dynamic_regularization_eps: value.dynamic_regularization_eps,
        dynamic_regularization_delta: value.dynamic_regularization_delta,
        iterative_refinement_enable: value.iterative_refinement_enable,
        iterative_refinement_reltol: value.iterative_refinement_reltol,
        iterative_refinement_abstol: value.iterative_refinement_abstol,
        iterative_refinement_max_iter: value.iterative_refinement_max_iter,
        iterative_refinement_stop_ratio: value.iterative_refinement_stop_ratio,
        presolve_enable: value.presolve_enable,
    }
}
