use super::solver::ClarabelSolverStatus;
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

    pub status: ClarabelSolverStatus,
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
                lib::SolverStatus::Unsolved => ClarabelSolverStatus::ClarabelUnsolved,
                lib::SolverStatus::Solved => ClarabelSolverStatus::ClarabelSolved,
                lib::SolverStatus::PrimalInfeasible => ClarabelSolverStatus::ClarabelPrimalInfeasible,
                lib::SolverStatus::DualInfeasible => ClarabelSolverStatus::ClarabelDualInfeasible,
                lib::SolverStatus::AlmostSolved => ClarabelSolverStatus::ClarabelAlmostSolved,
                lib::SolverStatus::AlmostPrimalInfeasible => ClarabelSolverStatus::ClarabelAlmostPrimalInfeasible,
                lib::SolverStatus::AlmostDualInfeasible => ClarabelSolverStatus::ClarabelAlmostDualInfeasible,
                lib::SolverStatus::MaxIterations => ClarabelSolverStatus::ClarabelMaxIterations,
                lib::SolverStatus::MaxTime => ClarabelSolverStatus::ClarabelMaxTime,
                lib::SolverStatus::NumericalError => ClarabelSolverStatus::ClarabelNumericalError,
                lib::SolverStatus::InsufficientProgress => ClarabelSolverStatus::ClarabelInsufficientProgress,
            },
            obj_val: solution.obj_val,
            solve_time: solution.solve_time,
            iterations: solution.iterations,
            r_prim: solution.r_prim,
            r_dual: solution.r_dual,
        }
    }
}
