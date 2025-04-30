use clarabel::algebra::FloatT;

pub type ClarabelDirectSolveMethods = clarabel::solver::ffi::DirectSolveMethodsFFI;

#[cfg(feature = "sdp")]
pub type ClarabelCliqueMergeMethods = clarabel::solver::ffi::CliqueMergeMethodsFFI;

pub type ClarabelDefaultSettings<T> =
    clarabel::solver::implementations::default::ffi::DefaultSettingsFFI<T>;

/// Wrapper function for DefaultSettings::default()
///
/// Get the default settings for the solver
#[allow(non_snake_case)]
fn _internal_DefaultSettings_default<T: FloatT>() -> ClarabelDefaultSettings<T> {
    let default = clarabel::solver::DefaultSettings::<T>::default();
    default.into()
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSettings_f64_default() -> ClarabelDefaultSettings<f64> {
    _internal_DefaultSettings_default::<f64>()
}

#[no_mangle]
pub extern "C" fn clarabel_DefaultSettings_f32_default() -> ClarabelDefaultSettings<f32> {
    _internal_DefaultSettings_default::<f32>()
}
