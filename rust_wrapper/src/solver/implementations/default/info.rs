#![allow(non_snake_case)]

pub type ClarabelLinearSolverInfo = clarabel::solver::ffi::LinearSolverInfoFFI;
pub type ClarabelDefaultInfo<T> =
    clarabel::solver::implementations::default::ffi::DefaultInfoFFI<T>;
