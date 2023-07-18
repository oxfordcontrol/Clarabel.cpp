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
