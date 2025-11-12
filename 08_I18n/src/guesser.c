#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A 1
#define B 100

static char * yes_str = "Yes";
static char * no_str = "No";

int main() {
    int a = A, b = B; // [a; b]
    printf("Guess a number between %d and %d.\n", a, b);

    int cur_mid = (a + b) / 2;
    char *line = NULL;
    size_t line_size;
    while (b - a > 0) {
        printf("Is the guessed number greater than %d? (%s/%s) ", cur_mid, yes_str, no_str);
        ssize_t read_syms = getline(&line, &line_size, stdin);
        if (read_syms == -1) {
            fprintf(stderr, "Got the end of input or an error, aborting\n");
            free(line);
            return 1;
        }
        // remove trailing \n
        line[read_syms - 1] = '\0';
        if (!strcmp(yes_str, line)) { // greater
            a = cur_mid + 1;
            cur_mid = (a + b) / 2;
            continue;
        } else if (!strcmp(no_str, line)) { // less
            b = cur_mid;
            cur_mid = (a + b) / 2;
            continue;
        } else {
            fprintf(stderr, "Please enter just \"%s\" or just \"%s\"\n", yes_str, no_str);
            continue;
        }
    }
    free(line);
    printf("You guessed %d!\n", a);
    return 0;
}