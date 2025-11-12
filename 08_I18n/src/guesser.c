#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libintl.h>
#include <locale.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// to silence code editor
#ifndef PACKAGE
#define PACKAGE "guesser"
#endif

// xgettext -k_ -c src/guesser.c -o guesser.pot; -> .pot, the template
// msginit -l ru_RU.UTF-8; -> .po, the translation
// mkdir -p ru/LC_MESSAGES;
// msgfmt ru.po -o ru/LC_MESSAGES/guesser.mo; -> .mo, the binary

#define _(STR) gettext(STR)
#define LOCALE_PATH "."

#define A 1
#define B 100

int main() {
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALE_PATH);
    textdomain(PACKAGE);

    char * yes_str = _("Yes");
    char * no_str = _("No");
    
    int a = A, b = B; // [a; b]
    printf(_("Guess a number between %d and %d.\n"), a, b);

    int cur_mid = (a + b) / 2;
    char *line = NULL;
    size_t line_size;
    while (b - a > 0) {
        printf(_("Is the guessed number greater than %d? (%s/%s) "), cur_mid, yes_str, no_str);
        ssize_t read_syms = getline(&line, &line_size, stdin);
        if (read_syms == -1) {
            fprintf(stderr, _("Got the end of input or an error, aborting\n"));
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
            fprintf(stderr, _("Please enter just \"%s\" or just \"%s\"\n"), yes_str, no_str);
            continue;
        }
    }
    free(line);
    printf(_("You guessed %d!\n"), a);
    return 0;
}