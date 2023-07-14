use crate::algebra::CscMatrix;
use crate::core::cones::SupportedConeT;
use crate::solver::implementations::default::settings::*;
use crate::utils;
use clarabel::algebra::FloatT;
use clarabel::solver::{self as lib, IPSolver};
use std::slice;
use std::{ffi::c_void, mem::forget};

use super::solution::DefaultSolution;

#[allow(non_camel_case_types)]
pub type DefaultSolver_f32 = c_void;
#[allow(non_camel_case_types)]
pub type DefaultSolver_f64 = c_void;

// Wrapper function to create a DefaultSolver object from C using dynamic memory allocation
// - Matrices and vectors are constructed from raw pointers
// - Cones are converted from C struct to Rust struct
// - Settings are converted from C struct to Rust struct
#[allow(non_snake_case)]
unsafe fn DefaultSolver_new<T: FloatT>(
    P: *const CscMatrix<T>, // Matrix P
    q: *const T,            // Array of double from C
    A: *const CscMatrix<T>, // Matrix A
    b: *const T,            // Array of double from C
    n_cones: usize,         // Number of cones
    cones: *const SupportedConeT<T>,
    settings: *const DefaultSettings<T>,
) -> *mut c_void {
    // Check null pointers
    debug_assert!(!P.is_null(), "Pointer P must not be null");
    debug_assert!(!q.is_null(), "Pointer q must not be null");
    debug_assert!(!A.is_null(), "Pointer A must not be null");
    debug_assert!(!b.is_null(), "Pointer b must not be null");
    debug_assert!(!cones.is_null(), "Pointer cones must not be null");
    if P.is_null() || q.is_null() || A.is_null() || b.is_null() || cones.is_null() {
        return std::ptr::null_mut();
    }

    // Recover the matrices from C structs
    let P = utils::convert_from_C_CscMatrix(P);
    let A = utils::convert_from_C_CscMatrix(A);
    // Recover the arrays from C pointers and deduce their lengths from the matrix dimensions
    let q = Vec::from_raw_parts(q as *mut T, A.n, A.n);
    let b = Vec::from_raw_parts(b as *mut T, A.m, A.m);

    // Get a reference to the DefaultSettings struct from the pointer passed from C
    let settings_struct = &*(settings as *const DefaultSettings<T>);
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
    forget(P.colptr);
    forget(P.rowval);
    forget(P.nzval);
    forget(A.colptr);
    forget(A.rowval);
    forget(A.nzval);
    forget(q);
    forget(b);

    // Return the solver object as an opaque pointer to C.
    // The solver object is boxed and left on the heap.
    Box::into_raw(Box::new(solver)) as *mut c_void
}

#[no_mangle]
#[allow(non_snake_case)]
pub unsafe extern "C" fn DefaultSolver_f64_new(
    P: *const CscMatrix<f64>,
    q: *const f64,
    A: *const CscMatrix<f64>,
    b: *const f64,
    n_cones: usize,
    cones: *const SupportedConeT<f64>,
    settings: *const DefaultSettings<f64>,
) -> *mut DefaultSolver_f64 {
    DefaultSolver_new(P, q, A, b, n_cones, cones, settings)
}

#[no_mangle]
#[allow(non_snake_case)]
pub unsafe extern "C" fn DefaultSolver_f32_new(
    P: *const CscMatrix<f32>,
    q: *const f32,
    A: *const CscMatrix<f32>,
    b: *const f32,
    n_cones: usize,
    cones: *const SupportedConeT<f32>,
    settings: *const DefaultSettings<f32>,
) -> *mut DefaultSolver_f32 {
    DefaultSolver_new(P, q, A, b, n_cones, cones, settings)
}

// Wrapper function to call DefaultSolver.solve() from C
#[allow(non_snake_case)]
fn DefaultSolver_solve<T: FloatT>(solver: *mut c_void) {
    // Recover the solver object from the opaque pointer
    let mut solver = unsafe { Box::from_raw(solver as *mut lib::DefaultSolver<T>) };

    // Use the recovered solver object
    solver.solve();

    // Leave the solver object on the heap
    Box::into_raw(solver);
}

#[no_mangle]
pub extern "C" fn DefaultSolver_f64_solve(solver: *mut DefaultSolver_f64) {
    DefaultSolver_solve::<f64>(solver);
}

#[no_mangle]
pub extern "C" fn DefaultSolver_f32_solve(solver: *mut DefaultSolver_f32) {
    DefaultSolver_solve::<f32>(solver);
}

// Function to free the memory of the solver object
#[allow(non_snake_case)]
unsafe fn free_DefaultSolver<T: FloatT>(solver: *mut c_void) {
    if !solver.is_null() {
        // Reconstruct the box to drop the solver object
        let boxed = Box::from_raw(solver as *mut lib::DefaultSolver<T>);
        drop(boxed);
    }
}

#[no_mangle]
pub unsafe extern "C" fn free_DefaultSolver_f64(solver: *mut DefaultSolver_f64) {
    free_DefaultSolver::<f64>(solver);
}

#[no_mangle]
pub unsafe extern "C" fn free_DefaultSolver_f32(solver: *mut DefaultSolver_f32) {
    free_DefaultSolver::<f32>(solver);
}

#[repr(C)]
#[allow(dead_code)]
pub enum SolverStatus {
    /// Problem is not solved (solver hasn't run).
    Unsolved,
    /// Solver terminated with a solution.
    Solved,
    /// Problem is primal infeasible.  Solution returned is a certificate of primal infeasibility.
    PrimalInfeasible,
    /// Problem is dual infeasible.  Solution returned is a certificate of dual infeasibility.
    DualInfeasible,
    /// Solver terminated with a solution (reduced accuracy)
    AlmostSolved,
    /// Problem is primal infeasible.  Solution returned is a certificate of primal infeasibility (reduced accuracy).
    AlmostPrimalInfeasible,
    /// Problem is dual infeasible.  Solution returned is a certificate of dual infeasibility (reduced accuracy).
    AlmostDualInfeasible,
    /// Iteration limit reached before solution or infeasibility certificate found.
    MaxIterations,
    /// Time limit reached before solution or infeasibility certificate found.
    MaxTime,
    /// Solver terminated with a numerical error
    NumericalError,
    /// Solver terminated due to lack of progress.
    InsufficientProgress,
}

/// Get the solution field from a DefaultSolver object.
///
/// The solution is returned as a C struct.
#[allow(non_snake_case)]
fn DefaultSolver_solution<T: FloatT>(solver: *mut c_void) -> DefaultSolution<T> {
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
pub extern "C" fn DefaultSolver_f64_solution(
    solver: *mut DefaultSolver_f64,
) -> DefaultSolution<f64> {
    DefaultSolver_solution::<f64>(solver)
}

#[no_mangle]
pub extern "C" fn DefaultSolver_f32_solution(
    solver: *mut DefaultSolver_f32,
) -> DefaultSolution<f32> {
    DefaultSolver_solution::<f32>(solver)
}