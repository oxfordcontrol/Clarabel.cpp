use clarabel::algebra as lib;
use clarabel::algebra::FloatT;
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
#[allow(non_snake_case)]
fn CscMatrix_from<T: FloatT>(m: usize, n: usize, matrix: *const T) -> *mut CscMatrix<T> {
    // Check if the pointer is null
    debug_assert!(!matrix.is_null(), "matrix must not be a null pointer");
    if matrix.is_null() {
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
    let csc_matrix = lib::CscMatrix::<T>::from(matrix_slice);

    // Build the C struct
    let c_struct = CscMatrix::<T> {
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
    Box::into_raw(Box::new(c_struct)) as *mut CscMatrix<T>
}

#[no_mangle]
pub extern "C" fn CscMatrix_f64_from(
    m: usize,
    n: usize,
    matrix: *const f64,
) -> *mut CscMatrix<f64> {
    CscMatrix_from(m, n, matrix)
}

#[no_mangle]
pub extern "C" fn CscMatrix_f32_from(
    m: usize,
    n: usize,
    matrix: *const f32,
) -> *mut CscMatrix<f32> {
    CscMatrix_from(m, n, matrix)
}

/// Wrapper function for CscMatrix::zeros()
///
/// Creates a CscMatrix object from C using dynamic memory allocation.
/// Values are owned by the matrix.
#[allow(non_snake_case)]
fn CscMatrix_zeros<T: FloatT>(rows: usize, cols: usize) -> *mut CscMatrix<T> {
    // Call the function that constructs the CscMatrix
    let csc_matrix = lib::CscMatrix::<T>::zeros((rows, cols));

    // Build the C struct
    let c_struct = CscMatrix::<T> {
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
    Box::into_raw(Box::new(c_struct)) as *mut CscMatrix<T>
}

#[no_mangle]
pub extern "C" fn CscMatrix_f64_zeros(rows: usize, cols: usize) -> *mut CscMatrix<f64> {
    CscMatrix_zeros(rows, cols)
}

#[no_mangle]
pub extern "C" fn CscMatrix_f32_zeros(rows: usize, cols: usize) -> *mut CscMatrix<f32> {
    CscMatrix_zeros(rows, cols)
}

/// Wrapper function for CscMatrix::identity()
///
/// Creates a CscMatrix object from C using dynamic memory allocation.
/// Values are owned by the matrix.
#[allow(non_snake_case)]
fn CscMatrix_identity<T: FloatT>(n: usize) -> *mut CscMatrix<T> {
    // Call the function that constructs the CscMatrix
    let csc_matrix = lib::CscMatrix::<T>::identity(n);

    // Build the C struct
    let c_struct = CscMatrix::<T> {
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
    Box::into_raw(Box::new(c_struct)) as *mut CscMatrix<T>
}

#[no_mangle]
pub extern "C" fn CscMatrix_f64_identity(n: usize) -> *mut CscMatrix<f64> {
    CscMatrix_identity(n)
}

#[no_mangle]
pub extern "C" fn CscMatrix_f32_identity(n: usize) -> *mut CscMatrix<f32> {
    CscMatrix_identity(n)
}

/// Function for freeing the memory of a Rust CscMatrix object.
///
/// This also frees all the memory of the matrix data.
/// DO NOT use this function if the matrix data is managed by the C side.
#[allow(non_snake_case)]
unsafe fn delete_CscMatrix<T: FloatT>(matrix: *mut CscMatrix<T>) {
    if matrix.is_null() || !(*matrix).owns_matrix_data {
        return;
    }

    // Free the memory of matrix data
    let csc_matrix = crate::utils::convert_from_C_CscMatrix(matrix);
    drop(csc_matrix);

    // Free the memory of the C struct
    drop(Box::from_raw(matrix));
}

#[no_mangle]
pub unsafe extern "C" fn delete_CscMatrix_f64(matrix: *mut CscMatrix<f64>) {
    delete_CscMatrix(matrix);
}

#[no_mangle]
pub unsafe extern "C" fn delete_CscMatrix_f32(matrix: *mut CscMatrix<f32>) {
    delete_CscMatrix(matrix);
}
