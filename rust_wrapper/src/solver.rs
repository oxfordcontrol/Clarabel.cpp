pub mod implementations {
    pub mod default {

        use crate::algebra::CscMatrix;
        use crate::utils::*;
        use clarabel::solver::{self as lib, IPSolver};
        use clarabel::solver::{DefaultSettingsBuilder, SupportedConeT::NonnegativeConeT};
        use std::{ffi::c_void, mem::forget};

        pub type DefaultSolver = c_void;

        #[no_mangle]
        #[allow(non_snake_case)]
        #[allow(unused_variables)]
        pub unsafe extern "C" fn DefaultSolver_new(
            P: *const CscMatrix<f64>,
            q: *const f64, // Array of double from C
            A: *const CscMatrix<f64>,
            b: *const f64,            // Array of double from C
            _n_cones: usize,          // Number of cones
            _cones: *const c_void,    // TODO:
            _settings: *const c_void, // TODO:
        ) -> *mut DefaultSolver {
            let P = convert_from_C_CscMatrix(P);
            let A = convert_from_C_CscMatrix(A);
            let q = Vec::from_raw_parts(q as *mut f64, A.n, A.n);
            let b = Vec::from_raw_parts(b as *mut f64, A.m, A.m);

            // Use default settings
            let settings = DefaultSettingsBuilder::<f64>::default()
                .equilibrate_enable(true)
                .max_iter(50)
                .build()
                .unwrap();

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
            let mut solver = *unsafe { Box::from_raw(solver as *mut lib::DefaultSolver<f64>) };
            solver.solve();
        }

        #[no_mangle]
        pub extern "C" fn free_DefaultSolver(solver: *mut DefaultSolver) {
            if !solver.is_null() {
                unsafe {
                    // Reconstruct the box to drop the solver object
                    drop(Box::from_raw(solver as *mut DefaultSolver));
                }
            }
        }
    }
}
