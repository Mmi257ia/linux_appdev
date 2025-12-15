// #include <matrixops.h>
#include "matrixops.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <libintl.h>
#include <locale.h>
#include <argp.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// to silence code editor
#ifndef PACKAGE
#define PACKAGE "matrix_ops_runner"
#endif
#ifndef LOCALE_PATH
#define LOCALE_PATH "."
#endif
#ifndef VERSION
#define VERSION "1.0.0"
#endif

#define _(STR) gettext(STR)

const char *argp_program_version = VERSION;

static struct argp_option options[] = {
    {"type", 't', "TYPE", 0, "Specify input type. Currently supported: `i8`, `f32` and `f64`. [REQUIRED]"},
    {"files", 'f', "BINARY_FILES_NAMES", 0, "Binary files with inputs and outputs (comma-separated) [REQUIRED]"},
    { 0 },
};

struct args {
    enum input_types_t { i8, i32, f32, f64, INVALID_TYPE } type;
    char *input_0, *input_1, *output;
};

static void split_binary_files(char *src, char **input_0, char **input_1, char **output, struct argp_state *state) {
    *input_0 = strtok(src, ",");
    if (*input_0 == NULL)
        argp_failure(state, argp_err_exit_status, 0, "no files were specified");
    *input_1 = strtok(NULL, ",");
    if (*input_1 == NULL)
        argp_failure(state, argp_err_exit_status, 0, "just 1 of 3 files was specified");
    *output = strtok(NULL, ",");
    if (*output == NULL)
        argp_failure(state, argp_err_exit_status, 0, "just 2 of 3 files was specified");
    char *tail = strtok(NULL, ",");
    if (tail != NULL)
        argp_failure(state, argp_err_exit_status, 0, "more that 3 files were specified, tail is: \"%s\"", tail);
}

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct args *args = state->input;
    switch (key) {
        case 't':
            if (!strcasecmp(arg, "i8"))
                args->type = i8;
            else if (!strcasecmp(arg, "f32"))
                args->type = f32;
            else if (!strcasecmp(arg, "f64"))
                args->type = f64;
            else
                argp_failure(state, argp_err_exit_status, 0, "unsupported input type \"%s\"", arg);
            break;
        case 'f':
            split_binary_files(arg, &args->input_0, &args->input_1, &args->output, state);
            break;
        case ARGP_KEY_END:
            if (args->type == INVALID_TYPE)
                argp_failure(state, argp_err_exit_status, 0, "input type is not specified");
            if (args->input_0 == NULL || args->input_1 == NULL || args->output == NULL)
                argp_failure(state, argp_err_exit_status, 0, "input/output files are not specified");
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static char doc[] = "Matrix operations runner.\vThe only supported operation is "
                    "matrix multiplication. Supported types: i8 x i8 -> i32, "
                    "f32 x f32 -> f32, f64 x f64 -> f64.";

static struct argp argp = { options, parse_opt, 0, doc, 0, 0, PACKAGE };

#define SELF_NAME self_name

#define PRINT_ERROR(err, format, ...) do {                                      \
    err = errno;                                                                \
    char *err_string = strerror_or_unknown(err);                                \
    fprintf(stderr, "%s: " format ": %s\n", SELF_NAME, __VA_ARGS__, err_string);\
} while (0)

static char *strerror_or_unknown(int err) {
    char *res = strerror(err);
    return res != NULL ? res : "Unknown error code";
}

// return NULL on error
static void *read_matrix_from_bin(size_t *h, size_t *w, char *file_name, int type, char *SELF_NAME) {
    int err;
    size_t elt_size = type == i8 ? 1 :
                     (type == i32 ? 4 :
                     (type == f32 ? 4 :
                     (type == f64 ? 8 :
                      0))); // unreachable

    // Open file
    FILE *f = fopen(file_name, "rb");
    if (f == NULL) {
        PRINT_ERROR(err, "error while opening file %s", file_name);
        return NULL;
    }

    // Read shape
    if (fread(h, sizeof(size_t), 1, f) != 1) {
        fprintf(stderr, "%s: error while reading matrix height from file %s\n", SELF_NAME, file_name);
        fclose(f);
        return NULL;
    }
    if (fread(w, sizeof(size_t), 1, f) != 1) {
        fprintf(stderr, "%s: error while reading matrix width from file %s\n", SELF_NAME, file_name);
        fclose(f);
        return NULL;
    }

    // Allocate space for matrix
    void *matrix = malloc(elt_size * (*h)*(*w));
    if (matrix == NULL) {
        PRINT_ERROR(err, "error while allocating %lu bytes for matrix from file %s", elt_size * (*h)*(*w), file_name);
        fclose(f);
        return NULL;
    }
    switch (type) {
        case i8: init_matrix_zero_i8(matrix, (*h)*(*w)); break;
        case i32: init_matrix_zero_i32(matrix, (*h)*(*w)); break;
        case f32: init_matrix_zero_f32(matrix, (*h)*(*w)); break;
        case f64: init_matrix_zero_f64(matrix, (*h)*(*w)); break;
        default: break; // unreachable
    }

    // Read everything that we can, absent elements are
    // already initialized so don't check how much we read,
    // ignore excessive elements.
    fread(matrix, elt_size, (*h)*(*w), f);

    fclose(f);
    return matrix;
}

static int write_matrix_bin(size_t h, size_t w, void *matrix, char *file_name, int type, char *SELF_NAME) {
    int err;
    size_t elt_size = type == i8 ? 1 :
                     (type == i32 ? 4 :
                     (type == f32 ? 4 :
                     (type == f64 ? 8 :
                      0))); // unreachable

    // Open file
    FILE *f = fopen(file_name, "wb");
    if (f == NULL) {
        PRINT_ERROR(err, "error while opening file %s", file_name);
        return 1;
    }

    // Write shape
    if (fwrite(&h, sizeof(size_t), 1, f) != 1) {
        fprintf(stderr, "%s: error while writing matrix height to file %s\n", SELF_NAME, file_name);
        fclose(f);
        return 1;
    }
    if (fwrite(&w, sizeof(size_t), 1, f) != 1) {
        fprintf(stderr, "%s: error while writing matrix height to file %s\n", SELF_NAME, file_name);
        fclose(f);
        return 1;
    }

    // Write
    if (fwrite(matrix, elt_size, h*w, f) != h*w) {
        fprintf(stderr, "%s: could not write all %lu bytes to file %s\n", SELF_NAME, elt_size * h*w, file_name);
        fclose(f);
        return 1;
    }

    fclose(f);
    return 0;
}

int main(int argc, char *argv[]) {
    char *SELF_NAME = argv[0];
    int err;
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALE_PATH);
    textdomain(PACKAGE);

    struct args args = { INVALID_TYPE, NULL, NULL, NULL };
    argp_parse(&argp, argc, argv, 0, 0, &args);

    size_t m_0, m_2, k_0, k_1, n_1, n_2;
    void *input_0, *input_1, *output;

    input_0 = read_matrix_from_bin(&m_0, &k_0, args.input_0, args.type, SELF_NAME);
    if (input_0 == NULL)
        return 1;
    input_1 = read_matrix_from_bin(&k_1, &n_1, args.input_1, args.type, SELF_NAME);
    if (input_1 == NULL) {
        free(input_0);
        return 1;
    }
    
    if (k_0 != k_1) {
        fprintf(stderr, "%s: width of the left multiplier is %lu, "
                        "but the height of the right one is %lu!\n", SELF_NAME, k_0, k_1);
        free(input_1);
        free(input_0);
        return 1;
    }

    if (!access(args.output, F_OK)) {
        output = read_matrix_from_bin(&m_2, &n_2, args.output, args.type == i8 ? i32 : args.type, SELF_NAME);
        if (output == NULL) {
            free(input_1);
            free(input_0);
            return 1;
        }
        if (m_0 != m_2) {
            fprintf(stderr, "%s: height of the left multiplier is %lu, "
                            "but the height of the output is %lu!\n", SELF_NAME, m_0, m_2);
            free(output);
            free(input_1);
            free(input_0);
            return 1;
        }
        if (n_1 != n_2) {
            fprintf(stderr, "%s: width of the right multiplier is %lu, "
                            "but the width of the output is %lu!\n", SELF_NAME, n_1, n_2);
            free(output);
            free(input_1);
            free(input_0);
            return 1;
        }
    } else { // Creating a new file
        size_t elt_size = args.type == i8 ? 4 :
                         (args.type == f32 ? 4 :
                         (args.type == f64 ? 8 :
                          0)); // unreachable
        // Allocate space for matrix
        output = malloc(elt_size * m_0*n_1);
        if (output == NULL) {
            PRINT_ERROR(err, "error while allocating %lu bytes for output matrix", elt_size * m_0*n_1);
            free(input_1);
            free(input_0);
            return 1;
        }
        switch (args.type) {
            case i8: init_matrix_zero_i32(output, m_0*n_1); break;
            case f32: init_matrix_zero_f32(output, m_0*n_1); break;
            case f64: init_matrix_zero_f64(output, m_0*n_1); break;
            default: break; // unreachable
        }
    }

    switch (args.type) {
        case i8:  gemm_i8_i32(output, input_0, input_1, m_0, k_0, n_1); break;
        case f32: gemm_f32(output, input_0, input_1, m_0, k_0, n_1); break;
        case f64: gemm_f64(output, input_0, input_1, m_0, k_0, n_1); break;
        default: break; // unreachable
    }

    if (write_matrix_bin(m_0, n_1, output, args.output, args.type == i8 ? i32 : args.type, SELF_NAME)) {
        free(output);
        free(input_1);
        free(input_0);
        return 1;
    }

    free(output);
    free(input_1);
    free(input_0);
    return 0;
}
