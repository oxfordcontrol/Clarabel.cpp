use crate::algebra::ClarabelCscMatrix;
use clarabel::algebra as lib;
use clarabel::algebra::FloatT;

/// Convert a CscMatrix from C to Rust
///
/// The CscMatrix object returned will take ownership of the memory of arrays allocated in C.
/// Make sure to call std::mem::forget on the vectors in the CscMatrix object to leave the memory management for C side
#[allow(non_snake_case)]
pub unsafe fn convert_from_C_CscMatrix<T: FloatT>(ptr: *const ClarabelCscMatrix<T>) -> lib::CscMatrix<T> {
    // Recover the CscMatrix from the raw pointer from C
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
    let nzval_vec = Vec::from_raw_parts(matrix.nzval as *mut T, colptr[n], colptr[n]);

    // Call the CscMatrix constructor defined in Rust
    let rust_matrix = lib::CscMatrix::<T> {
        m,
        n,
        colptr,
        rowval,
        nzval: nzval_vec,
    };

    // Return the Rust CscMatrix built from the C struct
    rust_matrix
}
