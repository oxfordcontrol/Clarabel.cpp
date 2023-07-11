use clarabel::algebra as lib;
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
    /// Indicates whether the memory of colptr, rowval and nzval is owned by this struct. Should never be changed by the user.
    pub owns_matrix_data: bool,
}

/// Wrapper function for CscMatrix::from()
/// 
/// Creates a CscMatrix object from C using dynamic memory allocation.
/// Values in the dense matrix are copied into the CSC matrix. Therefore, values in the CSC matrix are owned by the struct itself.
#[no_mangle]
pub extern "C" fn CscMatrix_f64_from(
    m: usize,
    n: usize,
    matrix: *const f64,
) -> *mut CscMatrix<f64> {
    // Check if the pointer is null
    if matrix.is_null() {
        #[cfg(debug_assertions)]
        println!("Null pointer received");

        return std::ptr::null_mut();
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

    // Call the function that constructs the CscMatrix
    let csc_matrix = lib::CscMatrix::<f64>::from(matrix_slice);

    // Build the C struct
    let c_struct = CscMatrix::<f64> {
        m: csc_matrix.m,
        n: csc_matrix.n,
        colptr: csc_matrix.colptr.as_ptr(),
        rowval: csc_matrix.rowval.as_ptr(),
        nzval: csc_matrix.nzval.as_ptr(),
        owns_matrix_data: true,
    };

    // Forget the vectors to leave the memory management to C
    std::mem::forget(csc_matrix.colptr);
    std::mem::forget(csc_matrix.rowval);
    std::mem::forget(csc_matrix.nzval);

    // Box the C struct and return its pointer
    Box::into_raw(Box::new(c_struct)) as *mut CscMatrix<f64>
}

/// Wrapper function for CscMatrix::zeros()
/// 
/// Creates a CscMatrix object from C using dynamic memory allocation.
/// Values are owned by the matrix.
#[no_mangle]
pub extern "C" fn CscMatrix_f64_zeros(rows: usize, cols: usize) -> *mut CscMatrix<f64> {
    // Call the function that constructs the CscMatrix
    let csc_matrix = lib::CscMatrix::<f64>::zeros((rows, cols));

    // Build the C struct
    let c_struct = CscMatrix::<f64> {
        m: csc_matrix.m,
        n: csc_matrix.n,
        colptr: csc_matrix.colptr.as_ptr(),
        rowval: csc_matrix.rowval.as_ptr(),
        nzval: csc_matrix.nzval.as_ptr(),
        owns_matrix_data: true,
    };

    // Forget the vectors to leave the memory management to C
    std::mem::forget(csc_matrix.colptr);
    std::mem::forget(csc_matrix.rowval);
    std::mem::forget(csc_matrix.nzval);

    // Box the C struct and return its pointer
    Box::into_raw(Box::new(c_struct)) as *mut CscMatrix<f64>
}

/// Wrapper function for CscMatrix::identity()
/// 
/// Creates a CscMatrix object from C using dynamic memory allocation.
/// Values are owned by the matrix.
#[no_mangle]
pub extern "C" fn CscMatrix_f64_identity(n: usize) -> *mut CscMatrix<f64> {
    // Call the function that constructs the CscMatrix
    let csc_matrix = lib::CscMatrix::<f64>::identity(n);

    // Build the C struct
    let c_struct = CscMatrix::<f64> {
        m: csc_matrix.m,
        n: csc_matrix.n,
        colptr: csc_matrix.colptr.as_ptr(),
        rowval: csc_matrix.rowval.as_ptr(),
        nzval: csc_matrix.nzval.as_ptr(),
        owns_matrix_data: true,
    };

    // Forget the vectors to leave the memory management to C
    std::mem::forget(csc_matrix.colptr);
    std::mem::forget(csc_matrix.rowval);
    std::mem::forget(csc_matrix.nzval);

    // Box the C struct and return its pointer
    Box::into_raw(Box::new(c_struct)) as *mut CscMatrix<f64>
}

/// Function for freeing the memory of a Rust CscMatrix object.
/// 
/// This also frees all the memory of the matrix data.
/// DO NOT use this function if the matrix data is managed by the C side.
#[no_mangle]
pub unsafe extern "C" fn delete_CscMatrix_f64(matrix: *mut CscMatrix<f64>) {
    if matrix.is_null() || !(*matrix).owns_matrix_data {
        return;
    }

    // Free the memory of matrix data
    let csc_matrix = crate::utils::convert_from_C_CscMatrix(matrix);
    drop(csc_matrix);

    // Free the memory of the C struct
    drop(Box::from_raw(matrix));
}
