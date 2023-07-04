pub mod implementations {
    pub mod default {

        use crate::algebra::CscMatrix;
        use crate::utils::*;
        use clarabel::solver::{SupportedConeT::{NonnegativeConeT, self}, DefaultSettingsBuilder};
        use clarabel::solver as lib;
        use std::{convert, ffi::c_void, mem::forget};

        pub type DefaultSolver = c_void;

        #[no_mangle]
        pub unsafe extern "C" fn DefaultSolver_new(
            P: *const CscMatrix<f64>,
            q: *const f64, // Array of double from C
            A: *const CscMatrix<f64>,
            b: *const f64,           // Array of double from C
            _n_cones: usize,          // Number of cones
            _cones: *const c_void,    // TODO:
            _settings: *const c_void, // TODO:
        ) -> *mut DefaultSolver {
            let P = convert_from_C_CscMatrix(P);
            let q = vec![1., -1.]; // test
            let A = convert_from_C_CscMatrix(A);
            let b = vec![1.; 4]; // test

            // Use default settings
            let settings = DefaultSettingsBuilder::<f64>::default()
                .equilibrate_enable(true)
                .max_iter(50)
                .build()
                .unwrap();
            
            let cones = [NonnegativeConeT(4)];

            let solver = lib::DefaultSolver::new(&P, &q, &A, &b, &cones, settings);

            let solver_ptr = Box::into_raw(Box::new(solver));
            
            // Ensure Rust does not free the memory of arrays managed by C
            forget(P.colptr);
            forget(P.rowval);
            forget(P.nzval);
            forget(A.colptr);
            forget(A.rowval);
            forget(A.nzval);
            forget(q);
            forget(b);
            
            // Return the solver object as an opque pointer to C
            solver_ptr as *mut c_void
        }

    }
}
