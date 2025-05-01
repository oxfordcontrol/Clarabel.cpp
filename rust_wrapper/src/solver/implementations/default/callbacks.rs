#![allow(non_snake_case)]

use super::solver::*;
use crate::solver::implementations::default::info::ClarabelDefaultInfo;
use clarabel::algebra::FloatT;
use clarabel::solver::{self as lib};
use std::ffi::{c_int, c_void};

pub(crate) type CallbackFcnFFI<T> = extern "C" fn(info: *const ClarabelDefaultInfo<T>) -> c_int;
pub type ClarabelCallbackFcn_f32 = CallbackFcnFFI<f32>;
pub type ClarabelCallbackFcn_f64 = CallbackFcnFFI<f64>;

/// Set the termination callback
fn _internal_DefaultSolver_set_termination_callback<T: FloatT>(
    solver: *mut c_void,
    callback: CallbackFcnFFI<T>,
) {
    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };

    // Set the termination callback
    solver.set_termination_callback_c(callback);
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSolver_f64_set_termination_callback(
    solver: *mut ClarabelDefaultSolver_f64,
    callback: ClarabelCallbackFcn_f64,
) {
    _internal_DefaultSolver_set_termination_callback::<f64>(solver, callback)
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSolver_f32_set_termination_callback(
    solver: *mut ClarabelDefaultSolver_f32,
    callback: ClarabelCallbackFcn_f32,
) {
    _internal_DefaultSolver_set_termination_callback::<f32>(solver, callback)
}

/// Turn off the termination callback
fn _internal_DefaultSolver_unset_termination_callback<T: FloatT>(solver: *mut c_void) {
    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };

    // Set the termination callback
    solver.unset_termination_callback();
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSolver_f64_unset_termination_callback(
    solver: *mut ClarabelDefaultSolver_f64,
) {
    _internal_DefaultSolver_unset_termination_callback::<f64>(solver)
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSolver_f32_unset_termination_callback(
    solver: *mut ClarabelDefaultSolver_f32,
) {
    _internal_DefaultSolver_unset_termination_callback::<f32>(solver)
}
