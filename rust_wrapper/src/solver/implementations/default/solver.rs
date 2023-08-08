use crate::algebra::ClarabelCscMatrix;
use crate::core::cones::ClarabelSupportedConeT;
use crate::solver::implementations::default::settings::*;
use crate::utils;
use clarabel::algebra::FloatT;
use clarabel::solver::{self as lib, IPSolver, SolverStatus};
use std::slice;
use std::{ffi::c_void, mem::forget};

use super::info::ClarabelDefaultInfo;
use super::solution::DefaultSolution;

#[allow(non_camel_case_types)]
pub type ClarabelDefaultSolver_f32 = c_void;
#[allow(non_camel_case_types)]
pub type ClarabelDefaultSolver_f64 = c_void;

// Wrapper function to create a DefaultSolver object from C using dynamic memory allocation
// - Matrices and vectors are constructed from raw pointers
// - Cones are converted from C struct to Rust struct
// - Settings are converted from C struct to Rust struct
//
// b and cones are allowed to be null pointers, in which case they form zero-length slices and this is consistent with Clarabel.rs.
#[allow(non_snake_case)]
unsafe fn _internal_DefaultSolver_new<T: FloatT>(
    P: *const ClarabelCscMatrix<T>, // Matrix P
    q: *const T,                    // Array of double from C
    A: *const ClarabelCscMatrix<T>, // Matrix A
    b: *const T,                    // Array of double from C
    n_cones: usize,                 // Number of cones
    cones: *const ClarabelSupportedConeT<T>,
    settings: *const ClarabelDefaultSettings<T>,
) -> *mut c_void {
    // Check null pointers
    debug_assert!(!P.is_null(), "Pointer P must not be null");
    debug_assert!(!q.is_null(), "Pointer q must not be null");
    debug_assert!(!A.is_null(), "Pointer A must not be null");
    if P.is_null() || q.is_null() || A.is_null() {
        return std::ptr::null_mut();
    }

    // Recover the matrices from C structs
    let P = utils::convert_from_C_CscMatrix(P);
    let A = utils::convert_from_C_CscMatrix(A);
    // Recover the arrays from C pointers and deduce their lengths from the matrix dimensions
    let q = Vec::from_raw_parts(q as *mut T, A.n, A.n);
    let b = Vec::from_raw_parts(b as *mut T, A.m, A.m);

    // Get a reference to the DefaultSettings struct from the pointer passed from C
    let settings_struct = &*(settings as *const ClarabelDefaultSettings<T>);
    let settings = utils::get_solver_settings_from_c::<T>(settings_struct);

    // Convert the cones from C to Rust
    let c_cones = slice::from_raw_parts(cones, n_cones);
    // c_cones is a slice created from a raw pointer, so it is not managed by Rust, i.e. C side array is not freed when the slice is dropped.
    let cones_vec = utils::convert_from_C_cones(c_cones);
    let cones = cones_vec.as_slice();

    // Create the solver
    // This is dropped at the end of the function because it exists on the Rust side only, and cones and settings are created on the Rust side.
    let solver = lib::DefaultSolver::<T>::new(&P, &q, &A, &b, cones, settings);

    // Ensure Rust does not free the memory of arrays managed by C
    forget(P);
    forget(A);
    forget(q);
    forget(b);

    // Return the solver object as an opaque pointer to C.
    // The solver object is boxed and left on the heap.
    Box::into_raw(Box::new(solver)) as *mut c_void
}

#[no_mangle]
#[allow(non_snake_case)]
pub unsafe extern "C" fn clarabel_DefaultSolver_f64_new(
    P: *const ClarabelCscMatrix<f64>,
    q: *const f64,
    A: *const ClarabelCscMatrix<f64>,
    b: *const f64,
    n_cones: usize,
    cones: *const ClarabelSupportedConeT<f64>,
    settings: *const ClarabelDefaultSettings<f64>,
) -> *mut ClarabelDefaultSolver_f64 {
    _internal_DefaultSolver_new(P, q, A, b, n_cones, cones, settings)
}

#[no_mangle]
#[allow(non_snake_case)]
pub unsafe extern "C" fn clarabel_DefaultSolver_f32_new(
    P: *const ClarabelCscMatrix<f32>,
    q: *const f32,
    A: *const ClarabelCscMatrix<f32>,
    b: *const f32,
    n_cones: usize,
    cones: *const ClarabelSupportedConeT<f32>,
    settings: *const ClarabelDefaultSettings<f32>,
) -> *mut ClarabelDefaultSolver_f32 {
    _internal_DefaultSolver_new(P, q, A, b, n_cones, cones, settings)
}

// Wrapper function to call DefaultSolver.solve() from C
#[allow(non_snake_case)]
fn _internal_DefaultSolver_solve<T: FloatT>(solver: *mut c_void) {
    // Recover the solver object from the opaque pointer
    let mut solver = unsafe { Box::from_raw(solver as *mut lib::DefaultSolver<T>) };

    // Use the recovered solver object
    solver.solve();

    // Leave the solver object on the heap
    Box::into_raw(solver);
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSolver_f64_solve(solver: *mut ClarabelDefaultSolver_f64) {
    _internal_DefaultSolver_solve::<f64>(solver);
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSolver_f32_solve(solver: *mut ClarabelDefaultSolver_f32) {
    _internal_DefaultSolver_solve::<f32>(solver);
}

// Function to free the memory of the solver object
#[allow(non_snake_case)]
unsafe fn _internal_DefaultSolver_free<T: FloatT>(solver: *mut c_void) {
    if !solver.is_null() {
        // Reconstruct the box to drop the solver object
        let boxed = Box::from_raw(solver as *mut lib::DefaultSolver<T>);
        drop(boxed);
    }
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_DefaultSolver_f64_free(solver: *mut ClarabelDefaultSolver_f64) {
    _internal_DefaultSolver_free::<f64>(solver);
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_DefaultSolver_f32_free(solver: *mut ClarabelDefaultSolver_f32) {
    _internal_DefaultSolver_free::<f32>(solver);
}

#[repr(C)]
#[allow(dead_code)]
pub enum ClarabelSolverStatus {
    /// Problem is not solved (solver hasn't run).
    ClarabelUnsolved,
    /// Solver terminated with a solution.
    ClarabelSolved,
    /// Problem is primal infeasible.  Solution returned is a certificate of primal infeasibility.
    ClarabelPrimalInfeasible,
    /// Problem is dual infeasible.  Solution returned is a certificate of dual infeasibility.
    ClarabelDualInfeasible,
    /// Solver terminated with a solution (reduced accuracy)
    ClarabelAlmostSolved,
    /// Problem is primal infeasible.  Solution returned is a certificate of primal infeasibility (reduced accuracy).
    ClarabelAlmostPrimalInfeasible,
    /// Problem is dual infeasible.  Solution returned is a certificate of dual infeasibility (reduced accuracy).
    ClarabelAlmostDualInfeasible,
    /// Iteration limit reached before solution or infeasibility certificate found.
    ClarabelMaxIterations,
    /// Time limit reached before solution or infeasibility certificate found.
    ClarabelMaxTime,
    /// Solver terminated with a numerical error
    ClarabelNumericalError,
    /// Solver terminated due to lack of progress.
    ClarabelInsufficientProgress,
}

impl From<&mut SolverStatus> for ClarabelSolverStatus {
    fn from(value: &mut SolverStatus) -> Self {
        match value {
            SolverStatus::Unsolved => ClarabelSolverStatus::ClarabelUnsolved,
            SolverStatus::Solved => ClarabelSolverStatus::ClarabelSolved,
            SolverStatus::PrimalInfeasible => ClarabelSolverStatus::ClarabelPrimalInfeasible,
            SolverStatus::DualInfeasible => ClarabelSolverStatus::ClarabelDualInfeasible,
            SolverStatus::AlmostSolved => ClarabelSolverStatus::ClarabelAlmostSolved,
            SolverStatus::AlmostPrimalInfeasible => {
                ClarabelSolverStatus::ClarabelAlmostPrimalInfeasible
            }
            SolverStatus::AlmostDualInfeasible => {
                ClarabelSolverStatus::ClarabelAlmostDualInfeasible
            }
            SolverStatus::MaxIterations => ClarabelSolverStatus::ClarabelMaxIterations,
            SolverStatus::MaxTime => ClarabelSolverStatus::ClarabelMaxTime,
            SolverStatus::NumericalError => ClarabelSolverStatus::ClarabelNumericalError,
            SolverStatus::InsufficientProgress => {
                ClarabelSolverStatus::ClarabelInsufficientProgress
            }
        }
    }
}

/// Get the solution field from a DefaultSolver object.
///
/// The solution is returned as a C struct.
#[allow(non_snake_case)]
fn _internal_DefaultSolver_solution<T: FloatT>(solver: *mut c_void) -> DefaultSolution<T> {
    // Recover the solver object from the opaque pointer
    let mut solver = unsafe { Box::from_raw(solver as *mut lib::DefaultSolver<T>) };

    // Get the solution and convert to C struct
    let solution = DefaultSolution::<T>::from(&mut solver.solution);

    // Leave the solver object on the heap
    Box::into_raw(solver);

    // Return the solution as a C struct
    solution
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSolver_f64_solution(
    solver: *mut ClarabelDefaultSolver_f64,
) -> DefaultSolution<f64> {
    _internal_DefaultSolver_solution::<f64>(solver)
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSolver_f32_solution(
    solver: *mut ClarabelDefaultSolver_f32,
) -> DefaultSolution<f32> {
    _internal_DefaultSolver_solution::<f32>(solver)
}

/// Get the info field from a DefaultSolver object.
#[allow(non_snake_case)]
fn _internal_DefaultSolver_info<T: FloatT>(solver: *mut c_void) -> ClarabelDefaultInfo<T> {
    // Recover the solver object from the opaque pointer
    let mut solver = unsafe { Box::from_raw(solver as *mut lib::DefaultSolver<T>) };

    // Get the info field and convert to C struct
    let info = ClarabelDefaultInfo::<T>::from(&mut solver.info);

    // Leave the solver object on the heap
    Box::into_raw(solver);

    // Return the info as a C struct
    info
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSolver_f64_info(
    solver: *mut ClarabelDefaultSolver_f64,
) -> ClarabelDefaultInfo<f64> {
    _internal_DefaultSolver_info::<f64>(solver)
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSolver_f32_info(
    solver: *mut ClarabelDefaultSolver_f32,
) -> ClarabelDefaultInfo<f32> {
    _internal_DefaultSolver_info::<f32>(solver)
}