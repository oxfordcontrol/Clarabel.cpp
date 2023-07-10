pub mod implementations {
    pub mod default {

        use crate::algebra::CscMatrix;
        use crate::utils::{self, *};
        use clarabel::algebra::FloatT;
        use clarabel::solver::SupportedConeT::NonnegativeConeT;
        use clarabel::solver::{self as lib, IPSolver};
        use std::{ffi::c_void, mem::forget};

        pub type DefaultSolver = c_void;

        #[repr(C)]
        pub struct DefaultSettings<T: FloatT> {
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
            //pub direct_solve_method: String, // TODO:

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

        #[no_mangle]
        pub extern "C" fn DefaultSettingsBuilder_f64_default() -> DefaultSettings<f64> {
            let default = clarabel::solver::DefaultSettings::<f64>::default();
            
            // Assign all fields to the C struct
            DefaultSettings::<f64>{
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
        #[allow(non_snake_case)]
        #[allow(unused_variables)]
        pub unsafe extern "C" fn DefaultSolver_new(
            P: *const CscMatrix<f64>,
            q: *const f64, // Array of double from C
            A: *const CscMatrix<f64>,
            b: *const f64,         // Array of double from C
            _n_cones: usize,       // Number of cones
            _cones: *const c_void, // TODO:
            settings: *const DefaultSettings<f64>,
        ) -> *mut DefaultSolver {
            let P = convert_from_C_CscMatrix(P);
            let A = convert_from_C_CscMatrix(A);
            let q = Vec::from_raw_parts(q as *mut f64, A.n, A.n);
            let b = Vec::from_raw_parts(b as *mut f64, A.m, A.m);

            // Get a reference to the DefaultSettings struct from the pointer passed from C
            let settings_struct = &*(settings as *const DefaultSettings<f64>);
            let settings = utils::get_solver_settings_from_c::<f64>(settings_struct);

            // example_lp cones for testing
            // TODO: use array of cones from C
            let cones = [NonnegativeConeT(4)];

            // Create the solver
            let solver = lib::DefaultSolver::new(&P, &q, &A, &b, &cones, settings);

            // Ensure Rust does not free the memory of arrays managed by C
            forget(P.colptr);
            forget(P.rowval);
            forget(P.nzval);
            forget(A.colptr);
            forget(A.rowval);
            forget(A.nzval);
            forget(q);
            forget(b);

            // Return the solver object as an opaque pointer to C
            Box::into_raw(Box::new(solver)) as *mut DefaultSolver
        }

        #[no_mangle]
        pub extern "C" fn DefaultSolver_solve(solver: *mut DefaultSolver) {
            // Recover the solver object from the opaque pointer
            let mut solver = unsafe { Box::from_raw(solver as *mut lib::DefaultSolver<f64>) };

            // Use the recovered solver object
            solver.solve();

            // Leave the solver object on the heap
            Box::into_raw(solver);
        }

        #[no_mangle]
        pub unsafe extern "C" fn free_DefaultSolver(solver: *mut DefaultSolver) {
            if !solver.is_null() {
                // Reconstruct the box to drop the solver object
                let boxed = Box::from_raw(solver as *mut lib::DefaultSolver<f64>);
                drop(boxed);
            }
        }
    }
}
