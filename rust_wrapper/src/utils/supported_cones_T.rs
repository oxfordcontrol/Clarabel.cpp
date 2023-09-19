use crate::core::cones::ClarabelSupportedConeT;
use clarabel::algebra::FloatT;
use clarabel::solver as lib;

/// Convert a slice of C SupportedConeT structs to a Vec of Rust SupportedCone<T> struct
#[allow(non_snake_case)]
pub fn convert_from_C_cones<T: FloatT>(
    c_cones: &[ClarabelSupportedConeT<T>],
) -> Vec<lib::SupportedConeT<T>> {
    // Initialize the vector with the correct capacity
    let mut cones: Vec<lib::SupportedConeT<T>> = Vec::with_capacity(c_cones.len());

    // Convert each cone
    for cone in c_cones.iter() {
        cones.push(convert_from_C_cone(cone));
    }
    cones
}

/// Convert a single C SupportedConeT<T> struct to a Rust SupportedCone<T> struct
#[allow(non_snake_case)]
pub fn convert_from_C_cone<T: FloatT>(cone: &ClarabelSupportedConeT<T>) -> lib::SupportedConeT<T> {
    match cone {
        ClarabelSupportedConeT::ZeroConeT(payload) => lib::SupportedConeT::ZeroConeT(*payload),
        ClarabelSupportedConeT::NonnegativeConeT(payload) => {
            lib::SupportedConeT::NonnegativeConeT(*payload)
        }
        ClarabelSupportedConeT::SecondOrderConeT(payload) => {
            lib::SupportedConeT::SecondOrderConeT(*payload)
        }
        ClarabelSupportedConeT::ExponentialConeT() => lib::SupportedConeT::ExponentialConeT(),
        ClarabelSupportedConeT::PowerConeT(payload) => lib::SupportedConeT::PowerConeT(*payload),
        ClarabelSupportedConeT::GenPowerConeT(ptr_alpha,dim1,dim2) => {
            let alpha = unsafe { std::slice::from_raw_parts(*ptr_alpha, *dim1) };
            lib::SupportedConeT::GenPowerConeT(alpha.to_vec(),*dim2)
        }
        #[cfg(feature = "sdp")]
        ClarabelSupportedConeT::PSDTriangleConeT(payload) => {
            lib::SupportedConeT::PSDTriangleConeT(*payload)
        }
    }
}
