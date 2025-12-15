#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

/// \file matrixops.h
/// Header with the interface of libmatrixops.

#include <stddef.h>
#include <stdint.h>

/// \brief General matrix multiplication for `f64` matrices.
///
/// Computes \p C += \p A * \p B where \p C shape is \p m x \p n, \p A shape is
/// \p m x \p k and \p B shape is \p k x \p n. All matrices are represented by a
/// pointer to a row-major storage of their elements. All matrices' elements are
/// of type `f64`.
///
/// \param C Pointer to the resulting `f64` matrix.
/// \param A Pointer to the `f64` left multiplier.
/// \param B Pointer to the `f64` right multiplier.
/// \param m Height of \p C and \p A matrices.
/// \param k Width of \p A and height of \p B matrices.
/// \param n Width of \p C and \p B matrices.
void gemm_f64(double *C, double *A, double *B, size_t m, size_t k, size_t n);

/// \brief General matrix multiplication for `f32` matrices.
///
/// Computes \p C += \p A * \p B where \p C shape is \p m x \p n, \p A shape is
/// \p m x \p k and \p B shape is \p k x \p n. All matrices are represented by a
/// pointer to a row-major storage of their elements. All matrices' elements are
/// of type `f32`.
///
/// \param C Pointer to the resulting `f32` matrix.
/// \param A Pointer to the `f32` left multiplier.
/// \param B Pointer to the `f32` right multiplier.
/// \param m Height of \p C and \p A matrices.
/// \param k Width of \p A and height of \p B matrices.
/// \param n Width of \p C and \p B matrices.
void gemm_f32(float *C, float *A, float *B, size_t m, size_t k, size_t n);

/// \brief General matrix multiplication for `i8` inputs and `i32` output.
///
/// Computes \p C += \p A * \p B where \p C shape is \p m x \p n, \p A shape is
/// \p m x \p k and \p B shape is \p k x \p n. All matrices are represented by a
/// pointer to a row-major storage of their elements. \p A and \p B elements are of
/// type `i8`, \p C elements are of type `i32`.
///
/// \param C Pointer to the resulting `i32` matrix.
/// \param A Pointer to the `i8` left multiplier.
/// \param B Pointer to the `i8` right multiplier.
/// \param m Height of \p C and \p A matrices.
/// \param k Width of \p A and height of \p B matrices.
/// \param n Width of \p C and \p B matrices.
void gemm_i8_i32(int32_t *C, int8_t *A, int8_t *B, size_t m, size_t k, size_t n);


/// \brief Initialize all elements of a `f64` matrix to zeros.
///
/// \param M Pointer to the `f64` matrix.
/// \param size Number of elements of \p M.
void init_matrix_zero_f64(double *M, size_t size);

/// \brief Initialize all elements of a `f32` matrix to zeros.
///
/// \param M Pointer to the `f32` matrix.
/// \param size Number of elements of \p M.
void init_matrix_zero_f32(float *M, size_t size);

/// \brief Initialize all elements of a `i32` matrix to zeros.
///
/// \param M Pointer to the `i32` matrix.
/// \param size Number of elements of \p M.
void init_matrix_zero_i32(int32_t *M, size_t size);

/// \brief Initialize all elements of a `i8` matrix to zeros.
///
/// \param M Pointer to the `i8` matrix.
/// \param size Number of elements of \p M.
void init_matrix_zero_i8(int8_t *M, size_t size);

#endif // MATRIX_OPS_H
