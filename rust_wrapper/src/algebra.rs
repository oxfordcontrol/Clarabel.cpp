//use clarabel::algebra as lib;
//use std::{ffi::c_void, usize};

#[repr(C)]
// This struct is for interfacing with C only and should be synched with clarabel::algebra::CscMatrix
pub struct CscMatrix<T = f64> {
    /// number of rows
    pub m: usize,
    /// number of columns
    pub n: usize,
    /// CSC format column pointer.   
    ///
    /// Ths field should have length `n+1`. The last entry corresponds
    /// to the the number of nonzeros and should agree with the lengths
    /// of the `rowval` and `nzval` fields.
    pub colptr: *const usize,
    /// vector of row indices
    pub rowval: *const usize,
    /// vector of non-zero matrix elements
    pub nzval: *const T,
}

// pub type CscMatrixF64 = c_void;

// #[no_mangle]
// pub extern "C" fn CscMatrix_new(
//     m: usize,
//     n: usize,
//     colptr: *const usize,
//     rowval: *const usize,
//     nzval: *const f64,
// ) -> *mut CscMatrixF64 {
//     // Recover the vectors from the raw array pointers

//     // Length of colptr is n + 1
//     let colptr_vec = unsafe { Vec::from_raw_parts(colptr as *mut usize, n + 1, n + 1) };
//     let num_of_nzvalues = colptr_vec[n]; // Length of rowval and nzval is colptr[n]
//     let rowval_vec =
//         unsafe { Vec::from_raw_parts(rowval as *mut usize, num_of_nzvalues, num_of_nzvalues) };
//     let nzval_vec =
//         unsafe { Vec::from_raw_parts(nzval as *mut f64, num_of_nzvalues, num_of_nzvalues) };

//     // Call the CscMatrix constructor defined in Rust
//     let matrix = lib::CscMatrix::<f64>::new(m, n, colptr_vec, rowval_vec, nzval_vec);

//     // Print the matrix in debug mode
//     #[cfg(debug_assertions)]
//     println!("{:?}", matrix);

//     // Return a raw pointer to the boxed lib::Cscmatrix<f64> object
//     // The matrix is allocated on the heap and the pointer is returned for C to use
//     let boxed = Box::new(matrix);
//     let ptr: *mut lib::CscMatrix<f64> = Box::into_raw(boxed);
//     ptr as *mut CscMatrixF64

//     // TODO: forget arrays from C
// }

// #[no_mangle]
// pub extern "C" fn CscMatrix_delete(ptr: *mut CscMatrixF64) {
//     if !ptr.is_null() {
//         unsafe {
//             // Reconstruct the box to drop the lib::CscMatrix<f64> object
//             drop(Box::from_raw(ptr as *mut CscMatrixF64));
//         }
//     }
// }