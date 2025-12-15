#include "matrixops.h"

/// \file libmatrixops.c
/// The implementation of libmatrixops.

void gemm_f64(double *C, double *A, double *B, size_t m, size_t k, size_t n) {
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            double old_value = C[i*n + j];
            for (size_t k_iter = 0; k_iter < k; k_iter++)
                old_value += A[i*k + k_iter] * B[k_iter*n + j];
            C[i*n + j] = old_value;
        }
    }
}

void gemm_f32(float *C, float *A, float *B, size_t m, size_t k, size_t n) {
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            float old_value = C[i*n + j];
            for (size_t k_iter = 0; k_iter < k; k_iter++)
                old_value += A[i*k + k_iter] * B[k_iter*n + j];
            C[i*n + j] = old_value;
        }
    }
}

void gemm_i8_i32(int32_t *C, int8_t *A, int8_t *B, size_t m, size_t k, size_t n) {
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            int32_t old_value = C[i*n + j];
            for (size_t k_iter = 0; k_iter < k; k_iter++)
                old_value += (int32_t)A[i*k + k_iter] * (int32_t)B[k_iter*n + j];
            C[i*n + j] = old_value;
        }
    }
}


void init_matrix_zero_f64(double *M, size_t size) {
    for (size_t i = 0; i < size; i++)
        M[i] = 0.0;
}

void init_matrix_zero_f32(float *M, size_t size) {
    for (size_t i = 0; i < size; i++)
        M[i] = 0.0f;
}

void init_matrix_zero_i32(int32_t *M, size_t size) {
    for (size_t i = 0; i < size; i++)
        M[i] = 0;
}

void init_matrix_zero_i8(int8_t *M, size_t size) {
    for (size_t i = 0; i < size; i++)
        M[i] = 0;
}
