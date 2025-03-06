#![allow(non_snake_case)]

use clarabel::{algebra::FloatT, solver::DefaultInfo};
use clarabel::solver::LinearSolverInfo;
use crate::solver::implementations::default::solver::ClarabelSolverStatus;
use super::settings::ClarabelDirectSolveMethods;

#[repr(C)]
pub struct ClarabelLinearSolverInfo {
        pub name: ClarabelDirectSolveMethods,
        pub threads: u32,
        pub direct: bool,
        pub nnzA: u32, 
        pub nnzL: u32, 
}

impl From<&LinearSolverInfo> for ClarabelLinearSolverInfo {
    fn from(linsolver: &LinearSolverInfo) -> Self {
        Self {
            name: (&linsolver.name).into(),
            threads: linsolver.threads as u32,
            direct: linsolver.direct,
            nnzA: linsolver.nnzA as u32,
            nnzL: linsolver.nnzL as u32,
        }
    }
}

#[repr(C)]
pub struct ClarabelDefaultInfo<T> {
    pub μ: T,
    pub sigma: T,
    pub step_length: T,
    pub iterations: u32,
    pub cost_primal: T,
    pub cost_dual: T,
    pub res_primal: T,
    pub res_dual: T,
    pub res_primal_inf: T,
    pub res_dual_inf: T,
    pub gap_abs: T,
    pub gap_rel: T,
    pub ktratio: T,

    pub solve_time: f64,
    pub status: ClarabelSolverStatus,

    pub linsolver: ClarabelLinearSolverInfo,
}

impl<T: FloatT> From<&DefaultInfo<T>> for ClarabelDefaultInfo<T> {
    fn from(info: &DefaultInfo<T>) -> Self {
        Self {
            μ: info.μ,
            sigma: info.sigma,
            step_length: info.step_length,
            iterations: info.iterations,
            cost_primal: info.cost_primal,
            cost_dual: info.cost_dual,
            res_primal: info.res_primal,
            res_dual: info.res_dual,
            res_primal_inf: info.res_primal_inf,
            res_dual_inf: info.res_dual_inf,
            gap_abs: info.gap_abs,
            gap_rel: info.gap_rel,
            ktratio: info.ktratio,
            solve_time: info.solve_time,
            status: ClarabelSolverStatus::from(&info.status),
            linsolver: ClarabelLinearSolverInfo::from(&info.linsolver),
        }
    }
}
