use crate::{algebra::CscMatrix, solver::implementations::default::settings::*};
use clarabel::algebra::{self as lib, FloatT};

/// Convert a CscMatrix from C to Rust
///
/// The CscMatrix object returned will take ownership of the memory of arrays allocated in C.
/// Make sure to call std::mem::forget on the vectors in the CscMatrix object to leave the memory management for C side
#[allow(non_snake_case)]
pub unsafe fn convert_from_C_CscMatrix(ptr: *const CscMatrix<f64>) -> lib::CscMatrix<f64> {
    // Recover the CscMatrix from the raw pointer from C
    let matrix = match ptr.as_ref() {
        Some(mat) => mat,
        None => panic!("Null pointer passed to convert_from_C_CscMatrix"),
    };

    let m = matrix.m;
    let n = matrix.n;

    // Recover the vectors from the raw array pointers from C

    // Length of colptr is always n + 1
    let colptr = Vec::from_raw_parts(matrix.colptr as *mut usize, n + 1, n + 1);

    // Length of rowval and nzval is given by colptr[n]
    let rowval = Vec::from_raw_parts(matrix.rowval as *mut usize, colptr[n], colptr[n]);
    let nzval_vec = Vec::from_raw_parts(matrix.nzval as *mut f64, colptr[n], colptr[n]);

    // Call the CscMatrix constructor defined in Rust
    let rust_matrix = lib::CscMatrix::<f64> {
        m,
        n,
        colptr,
        rowval,
        nzval: nzval_vec,
    };

    // Return the Rust CscMatrix built from the C struct
    rust_matrix
}

// Convert from the C struct to the Rust struct by copying the values
pub fn get_solver_settings_from_c<T: FloatT>(
    value: &DefaultSettings<T>,
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
            DirectSolveMethods::QDLDL => String::from("qdldl"),
            DirectSolveMethods::MKL => String::from("mkl"),
            DirectSolveMethods::CHOLMOD => String::from("cholmod"),
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
