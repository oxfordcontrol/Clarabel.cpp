use clarabel::algebra as lib;
//use std::{ffi::c_void, usize};

use std::slice;

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

#[no_mangle]
pub extern "C" fn CscMatrix_f64_from(m: usize, n: usize, matrix: *const f64) -> CscMatrix<f64> {
    // Check if the pointer is null
    if matrix.is_null() {
        #[cfg(debug_assertions)]
        println!("Null pointer received");

        return CscMatrix::<f64> {
            m: 0,
            n: 0,
            colptr: std::ptr::null(),
            rowval: std::ptr::null(),
            nzval: std::ptr::null(),
        };
    } 

    // Construct a flattened slice from the raw pointer.
    let raw_slice = unsafe { slice::from_raw_parts(matrix, m * n) };

    // Convert the 1D slice into a 2D array.
    let mut matrix_slice = Vec::with_capacity(m);
    for row in 0..m {
        let start = row * n;
        let end = start + n;
        matrix_slice.push(&raw_slice[start..end]);
    }

    // Call the function construct the CscMatrix
    let csc_matrix = lib::CscMatrix::<f64>::from(matrix_slice);

    // Build the C struct
    let result = CscMatrix::<f64> {
        m: csc_matrix.m,
        n: csc_matrix.n,
        colptr: csc_matrix.colptr.as_ptr(),
        rowval: csc_matrix.rowval.as_ptr(),
        nzval: csc_matrix.nzval.as_ptr(),
    };

    // Forget the vectors to leave the memory management to C
    std::mem::forget(csc_matrix.colptr);
    std::mem::forget(csc_matrix.rowval);
    std::mem::forget(csc_matrix.nzval);

    // Return the C struct
    result
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
