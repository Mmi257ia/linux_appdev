#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libintl.h>
#include <locale.h>
#include <argp.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// to silence code editor
#ifndef PACKAGE
#define PACKAGE "guesser"
#endif
#ifndef LOCALE_PATH
#define LOCALE_PATH "."
#endif

// xgettext -k_ -c src/guesser.c -o guesser.pot; -> .pot, the template
// msginit -l ru_RU.UTF-8; -> .po, the translation
// mkdir -p ru/LC_MESSAGES;
// msgfmt ru.po -o ru/LC_MESSAGES/guesser.mo; -> .mo, the binary

#define _(STR) gettext(STR)

#define A 1
#define B 100

static struct argp_option options[] = {
    {"roman", 'r', 0, 0, "Use roman numbers"},
};

struct args {
    bool roman;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct args *args = state->input;
    switch (key) {
        case 'r':
            args->roman = true;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static char doc[] = "Numbers guesser\vSupported languages: English, Russian.";

static struct argp argp = { options, parse_opt, 0, doc, 0, 0, PACKAGE };

char *decimal_to_roman(int decimal) {
    static char *roman[B - A + 1] = {
        [1-A] = "I",
        [2-A] = "II",
        [3-A] = "III",
        [4-A] = "IV",
        [5-A] = "V",
        [6-A] = "VI",
        [7-A] = "VII",
        [8-A] = "VIII",
        [9-A] = "IX",
        [10-A] = "X",
        [11-A] = "XI",
        [12-A] = "XII",
        [13-A] = "XIII",
        [14-A] = "XIV",
        [15-A] = "XV",
        [16-A] = "XVI",
        [17-A] = "XVII",
        [18-A] = "XVIII",
        [19-A] = "XIX",
        [20-A] = "XX",
        [21-A] = "XXI",
        [22-A] = "XXII",
        [23-A] = "XXIII",
        [24-A] = "XXIV",
        [25-A] = "XXV",
        [26-A] = "XXVI",
        [27-A] = "XXVII",
        [28-A] = "XXVIII",
        [29-A] = "XXIX",
        [30-A] = "XXX",
        [31-A] = "XXXI",
        [32-A] = "XXXII",
        [33-A] = "XXXIII",
        [34-A] = "XXXIV",
        [35-A] = "XXXV",
        [36-A] = "XXXVI",
        [37-A] = "XXXVII",
        [38-A] = "XXXVIII",
        [39-A] = "XXXIX",
        [40-A] = "XL",
        [41-A] = "XLI",
        [42-A] = "XLII",
        [43-A] = "XLIII",
        [44-A] = "XLIV",
        [45-A] = "XLV",
        [46-A] = "XLVI",
        [47-A] = "XLVII",
        [48-A] = "XLVIII",
        [49-A] = "XLIX",
        [50-A] = "L",
        [51-A] = "LI",
        [52-A] = "LII",
        [53-A] = "LIII",
        [54-A] = "LIV",
        [55-A] = "LV",
        [56-A] = "LVI",
        [57-A] = "LVII",
        [58-A] = "LVIII",
        [59-A] = "LIX",
        [60-A] = "LX",
        [61-A] = "LXI",
        [62-A] = "LXII",
        [63-A] = "LXIII",
        [64-A] = "LXIV",
        [65-A] = "LXV",
        [66-A] = "LXVI",
        [67-A] = "LXVII",
        [68-A] = "LXVIII",
        [69-A] = "LXIX",
        [70-A] = "LXX",
        [71-A] = "LXXI",
        [72-A] = "LXXII",
        [73-A] = "LXXIII",
        [74-A] = "LXXIV",
        [75-A] = "LXXV",
        [76-A] = "LXXVI",
        [77-A] = "LXXVII",
        [78-A] = "LXXVIII",
        [79-A] = "LXXIX",
        [80-A] = "LXXX",
        [81-A] = "LXXXI",
        [82-A] = "LXXXII",
        [83-A] = "LXXXIII",
        [84-A] = "LXXXIV",
        [85-A] = "LXXXV",
        [86-A] = "LXXXVI",
        [87-A] = "LXXXVII",
        [88-A] = "LXXXVIII",
        [89-A] = "LXXXIX",
        [90-A] = "XC",
        [91-A] = "XCI",
        [92-A] = "XCII",
        [93-A] = "XCIII",
        [94-A] = "XCIV",
        [95-A] = "XCV",
        [96-A] = "XCVI",
        [97-A] = "XCVII",
        [98-A] = "XCVIII",
        [99-A] = "XCIX",
        [100-A] = "C",
    };
    if (decimal > B || decimal < A) {
        fprintf(stderr, _("Number %d is too big to convert it to Roman\n"), decimal);
        return NULL;
    }
    return roman[decimal - A];
}

static int roman_digit_to_decimal(char roman) {
    switch (roman) {
        case 'C': return 100;
        case 'L': return 50;
        case 'X': return 10;
        case 'V': return 5;
        case 'I': return 1;
        default:
            fprintf(stderr, _("Can't convert character %c from Roman to decimal number\n"), roman);
            return -1;
    }
}

int roman_to_decimal(char *roman) {
    int res = 0;
    int i = 0;
    int len = strlen(roman);
    while (i < len) {
        int cur_digit = roman_digit_to_decimal(roman[i]);
        if (cur_digit < 0)
            return -1;
        if (i != len - 1) {
            int next_digit = roman_digit_to_decimal(roman[i+1]);
            if (cur_digit < 0)
                return -1;
            if (next_digit > cur_digit) {
                res += next_digit - cur_digit;
                i += 2;
                continue;
            }
        }
        res += cur_digit;
        i += 1;
    }
    return res;
}

// stub for xgettext to list these strings in .pot
__attribute__((unused)) static void __gettext_stub() {
    _("Usage:");
    _(" [OPTION...]");
    _("Give a short usage message");
    _("Give this help list");
    _("Use roman numbers");
    _("Numbers guesser\vSupported languages: English, Russian.");
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALE_PATH);
    textdomain(PACKAGE);

    struct args args = { false };
    argp_parse(&argp, argc, argv, 0, 0, &args);
    bool roman = args.roman;

    char * yes_str = _("Yes");
    char * no_str = _("No");
    
    int a = A, b = B; // [a; b]
    if (roman)
        printf(_("Guess a number between %s and %s.\n"), decimal_to_roman(a), decimal_to_roman(b));
    else
        printf(_("Guess a number between %d and %d.\n"), a, b);

    int cur_mid = (a + b) / 2;
    char *line = NULL;
    size_t line_size;
    while (b - a > 0) {
        if (roman)
            printf(_("Is the guessed number greater than %s? (%s/%s) "), decimal_to_roman(cur_mid), yes_str, no_str);
        else
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
    if (roman)
        printf(_("You guessed %s!\n"), decimal_to_roman(a));
    else
        printf(_("You guessed %d!\n"), a);
    return 0;
}