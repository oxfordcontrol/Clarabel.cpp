use crate::algebra::CscMatrix;
use crate::core::cones::SupportedConeT;
use crate::solver::implementations::default::settings::*;
use crate::utils;
use clarabel::solver::{self as lib, IPSolver};
use std::slice;
use std::{ffi::c_void, mem::forget};

pub type DefaultSolver = c_void;

#[no_mangle]
#[allow(non_snake_case)]
#[allow(unused_variables)]
pub unsafe extern "C" fn DefaultSolver_f64_new(
    P: *const CscMatrix<f64>,
    q: *const f64, // Array of double from C
    A: *const CscMatrix<f64>,
    b: *const f64,  // Array of double from C
    n_cones: usize, // Number of cones
    cones: *const SupportedConeT<f64>,
    settings: *const DefaultSettings<f64>,
) -> *mut DefaultSolver {
    // Check null pointers
    if P.is_null() || q.is_null() || A.is_null() || b.is_null() || cones.is_null() {
        return std::ptr::null_mut();
    }

    // Recover the matrices from C structs
    let P = utils::convert_from_C_CscMatrix(P);
    let A = utils::convert_from_C_CscMatrix(A);
    // Recover the arrays from C pointers and deduce their lengths from the matrix dimensions
    let q = Vec::from_raw_parts(q as *mut f64, A.n, A.n);
    let b = Vec::from_raw_parts(b as *mut f64, A.m, A.m);

    // Get a reference to the DefaultSettings struct from the pointer passed from C
    let settings_struct = &*(settings as *const DefaultSettings<f64>);
    let settings = utils::get_solver_settings_from_c::<f64>(settings_struct);

    // Convert the cones from C to Rust
    let c_cones = slice::from_raw_parts(cones, n_cones);
    // c_cones is a slice created from a raw pointer, so it is not managed by Rust, i.e. C side array is not freed when the slice is dropped.
    let cones_vec = utils::convert_from_C_cones(c_cones);
    let cones = cones_vec.as_slice();

    // Create the solver
    // This is dropped at the end of the function because it exists on the Rust side only, and cones and settings are created on the Rust side.
    let solver = lib::DefaultSolver::new(&P, &q, &A, &b, cones, settings);

    // Ensure Rust does not free the memory of arrays managed by C
    forget(P.colptr);
    forget(P.rowval);
    forget(P.nzval);
    forget(A.colptr);
    forget(A.rowval);
    forget(A.nzval);
    forget(q);
    forget(b);

    // Return the solver object as an opaque pointer to C
    Box::into_raw(Box::new(solver)) as *mut DefaultSolver
}

#[no_mangle]
pub extern "C" fn DefaultSolver_solve(solver: *mut DefaultSolver) {
    // Recover the solver object from the opaque pointer
    let mut solver = unsafe { Box::from_raw(solver as *mut lib::DefaultSolver<f64>) };

    // Use the recovered solver object
    solver.solve();

    // Leave the solver object on the heap
    Box::into_raw(solver);
}

#[no_mangle]
pub unsafe extern "C" fn free_DefaultSolver(solver: *mut DefaultSolver) {
    if !solver.is_null() {
        // Reconstruct the box to drop the solver object
        let boxed = Box::from_raw(solver as *mut lib::DefaultSolver<f64>);
        drop(boxed);
    }
}
