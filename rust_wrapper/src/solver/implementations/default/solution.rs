use super::solver::SolverStatus;
use clarabel::algebra::FloatT;
use clarabel::solver as lib;

#[repr(C)]
pub struct DefaultSolution<T: FloatT> {
    pub x: *mut T,
    pub x_length: usize,

    pub z: *mut T,
    pub z_length: usize,

    pub s: *mut T,
    pub s_length: usize,

    pub status: SolverStatus,
    pub obj_val: T,
    pub solve_time: f64,
    pub iterations: u32,
    pub r_prim: T,
    pub r_dual: T,
}

impl<T: FloatT> DefaultSolution<T> {
    pub fn from(solution: &mut lib::DefaultSolution<T>) -> Self {
        Self {
            x: solution.x.as_mut_ptr(),
            x_length: solution.x.len(),
            z: solution.z.as_mut_ptr(),
            z_length: solution.z.len(),
            s: solution.s.as_mut_ptr(),
            s_length: solution.s.len(),
            status: match solution.status {
                lib::SolverStatus::Unsolved => SolverStatus::Unsolved,
                lib::SolverStatus::Solved => SolverStatus::Solved,
                lib::SolverStatus::PrimalInfeasible => SolverStatus::PrimalInfeasible,
                lib::SolverStatus::DualInfeasible => SolverStatus::DualInfeasible,
                lib::SolverStatus::AlmostSolved => SolverStatus::AlmostSolved,
                lib::SolverStatus::AlmostPrimalInfeasible => SolverStatus::AlmostPrimalInfeasible,
                lib::SolverStatus::AlmostDualInfeasible => SolverStatus::AlmostDualInfeasible,
                lib::SolverStatus::MaxIterations => SolverStatus::MaxIterations,
                lib::SolverStatus::MaxTime => SolverStatus::MaxTime,
                lib::SolverStatus::NumericalError => SolverStatus::NumericalError,
                lib::SolverStatus::InsufficientProgress => SolverStatus::InsufficientProgress,
            },
            obj_val: solution.obj_val,
            solve_time: solution.solve_time,
            iterations: solution.iterations,
            r_prim: solution.r_prim,
            r_dual: solution.r_dual,
        }
    }
}
