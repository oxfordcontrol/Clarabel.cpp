use crate::algebra::ClarabelCscMatrix;
use crate::solver::implementations::default::solver::*;
use crate::utils;
use core::iter::zip;
use clarabel::algebra::FloatT;
use clarabel::solver::{self as lib};
use std::{ffi::c_void, mem::forget};
use paste::paste;

#[allow(non_camel_case_types)]
enum DataUpdateTarget{
    P,
    A,
    q,
    b,
}


// Wrapper function to update solver P or A data (csc based full rewrite form)
#[allow(non_snake_case)]
unsafe fn _internal_DefaultSolver_update_csc<T: FloatT>(
    solver: *mut c_void,
    mat: *const ClarabelCscMatrix<T>, 
    method: DataUpdateTarget
) {

    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };

    // convert values to rust CSC types
    let mat = utils::convert_from_C_CscMatrix(mat);

    // Use the recovered solver object
    match method {
        DataUpdateTarget::P => solver.update_P(&mat).unwrap(),
        DataUpdateTarget::A => solver.update_A(&mat).unwrap(),
        _ => panic!("Only P and A can be updated with a CSC matrix"),
    }

    // Ensure Rust does not free the memory of arrays managed by C
    forget(mat);
}

// Wrapper function to update solver P or Adata (array based full rewrite form)
#[allow(non_snake_case)]
unsafe fn _internal_DefaultSolver_update<T: FloatT>(
    solver: *mut c_void,
    nzval: *const T,   
    nnz: usize,      
    method: DataUpdateTarget
) {

    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };

    // convert values to a vector
    let nzval = Vec::from_raw_parts(nzval as *mut T, nnz, nnz);

    // Use the recovered solver object
    match method {
        DataUpdateTarget::P => solver.update_P(&nzval).unwrap(),
        DataUpdateTarget::A => solver.update_A(&nzval).unwrap(),
        DataUpdateTarget::q => solver.update_q(&nzval).unwrap(),
        DataUpdateTarget::b => solver.update_b(&nzval).unwrap(),
    }

    // Ensure Rust does not free the memory of arrays managed by C
    forget(nzval);
}

// Wrapper function to update solver P or A data (array based partial rewrite form)
#[allow(non_snake_case)]
unsafe fn _internal_DefaultSolver_update_partial<T: FloatT>(
    solver: *mut c_void,
    index: *const usize,
    values: *const T,   
    nvals: usize,      
    method: DataUpdateTarget
) {
    // Recover the solver object from the opaque pointer
    let solver = unsafe { &mut *(solver as *mut lib::DefaultSolver<T>) };

    // convert values to a vector
    let index  = Vec::from_raw_parts(index as *mut usize, nvals, nvals);
    let values = Vec::from_raw_parts(values as *mut T, nvals, nvals);

    // Use the recovered solver object
    match method {
        DataUpdateTarget::P => solver.update_P(&zip(&index,&values)).unwrap(),
        DataUpdateTarget::A => solver.update_A(&zip(&index,&values)).unwrap(),
        DataUpdateTarget::q => solver.update_q(&zip(&index,&values)).unwrap(),
        DataUpdateTarget::b => solver.update_b(&zip(&index,&values)).unwrap(),
    }

    // Ensure Rust does not free the memory of arrays managed by C
    forget(index);
    forget(values);
}


macro_rules! _make_clarabel_DefaultSolver_update_csc {
    ($TYPE:expr,$FIELD:expr)=> {

        paste!{
            #[no_mangle]
            #[allow(non_snake_case)]
            pub unsafe extern "C" fn [<clarabel_DefaultSolver_ $TYPE _update_ $FIELD _csc>](
                solver: *mut [<ClarabelDefaultSolver _$TYPE>],
                P: *const ClarabelCscMatrix<$TYPE>,
            ) {
                _internal_DefaultSolver_update_csc::<$TYPE>(solver,P,DataUpdateTarget::$FIELD);
            }
        }
    }
}

macro_rules! _make_clarabel_DefaultSolver_update {
    ($TYPE:expr,$FIELD:expr)=> {

        paste!{
            #[no_mangle]
            #[allow(non_snake_case)]
            pub unsafe extern "C" fn [<clarabel_DefaultSolver_ $TYPE _update_ $FIELD>](
                solver: *mut [<ClarabelDefaultSolver _$TYPE>],
                nzval: *const $TYPE,
                nnz: usize,
            ) {
                _internal_DefaultSolver_update::<$TYPE>(solver,nzval,nnz,DataUpdateTarget::$FIELD);
            }


            #[no_mangle]
            #[allow(non_snake_case)]
            pub unsafe extern "C" fn [<clarabel_DefaultSolver_ $TYPE _update_ $FIELD _partial>](
                solver: *mut [<ClarabelDefaultSolver _$TYPE>],
                index: *const usize,
                values: *const $TYPE,
                nvals: usize,
            ) {
                _internal_DefaultSolver_update_partial::<$TYPE>(solver,index,values,nvals,DataUpdateTarget::$FIELD);
            }
        }
    }
}

_make_clarabel_DefaultSolver_update_csc!(f64,P);
_make_clarabel_DefaultSolver_update_csc!(f32,P);
_make_clarabel_DefaultSolver_update_csc!(f64,A);
_make_clarabel_DefaultSolver_update_csc!(f32,A);

_make_clarabel_DefaultSolver_update!(f64,P);
_make_clarabel_DefaultSolver_update!(f32,P);
_make_clarabel_DefaultSolver_update!(f64,A);
_make_clarabel_DefaultSolver_update!(f32,A);
_make_clarabel_DefaultSolver_update!(f64,q);
_make_clarabel_DefaultSolver_update!(f32,q);
_make_clarabel_DefaultSolver_update!(f64,b);
_make_clarabel_DefaultSolver_update!(f32,b);

