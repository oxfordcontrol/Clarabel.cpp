use clarabel::algebra::FloatT;

#[repr(C)]
// This struct is for interfacing with C only and should be synched with clarabel::algebra::CscMatrix
pub struct ClarabelCscMatrix<T = f64> {
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

#[allow(non_snake_case)]
unsafe fn _internal_Cscmatrix_init<T: FloatT>(
    ptr: *mut ClarabelCscMatrix<T>,
    m: usize,
    n: usize,
    colptr: *const usize,
    rowval: *const usize,
    nzval: *const T,
) {
    *ptr = ClarabelCscMatrix::<T> {
        m,
        n,
        colptr,
        rowval,
        nzval,
    };
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_CscMatrix_f32_init(
    ptr: *mut ClarabelCscMatrix<f32>,
    m: usize,
    n: usize,
    colptr: *const usize,
    rowval: *const usize,
    nzval: *const f32,
) {
    _internal_Cscmatrix_init::<f32>(ptr, m, n, colptr, rowval, nzval);
}

#[no_mangle]
pub unsafe extern "C" fn clarabel_CscMatrix_f64_init(
    ptr: *mut ClarabelCscMatrix<f64>,
    m: usize,
    n: usize,
    colptr: *const usize,
    rowval: *const usize,
    nzval: *const f64,
) {
    _internal_Cscmatrix_init::<f64>(ptr, m, n, colptr, rowval, nzval);
}
