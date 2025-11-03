#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>

#include <rhash.h>

#ifdef HAVE_READLINE
#include <readline/readline.h>
#endif

#define SELF_NAME self_name

#define PRINT_ERROR(format, ...) do {                                           \
    int err = errno;                                                            \
    char *err_string = strerror_or_unknown(err);                                \
    fprintf(stderr, format ": %s\n", __VA_ARGS__, err_string);\
} while (0)

static char *strerror_or_unknown(int err) {
    char *res = strerror(err);
    return res != NULL ? res : "Unknown error code";
}

#define MAX_DIGEST_LEN 24 // 16 for MD5, 20 for SHA1, 24 for TTH
#define MAX_DIGEST_STRING_LEN 48 // 32 for MD5, 40 for SHA1, 48 for TTH

// Converts a string to hash type. Returns 0 on error.
// Also converts the given string to upper case!
static enum rhash_ids get_hash_type(char *str) {
    size_t str_len = strlen(str);
        for (size_t i = 0; i < str_len; i++)
            str[i] = toupper((unsigned char)str[i]);
    if (!strcmp(str, "MD5"))
        return RHASH_MD5;
    else if (!strcmp(str, "SHA1"))
        return RHASH_SHA1;
    else if (!strcmp(str, "TTH"))
        return RHASH_TTH;
    else
        return 0;
}

// Get the string representing the hash of a string or a file. rhash library must be initialized.
// Returns 0 on success or anything else on error
static int hash(char *out, char *in, bool str, enum rhash_ids hash_type, bool print_hex) {
    unsigned char digest[MAX_DIGEST_LEN];
    int err;
    if (str)
        err = rhash_msg(hash_type, in, strlen(in), digest);
    else
        err = rhash_file(hash_type, in, digest);
    if (err)
        return err;

    int print_flags = 0;
    if (print_hex)
        print_flags |= RHPR_HEX;
    else
        print_flags |= RHPR_BASE64;
    rhash_print_bytes(out, digest, rhash_get_digest_size(hash_type), print_flags);
    return 0;
}

#define TOKEN_DELIMS " \n"

int main(int argc, char *argv[]) {
    char *SELF_NAME = argv[0];

    rhash_library_init();
    char digest_string[MAX_DIGEST_STRING_LEN];

    char *line = NULL;
    do {
#ifdef HAVE_READLINE
        line = readline(">>> ");
        if (line == NULL)
            return 0;
#else
        printf(">>> ");
        size_t line_buf_size;
        ssize_t read_syms = getline(&line, &line_buf_size, stdin);
        if (read_syms == -1) {
            if (feof(stdin)) {
                printf("\n");
                free(line);
                return 0;
            } else {
                PRINT_ERROR("getline() failed%s", ""); // empty __VA_ARGS__ is bad =(
                free(line);
                return 1;
            }
        }
#endif

        // Fetching tokens
        char *hash_type_str = strtok(line, TOKEN_DELIMS);
        if (hash_type_str == NULL) {
            fprintf(stderr, "Usage: <hash_type> <input>\n");
            free(line);
            continue;
        }
        char *input = strtok(NULL, TOKEN_DELIMS);
        if (input == NULL) {
            fprintf(stderr, "Usage: <hash_type> <input>\n");
            free(line);
            continue;
        }
        
        // Parsing
        bool print_hex = isupper((unsigned char)hash_type_str[0]);
        enum rhash_ids hash_type = get_hash_type(hash_type_str);
        if (hash_type == 0) {
            fprintf(stderr, "Invalid hash type: %s\n", hash_type_str);
            free(line);
            continue;
        }
        bool is_str = input[0] == '"';
        if (hash(digest_string, input + is_str, is_str, hash_type, print_hex)) {
            PRINT_ERROR("%s-hashing of string %s failed", hash_type_str, input);
            free(line);
            continue;
        }
        printf("%s\n", digest_string);
        free(line);
    } while (true);

    return 0;
}