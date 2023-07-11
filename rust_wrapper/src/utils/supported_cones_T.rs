use crate::core::cones::SupportedConeT;
use clarabel::algebra::FloatT;
use clarabel::solver as lib;

/// Convert a slice of C SupportedConeT structs to a Vec of Rust SupportedCone<T> struct
#[allow(non_snake_case)]
pub fn convert_from_C_cones<T>(c_cones: &[SupportedConeT<T>]) -> Vec<lib::SupportedConeT<T>>
where
    T: FloatT,
{
    // Initialize the vector with the correct capacity
    let mut cones: Vec<lib::SupportedConeT<T>> = Vec::with_capacity(c_cones.len());

    // Convert each cone
    for cone in c_cones {
        cones.push(convert_from_C_cone(cone));
    }
    cones
}

/// Convert a single C SupportedConeT<T> struct to a Rust SupportedCone<T> struct
#[allow(non_snake_case)]
pub fn convert_from_C_cone<T>(cone: &SupportedConeT<T>) -> lib::SupportedConeT<T>
where
    T: FloatT,
{
    match cone {
        SupportedConeT::ZeroConeT(payload) => lib::SupportedConeT::ZeroConeT(*payload),
        SupportedConeT::NonnegativeConeT(payload) => {
            lib::SupportedConeT::NonnegativeConeT(*payload)
        }
        SupportedConeT::SecondOrderConeT(payload) => {
            lib::SupportedConeT::SecondOrderConeT(*payload)
        }
        SupportedConeT::ExponentialConeT() => lib::SupportedConeT::ExponentialConeT(),
        SupportedConeT::PowerConeT(payload) => lib::SupportedConeT::PowerConeT(*payload),
        //SupportedConeT::PSDTriangleConeT(payload) => lib::SupportedConeT::PSDTriangleConeT(payload),
    }
}
