#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#define SELF_NAME self_name

#define PRINT_ERROR(err, format, ...) do {                                      \
    err = errno;                                                                \
    char *err_string = strerror_or_unknown(err);                                \
    fprintf(stderr, "%s: " format ": %s\n", SELF_NAME, __VA_ARGS__, err_string); \
} while (0)

static char *strerror_or_unknown(int err) {
    char *res = strerror(err);
    return res != NULL ? res : "Unknown error code";
}

int main(int argc, char *argv[]) {
    char *SELF_NAME = argv[0];
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", SELF_NAME);
        return EINVAL;
    }
    char *infile_name = argv[1];
    char *outfile_name = argv[2];
    int err;
    // Opening infile
    if (!strcmp(infile_name, outfile_name)) {
        fprintf(stderr, "%s: \"%s\" and \"%s\" are the same\n", SELF_NAME, infile_name, outfile_name);
        return EINVAL;
    }
    FILE *infile = fopen(infile_name, "r");
    if (infile == NULL) {
        PRINT_ERROR(err, "error while opening file %s", infile_name);
        return errno;
    }

    // Reading infile
    struct stat statbuf;
    if (fstat(fileno(infile), &statbuf)) {
        PRINT_ERROR(err, "fstat() failed on file %s", infile_name);
        perror(NULL);
        fclose(infile);
        return err;
    }
    off_t filesize = statbuf.st_size;
    char *data = malloc(filesize);
    if (data == NULL) {
        PRINT_ERROR(err, "malloc() failed%s", ""); // empty __VA_ARGS__ is bad =(
        fclose(infile);
        return err;
    }
    if (fread(data, 1, filesize, infile) != filesize) {
        if (ferror(infile))
            PRINT_ERROR(err, "fread() failed on file %s", infile_name);
        else
            PRINT_ERROR(err, "fread() couldn't read the entire file %s", infile_name);
        free(data);
        fclose(infile);
        return err;
    } // if not ferror() -> EOF occured. Shouldn't ever happen but OK
    if (fclose(infile)) {
        PRINT_ERROR(err, "error while closing file %s", infile_name);
        free(data);
        return err;
    }

    // Opening outfile
    FILE *outfile = fopen(outfile_name, "w");
    if (outfile == NULL) {
        PRINT_ERROR(err, "error while opening file %s", outfile_name);
        free(data);
        return err;
    }
    if (fwrite(data, 1, filesize, outfile) != filesize) {
        if (ferror(outfile))
            PRINT_ERROR(err, "fwrite() failed on file %s", outfile_name);
        else
            PRINT_ERROR(err, "fwrite() couldn't write the entire file %s", outfile_name);
        free(data);
        // also removing outfile on fail
        remove(outfile_name);
        return err;
    }
    free(data); // finally
    
    // Closing outfile, removing infile
    if (fclose(outfile)) {
        PRINT_ERROR(err, "error while closing file %s", outfile_name);
        remove(outfile_name);
        return err;
    }
    if (remove(infile_name)) {
        PRINT_ERROR(err, "error while removing file %s", infile_name);
        return err;
    }
    
    return 0;
}