pub mod cones {

    #[allow(dead_code)] // Suppress warnings for unused code
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
        /*
        /// The positive semidefinite cone in triangular form.
        ///
        /// The parameter indicates the matrix dimension, i.e. size = n
        /// means that the variable is the upper triangle of an nxn matrix.
        // TODO: Add config for #[cfg(feature = "sdp")]
        // PSDTriangleConeT(usize),
         */
    }
}
