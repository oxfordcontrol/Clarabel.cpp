use crate::algebra::CscMatrix;
use crate::utils::{self, *};
use crate::solver::implementations::default::settings::*;
use clarabel::solver::SupportedConeT::NonnegativeConeT;
use clarabel::solver::{self as lib, IPSolver};
use std::{ffi::c_void, mem::forget};

pub type DefaultSolver = c_void;

#[no_mangle]
#[allow(non_snake_case)]
#[allow(unused_variables)]
pub unsafe extern "C" fn DefaultSolver_new(
    P: *const CscMatrix<f64>,
    q: *const f64, // Array of double from C
    A: *const CscMatrix<f64>,
    b: *const f64,         // Array of double from C
    _n_cones: usize,       // Number of cones
    _cones: *const c_void, // TODO:
    settings: *const DefaultSettings<f64>,
) -> *mut DefaultSolver {
    let P = convert_from_C_CscMatrix(P);
    let A = convert_from_C_CscMatrix(A);
    let q = Vec::from_raw_parts(q as *mut f64, A.n, A.n);
    let b = Vec::from_raw_parts(b as *mut f64, A.m, A.m);

    // Get a reference to the DefaultSettings struct from the pointer passed from C
    let settings_struct = &*(settings as *const DefaultSettings<f64>);
    let settings = utils::get_solver_settings_from_c::<f64>(settings_struct);

    // example_lp cones for testing
    // TODO: use array of cones from C
    let cones = [NonnegativeConeT(4)];

    // Create the solver
    let solver = lib::DefaultSolver::new(&P, &q, &A, &b, &cones, settings);

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
