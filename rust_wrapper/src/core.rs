pub mod cones {

    #[allow(dead_code)] // Suppress warnings for unused code
    #[derive(Debug)]
    #[repr(C)]
    pub enum ClarabelSupportedConeT<T> {
        /// The zero cone (used for equality constraints).
        ///
        /// The parameter indicates the cones dimension.
        ZeroConeT(usize),
        /// The nonnegative orthant.  
        ///
        /// The parameter indicates the cones dimension.
        NonnegativeConeT(usize),
        /// The second order cone / Lorenz cone / ice-cream cone.
        ///  
        /// The parameter indicates the cones dimension.
        SecondOrderConeT(usize),
        /// The exponential cone in R^3.
        ///
        /// This cone takes no parameters
        ExponentialConeT(),
        /// The power cone in R^3.
        ///
        /// The parameter indicates the power.
        PowerConeT(T),
        /// The generalized power cone
        ///
        /// First parameter is alpha, second is dimension of RHS
        GenPowerConeT(*const T, usize, usize),
        /// The positive semidefinite cone in triangular form.
        ///
        /// The parameter indicates the matrix dimension, i.e. size = n
        /// means that the variable is the upper triangle of an nxn matrix.
        #[cfg(feature = "sdp")]
        PSDTriangleConeT(usize),
    }
}
