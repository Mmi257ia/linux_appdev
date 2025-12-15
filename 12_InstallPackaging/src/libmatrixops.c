#include "matrixops.h"

void gemm_f64(double *c, double *a, double *b, size_t m, size_t k, size_t n) {
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            double old_value = c[i*n + j];
            for (size_t k_iter = 0; k_iter < k; k_iter++)
                old_value += a[i*k + k_iter] * b[k_iter*n + j];
            c[i*n + j] = old_value;
        }
    }
}

void gemm_f32(float *c, float *a, float *b, size_t m, size_t k, size_t n) {
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            float old_value = c[i*n + j];
            for (size_t k_iter = 0; k_iter < k; k_iter++)
                old_value += a[i*k + k_iter] * b[k_iter*n + j];
            c[i*n + j] = old_value;
        }
    }
}

void gemm_i8_i32(int32_t *c, int8_t *a, int8_t *b, size_t m, size_t k, size_t n) {
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            int32_t old_value = c[i*n + j];
            for (size_t k_iter = 0; k_iter < k; k_iter++)
                old_value += (int32_t)a[i*k + k_iter] * (int32_t)b[k_iter*n + j];
            c[i*n + j] = old_value;
        }
    }
}


void init_matrix_zero_f64(double *m, size_t size) {
    for (size_t i = 0; i < size; i++)
        m[i] = 0.0;
}

void init_matrix_zero_f32(float *m, size_t size) {
    for (size_t i = 0; i < size; i++)
        m[i] = 0.0f;
}

void init_matrix_zero_i32(int32_t *m, size_t size) {
    for (size_t i = 0; i < size; i++)
        m[i] = 0;
}

void init_matrix_zero_i8(int8_t *m, size_t size) {
    for (size_t i = 0; i < size; i++)
        m[i] = 0;
}
