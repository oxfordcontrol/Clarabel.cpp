#![allow(non_snake_case)]

pub type ClarabelDefaultInfo<T> =
    clarabel::solver::implementations::default::ffi::DefaultInfoFFI<T>;

pub type ClarabelDefaultInfo_f64 = ClarabelDefaultInfo<f64>;
pub type ClarabelDefaultInfo_f32 = ClarabelDefaultInfo<f32>;
