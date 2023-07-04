
pub mod implementations {
    pub mod default {
    
        use std::ffi::c_void;
        use crate::algebra::CscMatrix;
        
        pub type DefaultSolver = c_void;

        #[no_mangle]
        pub extern fn DefaultSolver_new(
            P: *const CscMatrix<f64>,
            q: *const f64,              // Array of double from C
            A: *const CscMatrix<f64>,
            b: *const f64,              // Array of double from C
            n_cones: usize,             // Number of cones
            cones: *const c_void,       // TODO:
            settings: *const c_void,    // TODO:
        )-> *mut DefaultSolver {
            // let solver = Box::new(DefaultSolver::new());
            // Box::into_raw(solver)
            unimplemented!()
        }

    }
}
