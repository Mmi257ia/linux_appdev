#include <check.h>
#include <stdlib.h>
#include "matrixops.h"

int8_t *i8_0, *i8_1;
int32_t *i32;
float *f32_0, *f32_1, *f32_2;
double *f64_0, *f64_1, *f64_2;

#suite libmatrixops_unittest
#tcase inits
#test init_zero_i8_test
    i8_0 = malloc(sizeof(*i8_0) * 4);
    init_matrix_zero_i8(i8_0, 4);
    ck_assert_int_eq(i8_0[0], 0);
    ck_assert_int_eq(i8_0[1], 0);
    ck_assert_int_eq(i8_0[2], 0);
    ck_assert_int_eq(i8_0[3], 0);
    free(i8_0);

#test init_zero_i32_test
    i32 = malloc(sizeof(*i32) * 4);
    init_matrix_zero_i32(i32, 4);
    ck_assert_int_eq(i32[0], 0);
    ck_assert_int_eq(i32[1], 0);
    ck_assert_int_eq(i32[2], 0);
    ck_assert_int_eq(i32[3], 0);
    free(i32);

#test init_zero_f32_test
    f32_0 = malloc(sizeof(*f32_0) * 4);
    init_matrix_zero_f32(f32_0, 4);
    ck_assert_float_eq(f32_0[0], 0.0f);
    ck_assert_float_eq(f32_0[1], 0.0f);
    ck_assert_float_eq(f32_0[2], 0.0f);
    ck_assert_float_eq(f32_0[3], 0.0f);
    free(f32_0);

#test init_zero_f64_test
    f64_0 = malloc(sizeof(*f64_0) * 4);
    init_matrix_zero_f64(f64_0, 4);
    ck_assert_float_eq(f64_0[0], 0.0);
    ck_assert_float_eq(f64_0[1], 0.0);
    ck_assert_float_eq(f64_0[2], 0.0);
    ck_assert_float_eq(f64_0[3], 0.0);
    free(f64_0);


#tcase gemms
#test gemm_i8_i32_test
    i8_0 = malloc(sizeof(*i8_0) * 2*3);
    for (int i = 0; i < 2*3; i++)
        i8_0[i] = i + 1;
    i8_1 = malloc(sizeof(*i8_1) * 3*1);
    for (int i = 0; i < 3*1; i++)
        i8_1[i] = (i + 1) * (i % 2 == 0 ? 1 : -1);
    i32 = malloc(sizeof(*i32) * 2*1);
    init_matrix_zero_i32(i32, 2);
    gemm_i8_i32(i32, i8_0, i8_1, 2, 3, 1);
    ck_assert_int_eq(i32[0], 6);
    ck_assert_int_eq(i32[1], 12);
    free(i32);
    free(i8_1);
    free(i8_0);

#test gemm_f32_test
    f32_0 = malloc(sizeof(*f32_0) * 2*3);
    for (int i = 0; i < 2*3; i++)
        f32_0[i] = 0.1f * (float)(i + 1);
    f32_1 = malloc(sizeof(*f32_1) * 3*1);
    for (int i = 0; i < 3*1; i++)
        f32_1[i] = (float)(i + 1) * (float)(i % 2 == 0 ? 1 : -1);
    f32_2 = malloc(sizeof(*f32_2) * 2*1);
    init_matrix_zero_f32(f32_2, 2);
    gemm_f32(f32_2, f32_0, f32_1, 2, 3, 1);
    ck_assert_float_eq(f32_2[0], 0.6f);
    ck_assert_float_eq(f32_2[1], 1.2f);
    free(f32_2);
    free(f32_1);
    free(f32_0);

#test gemm_f64_test
    f64_0 = malloc(sizeof(*f64_0) * 2*3);
    for (int i = 0; i < 2*3; i++)
        f64_0[i] = 0.1 * (double)(i + 1);
    f64_1 = malloc(sizeof(*f64_1) * 3*1);
    for (int i = 0; i < 3*1; i++)
        f64_1[i] = (double)(i + 1) * (double)(i % 2 == 0 ? 1 : -1);
    f64_2 = malloc(sizeof(*f64_2) * 2*1);
    init_matrix_zero_f64(f64_2, 2);
    gemm_f64(f64_2, f64_0, f64_1, 2, 3, 1);
    ck_assert_float_eq(f64_2[0], 0.6);
    ck_assert_float_eq(f64_2[1], 1.2);
    free(f64_2);
    free(f64_1);
    free(f64_0);
    