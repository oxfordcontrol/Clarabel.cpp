#![allow(non_snake_case)]
#![allow(non_camel_case_types)]
#![allow(dead_code)]

pub type ClarabelDefaultInfo<T> =
    clarabel::solver::implementations::default::ffi::DefaultInfoFFI<T>;

pub type ClarabelDefaultInfo_f64 = ClarabelDefaultInfo<f64>;
pub type ClarabelDefaultInfo_f32 = ClarabelDefaultInfo<f32>;
