#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int start, stop, step, cur;
} range;

void argparse(int argc, char *argv[], int *start, int *stop, int *step) {
    if (argc == 1) {
        fprintf(stderr, "Usage: %s (<stop>|<start> <stop> [<step>])\n", argv[0]);
        exit(1);
    } else if (argc == 2) {
        *start = 0;
        *step = 1;
        sscanf(argv[1], "%d", stop);
    } else if (argc >= 3) {
        sscanf(argv[1], "%d", start);
        sscanf(argv[2], "%d", stop);
        if (argc >= 4)
            sscanf(argv[3], "%d", step);
        else
            *step = 1;
    }
}

void range_init(range *r) {
    r->cur = r->start;
}

bool range_run(range *r) {
    if (r->step > 0)
        return r->cur < r->stop;
    else
        return r->cur > r->stop;
    
}

void range_next(range *r) {
    r->cur += r->step;
}

int range_get(range *r) {
    return r->cur;  
}

int main(int argc, char *argv[]) {
        range I;
        argparse(argc, argv, &I.start, &I.stop, &I.step);
        for(range_init(&I); range_run(&I); range_next(&I))
                printf("%d\n", range_get(&I));
        return 0;
}
