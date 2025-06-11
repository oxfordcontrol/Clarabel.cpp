#![allow(non_snake_case)]
#![allow(non_camel_case_types)]

use crate::algebra::ClarabelCscMatrix;
use crate::core::cones::ClarabelSupportedConeT;
use crate::solver::implementations::default::settings::{
    ClarabelDefaultSettings, ClarabelDefaultSettings_f32, ClarabelDefaultSettings_f64,
};
use crate::utils;
use clarabel::solver::ffi::SolverStatusFFI;

use clarabel::algebra::FloatT;
use clarabel::io::ConfigurablePrintTarget;
use clarabel::solver::{self as lib, IPSolver};

use std::ffi::c_char;
use std::slice;
use std::{ffi::c_void, mem::forget};

cfg_if::cfg_if! {
    if #[cfg(feature = "serde")] {
        use serde::{de::DeserializeOwned, Serialize};
        use clarabel::solver::SolverJSONReadWrite;
    }
}

use super::info::ClarabelDefaultInfo;
use super::solution::DefaultSolution;

pub type ClarabelDefaultSolver_f32 = c_void;
pub type ClarabelDefaultSolver_f64 = c_void;

pub type ClarabelSolverStatus = SolverStatusFFI;

// Wrapper function to create a DefaultSolver object from C using dynamic memory allocation
// - Matrices and vectors are constructed from raw pointers
// - Cones are converted from C struct to Rust struct
// - Settings are converted from C struct to Rust struct
//
// b and cones are allowed to be null pointers, in which case they form zero-length slices and this is consistent with Clarabel.rs.
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
    let q = match q.is_null() {
        true => Vec::new(),
        false => Vec::from_raw_parts(q as *mut T, P.n, P.n),
    };
    let b = match b.is_null() {
        true => Vec::new(),
        false => Vec::from_raw_parts(b as *mut T, A.m, A.m),
    };

    // Get a reference to the DefaultSettings struct from the pointer passed from C
    let settings = (*settings).clone().into();

    // Convert the cones from C to Rust
    let cones = match cones.is_null() {
        true => Vec::new(),
        false => {
            let c_cones = slice::from_raw_parts(cones, n_cones);
            utils::convert_from_C_cones(c_cones)
        }
    };

    // Create the solver
    // This is dropped at the end of the function because it exists on the Rust side only,
    // and cones and settings are created on the Rust side.
    let solver = lib::DefaultSolver::<T>::new(&P, &q, &A, &b, &cones, settings);

    // Ensure Rust does not free the memory of arrays managed by C
    // Should be fine to forget vectors that were created as zero-length
    // vecs when receiving null pointers, since rust vec::new() should
    // not allocate memory for zero-length vectors.
    forget(P);
    forget(A);
    forget(q);
    forget(b);

    // Solver should be a Result<DefaultSolver<T>, SolverError>
    match solver {
        Ok(solver) => Box::into_raw(Box::new(solver)) as *mut c_void,
        Err(e) => {
            // Just print an error here and return a null pointer
            // This could surely done in a more graceful way
            println!("Error creating DefaultSolver: {:?}", e);
            std::ptr::null_mut()
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_DefaultSolver_f64_new(
    P: *const ClarabelCscMatrix<f64>,
    q: *const f64,
    A: *const ClarabelCscMatrix<f64>,
    b: *const f64,
    n_cones: usize,
    cones: *const ClarabelSupportedConeT<f64>,
    settings: *const ClarabelDefaultSettings_f64,
) -> *mut ClarabelDefaultSolver_f64 {
    _internal_DefaultSolver_new(P, q, A, b, n_cones, cones, settings)
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_DefaultSolver_f32_new(
    P: *const ClarabelCscMatrix<f32>,
    q: *const f32,
    A: *const ClarabelCscMatrix<f32>,
    b: *const f32,
    n_cones: usize,
    cones: *const ClarabelSupportedConeT<f32>,
    settings: *const ClarabelDefaultSettings_f32,
) -> *mut ClarabelDefaultSolver_f32 {
    _internal_DefaultSolver_new(P, q, A, b, n_cones, cones, settings)
}

// Wrapper function to call DefaultSolver.solve() from C
fn _internal_DefaultSolver_solve<T: FloatT>(solver: *mut c_void) {
    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };

    // Use the recovered solver object
    solver.solve();
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

fn _internal_DefaultSolver_print_to_stdout<T>(solver: *mut c_void)
where
    T: FloatT,
{
    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };

    // Use the recovered solver object
    solver.print_to_stdout();
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_DefaultSolver_f64_print_to_stdout(
    solver: *mut ClarabelDefaultSolver_f64,
) {
    _internal_DefaultSolver_print_to_stdout::<f64>(solver);
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_DefaultSolver_f32_print_to_stdout(
    solver: *mut ClarabelDefaultSolver_f32,
) {
    _internal_DefaultSolver_print_to_stdout::<f32>(solver);
}

fn _internal_DefaultSolver_print_to_file<T>(solver: *mut c_void, filename: *const c_char)
where
    T: FloatT,
{
    if filename.is_null() {
        return;
    }
    let filename = unsafe {
        std::ffi::CStr::from_ptr(filename)
            .to_str()
            .expect("Invalid filename")
    };
    let file = std::fs::File::create(filename).expect("File not found");

    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };

    // Use the recovered solver object
    solver.print_to_file(file);
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_DefaultSolver_f64_print_to_file(
    solver: *mut ClarabelDefaultSolver_f64,
    filename: *const c_char,
) {
    _internal_DefaultSolver_print_to_file::<f64>(solver, filename);
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_DefaultSolver_f32_print_to_file(
    solver: *mut ClarabelDefaultSolver_f32,
    filename: *const c_char,
) {
    _internal_DefaultSolver_print_to_file::<f32>(solver, filename);
}

fn _internal_DefaultSolver_print_to_buffer<T>(solver: *mut c_void)
where
    T: FloatT,
{
    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };

    // Use the recovered solver object
    solver.print_to_buffer();
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_DefaultSolver_f64_print_to_buffer(
    solver: *mut ClarabelDefaultSolver_f64,
) {
    _internal_DefaultSolver_print_to_buffer::<f64>(solver);
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_DefaultSolver_f32_print_to_buffer(
    solver: *mut ClarabelDefaultSolver_f32,
) {
    _internal_DefaultSolver_print_to_buffer::<f32>(solver);
}

fn _internal_DefaultSolver_get_from_buffer<T>(solver: *mut c_void) -> *const c_char
where
    T: FloatT,
{
    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };
    let out = solver.get_print_buffer().unwrap_or("".to_string());
    let c_str = std::ffi::CString::new(out).unwrap();
    // Return the string as a raw pointer.  It must be returned to
    // rust to freed here.  Can't call free from C on it.
    c_str.into_raw()
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_DefaultSolver_f64_get_print_buffer(
    solver: *mut ClarabelDefaultSolver_f64,
) -> *const c_char {
    _internal_DefaultSolver_get_from_buffer::<f64>(solver)
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_DefaultSolver_f32_get_print_buffer(
    solver: *mut ClarabelDefaultSolver_f32,
) -> *const c_char {
    _internal_DefaultSolver_get_from_buffer::<f32>(solver)
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_free_print_buffer(ptr: *const c_char) {
    // This function should be called from C to free the memory of the string
    // returned by get_from_buffer.
    unsafe {
        let _ = std::ffi::CString::from_raw(ptr as *mut c_char);
    }
}

#[cfg(feature = "serde")]
pub unsafe fn _internal_DefaultSolver_load_from_file<T>(
    filename: *const c_char,
    settings: *const ClarabelDefaultSettings<T>,
) -> *mut c_void
where
    T: FloatT + DeserializeOwned + Serialize,
{
    if filename.is_null() {
        return std::ptr::null_mut();
    }
    let filename = unsafe {
        std::ffi::CStr::from_ptr(filename)
            .to_str()
            .expect("Invalid filename")
    };
    let mut file = std::fs::File::open(filename).expect("File not found");

    let solver = if settings.is_null() {
        lib::DefaultSolver::<T>::load_from_file(&mut file, None)
    } else {
        let settings = (*settings).clone().into();
        lib::DefaultSolver::<T>::load_from_file(&mut file, Some(settings))
    };
    Box::into_raw(Box::new(solver)) as *mut c_void
}

#[cfg(feature = "serde")]
fn _internal_DefaultSolver_save_to_file<T>(solver: *mut c_void, filename: *const c_char)
where
    T: FloatT + DeserializeOwned + Serialize,
{
    if filename.is_null() {
        return;
    }
    let filename = unsafe {
        std::ffi::CStr::from_ptr(filename)
            .to_str()
            .expect("Invalid filename")
    };
    let mut file = std::fs::File::create(filename).expect("File not found");

    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };

    // Use the recovered solver object
    solver.save_to_file(&mut file).unwrap();
}

#[no_mangle]
#[cfg(feature = "serde")]
pub unsafe extern "C" fn clarabel_DefaultSolver_f64_load_from_file(
    filename: *const c_char,
    settings: *const ClarabelDefaultSettings<f64>,
) -> *mut ClarabelDefaultSolver_f64 {
    _internal_DefaultSolver_load_from_file::<f64>(filename, settings)
}

#[no_mangle]
#[cfg(feature = "serde")]
pub unsafe extern "C" fn clarabel_DefaultSolver_f32_load_from_file(
    filename: *const c_char,
    settings: *const ClarabelDefaultSettings<f32>,
) -> *mut ClarabelDefaultSolver_f32 {
    _internal_DefaultSolver_load_from_file::<f32>(filename, settings)
}

#[no_mangle]
#[cfg(feature = "serde")]
pub extern "C" fn clarabel_DefaultSolver_f64_save_to_file(
    solver: *mut ClarabelDefaultSolver_f64,
    filename: *const c_char,
) {
    _internal_DefaultSolver_save_to_file::<f64>(solver, filename);
}

#[no_mangle]
#[cfg(feature = "serde")]
pub extern "C" fn clarabel_DefaultSolver_f32_save_to_file(
    solver: *mut ClarabelDefaultSolver_f32,
    filename: *const c_char,
) {
    _internal_DefaultSolver_save_to_file::<f32>(solver, filename);
}

/// Get the solution field from a DefaultSolver object.
///
/// The solution is returned as a C struct.
fn _internal_DefaultSolver_solution<T: FloatT>(solver: *mut c_void) -> DefaultSolution<T> {
    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };

    // Get the solution and convert to C struct
    DefaultSolution::<T>::from(&mut solver.solution)
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
fn _internal_DefaultSolver_info<T: FloatT>(solver: *mut c_void) -> ClarabelDefaultInfo<T> {
    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };

    // Get the info field and convert it to a C struct.
    ClarabelDefaultInfo::<T>::from(solver.info.clone())
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
