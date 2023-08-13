use clarabel::algebra::FloatT;

/// Used to replace the String type in the DefaultSettings struct
#[repr(C)]
pub enum ClarabelDirectSolveMethods {
    QDLDL,
    // MKL, (not supported yet)
    // CHOLMOD, (not supported yet)
}

/// DefaultSettings struct used by the C side
#[repr(C)]
pub struct ClarabelDefaultSettings<T: FloatT> {
    // Main algorithm settings
    pub max_iter: u32,
    pub time_limit: f64,
    pub verbose: bool,
    pub max_step_fraction: T,

    // Full accuracy settings
    pub tol_gap_abs: T,
    pub tol_gap_rel: T,
    pub tol_feas: T,
    pub tol_infeas_abs: T,
    pub tol_infeas_rel: T,
    pub tol_ktratio: T,

    // Reduced accuracy settings
    pub reduced_tol_gap_abs: T,
    pub reduced_tol_gap_rel: T,
    pub reduced_tol_feas: T,
    pub reduced_tol_infeas_abs: T,
    pub reduced_tol_infeas_rel: T,
    pub reduced_tol_ktratio: T,

    // data equilibration settings
    pub equilibrate_enable: bool,
    pub equilibrate_max_iter: u32,
    pub equilibrate_min_scaling: T,
    pub equilibrate_max_scaling: T,

    // Step size settings
    pub linesearch_backtrack_step: T,
    pub min_switch_step_length: T,
    pub min_terminate_step_length: T,

    // Linear solver settings
    pub direct_kkt_solver: bool,
    pub direct_solve_method: ClarabelDirectSolveMethods,

    // static regularization parameters
    pub static_regularization_enable: bool,
    pub static_regularization_constant: T,
    pub static_regularization_proportional: T,

    // dynamic regularization parameters
    pub dynamic_regularization_enable: bool,
    pub dynamic_regularization_eps: T,
    pub dynamic_regularization_delta: T,

    // iterative refinement (for direct solves)
    pub iterative_refinement_enable: bool,
    pub iterative_refinement_reltol: T,
    pub iterative_refinement_abstol: T,
    pub iterative_refinement_max_iter: u32,
    pub iterative_refinement_stop_ratio: T,

    // preprocessing
    pub presolve_enable: bool,
}

/// Wrapper function for DefaultSettings::default()
///
/// Get the default settings for the solver
#[allow(non_snake_case)]
fn _internal_DefaultSettings_default<T: FloatT>() -> ClarabelDefaultSettings<T> {
    let default = clarabel::solver::DefaultSettings::<T>::default();
    let default_direct_solver_setting = match default.direct_solve_method.as_str() {
        "qdldl" => ClarabelDirectSolveMethods::QDLDL,
        //"mkl" => ClarabelDirectSolveMethods::MKL,
        //"cholmod" => ClarabelDirectSolveMethods::CHOLMOD,
        _ => ClarabelDirectSolveMethods::QDLDL, // Default
    };

    // Assign all fields to the C struct
    ClarabelDefaultSettings::<T> {
        max_iter: default.max_iter,
        time_limit: default.time_limit,
        verbose: default.verbose,
        max_step_fraction: default.max_step_fraction,
        tol_gap_abs: default.tol_gap_abs,
        tol_gap_rel: default.tol_gap_rel,
        tol_feas: default.tol_feas,
        tol_infeas_abs: default.tol_infeas_abs,
        tol_infeas_rel: default.tol_infeas_rel,
        tol_ktratio: default.tol_ktratio,
        reduced_tol_gap_abs: default.reduced_tol_gap_abs,
        reduced_tol_gap_rel: default.reduced_tol_gap_rel,
        reduced_tol_feas: default.reduced_tol_feas,
        reduced_tol_infeas_abs: default.reduced_tol_infeas_abs,
        reduced_tol_infeas_rel: default.reduced_tol_infeas_rel,
        reduced_tol_ktratio: default.reduced_tol_ktratio,
        equilibrate_enable: default.equilibrate_enable,
        equilibrate_max_iter: default.equilibrate_max_iter,
        equilibrate_min_scaling: default.equilibrate_min_scaling,
        equilibrate_max_scaling: default.equilibrate_max_scaling,
        linesearch_backtrack_step: default.linesearch_backtrack_step,
        min_switch_step_length: default.min_switch_step_length,
        min_terminate_step_length: default.min_terminate_step_length,
        direct_kkt_solver: default.direct_kkt_solver,
        direct_solve_method: default_direct_solver_setting,
        static_regularization_enable: default.static_regularization_enable,
        static_regularization_constant: default.static_regularization_constant,
        static_regularization_proportional: default.static_regularization_proportional,
        dynamic_regularization_enable: default.dynamic_regularization_enable,
        dynamic_regularization_eps: default.dynamic_regularization_eps,
        dynamic_regularization_delta: default.dynamic_regularization_delta,
        iterative_refinement_enable: default.iterative_refinement_enable,
        iterative_refinement_reltol: default.iterative_refinement_reltol,
        iterative_refinement_abstol: default.iterative_refinement_abstol,
        iterative_refinement_max_iter: default.iterative_refinement_max_iter,
        iterative_refinement_stop_ratio: default.iterative_refinement_stop_ratio,
        presolve_enable: default.presolve_enable,
    }
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSettings_f64_default() -> ClarabelDefaultSettings<f64> {
    _internal_DefaultSettings_default::<f64>()
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSettings_f32_default() -> ClarabelDefaultSettings<f32> {
    _internal_DefaultSettings_default::<f32>()
}
