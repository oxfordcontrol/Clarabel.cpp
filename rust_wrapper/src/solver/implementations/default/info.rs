use clarabel::{algebra::FloatT, solver::DefaultInfo};

use crate::solver::implementations::default::solver::ClarabelSolverStatus;

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
}

impl<T: FloatT> From<&mut DefaultInfo<T>> for ClarabelDefaultInfo<T> {
    fn from(info: &mut DefaultInfo<T>) -> Self {
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
            status: ClarabelSolverStatus::from(&mut info.status),
        }
    }
}
