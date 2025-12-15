#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include <stddef.h>
#include <stdint.h>

void gemm_f64(double *c, double *a, double *b, size_t m, size_t k, size_t n);

void gemm_f32(float *c, float *a, float *b, size_t m, size_t k, size_t n);

void gemm_i8_i32(int32_t *c, int8_t *a, int8_t *b, size_t m, size_t k, size_t n);


void init_matrix_zero_f64(double *m, size_t size);

void init_matrix_zero_f32(float *m, size_t size);

void init_matrix_zero_i32(int32_t *m, size_t size);

void init_matrix_zero_i8(int8_t *m, size_t size);

#endif // MATRIX_OPS_H
