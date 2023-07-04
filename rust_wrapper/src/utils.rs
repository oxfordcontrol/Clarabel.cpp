use std::mem::forget;

use crate::algebra::CscMatrix;
use clarabel::algebra as lib;

pub unsafe fn convert_from_C_CscMatrix(ptr: *const CscMatrix<f64>) -> lib::CscMatrix<f64> {
    let matrix = match ptr.as_ref() {
        Some(mat) => mat,
        None => panic!("Null pointer passed to convert_from_C_CscMatrix"),
    };

    let m = matrix.m;
    let n = matrix.n;

    // Recover the vectors from the raw array pointers from C
    
    // Length of colptr is always n + 1
    let colptr = Vec::from_raw_parts(matrix.colptr as *mut usize, n + 1, n + 1);

    // Length of rowval and nzval is given by colptr[n]
    let rowval = Vec::from_raw_parts(matrix.rowval as *mut usize, colptr[n], colptr[n]);
    let nzval_vec = Vec::from_raw_parts(matrix.nzval as *mut f64, colptr[n], colptr[n]);

    // Ensure Rust does not try to deallocate memory of arrays managed by C
    forget(colptr);
    forget(rowval);
    forget(nzval_vec);

    // Call the CscMatrix constructor defined in Rust
    let rust_matrix = lib::CscMatrix::<f64> {
        m,
        n,
        colptr,
        rowval,
        nzval: nzval_vec
    };

    // Return the Rust CscMatrix built from the C struct
    rust_matrix
}
