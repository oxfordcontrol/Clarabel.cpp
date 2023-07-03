use clarabel::algebra as lib;
use std::{ffi::c_void, usize};

//pub type CscMatrixF64 = c_void;

#[no_mangle]
pub extern "C" fn CscMatrix_new(
    m: usize,
    n: usize,
    colptr: *const usize,
    rowval: *const usize,
    nzval: *const f64,
) -> *mut c_void {
    // Recover the vectors from the raw array pointers

    // Length of colptr is n + 1
    let colptr_vec = unsafe { Vec::from_raw_parts(colptr as *mut usize, n + 1, n + 1) };
    let num_of_nzvalues = colptr_vec[n]; // Length of rowval and nzval is colptr[n]
    let rowval_vec =
        unsafe { Vec::from_raw_parts(rowval as *mut usize, num_of_nzvalues, num_of_nzvalues) };
    let nzval_vec =
        unsafe { Vec::from_raw_parts(nzval as *mut f64, num_of_nzvalues, num_of_nzvalues) };

    // Call the CscMatrix constructor defined in Rust
    let matrix = lib::CscMatrix::<f64>::new(m, n, colptr_vec, rowval_vec, nzval_vec);

    // Print the matrix in debug mode
    println!("{:?}", matrix);

    // Return a raw pointer to the boxed lib::Cscmatrix<f64> object
    // The matrix is allocated on the heap and the pointer is returned for C to use
    let boxed = Box::new(matrix);
    let ptr: *mut lib::CscMatrix<f64> = Box::into_raw(boxed);
    ptr as *mut c_void
}
