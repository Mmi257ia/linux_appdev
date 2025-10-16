#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <regexp> <substitution> <string>\n", argv[0]);
        exit(1);
    }
    char *regex_str = argv[1];
    char *substitution = argv[2];
    char *str = argv[3];
    regex_t regex;
    regmatch_t *matches;
    int err;
    
    // Compile the regex
    if ((err = regcomp(&regex, regex_str, REG_EXTENDED))) {
        size_t regexr_err_length = regerror(err, &regex, NULL, 0);
        char *regex_err_str = malloc(regexr_err_length * sizeof(char));
        if (regex_err_str == NULL) {
            perror("malloc() for regex error string failed: ");
            exit(1);
        }
        regerror(err, &regex, regex_err_str, regexr_err_length);
        fprintf(stderr, "Error on regcomp(): %s\n", regex_err_str);
        free(regex_err_str);
        exit(1);
    }

    // Allocate memory for matches
    matches = calloc(regex.re_nsub + 1, sizeof(regmatch_t));
    if (matches == NULL) {
        perror("calloc() for regex matches failed: ");
        regfree(&regex);
        exit(1);
    }

    // Execute regex
    if ((err = regexec(&regex, str, regex.re_nsub + 1, matches, 0))) {
        // err can be only REG_NOMATCH, according to man
        // so returning just the input string and exiting normally
        printf("%s\n", str);
        free(matches);
        regfree(&regex);
        exit(0);
    }
    
    // A match was found, need to substitute
    // First pass: determine required space
    size_t substitution_len = strlen(substitution); // the length of the substitution template
    size_t substring_len = substitution_len; // the length of the real substring that will be inserted
    size_t i = 0;
    while (i < substitution_len) {
        char c = substitution[i];
        if (c == '\\') {
            i++; // skipping backslash
            substring_len--;
            char after_backslash = substitution[i];
            if (isdigit(after_backslash)) {
                int group_num = after_backslash - '0';
                if (group_num > regex.re_nsub) {
                    fprintf(stderr, "Error: in substring:%lu the %d'th subgroup asked, but there are %lu of them!\n",
                        i-1, group_num, regex.re_nsub);
                    free(matches);
                    regfree(&regex);
                    exit(1);
                } else
                    substring_len += matches[group_num].rm_eo - matches[group_num].rm_so - 1;
            }
        } else if (c == '&') // the entire match
            substring_len += matches[0].rm_eo - matches[0].rm_so - 1;
        i++;
    }
    size_t str_len = strlen(str);
    size_t match_len = matches[0].rm_eo - matches[0].rm_so;
    char *result = malloc(str_len - match_len + substring_len + 1);
    if (result == NULL) {
        perror("malloc() for result string failed:");
        free(matches);
        regfree(&regex);
        exit(1);
    }

    // Second pass: substitution!
    strncpy(result, str, matches[0].rm_so); // before match
    size_t result_i = matches[0].rm_so, substitution_i = 0;
    // This code is awful, I'm sorry
    while (substitution_i < substitution_len) {
        char c = substitution[substitution_i];
        if (c == '\\') {
            char after_backslash = substitution[substitution_i+1];
            if (isdigit(after_backslash)) {
                int match_ind = after_backslash - '0';
                size_t substring_len = matches[match_ind].rm_eo - matches[match_ind].rm_so;
                strncpy(result + result_i, str + matches[match_ind].rm_so, substring_len);
                result_i += substring_len;
                substitution_i += 2;
            } else if (after_backslash == '\\' || after_backslash == '&') {
                result[result_i] = after_backslash;
                result_i++;
                substitution_i += 2;
            } else // else: just skip the backslash
                substitution_i++;
        } else if (c == '&') {
            size_t substring_len = matches[0].rm_eo - matches[0].rm_so;
            strncpy(result + result_i, str + matches[0].rm_so, substring_len);
            result_i += substring_len;
            substitution_i++;
        } else {
            result[result_i] = c;
            result_i++;
            substitution_i++;
        }
    }
    strcpy(result + result_i, str + matches[0].rm_eo); // after match
    
    printf("%s\n", result);
    
    free(result);
    free(matches);
    regfree(&regex);
    return 0;
}