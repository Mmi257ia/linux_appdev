// https://habr.com/ru/articles/320140/ -- "binary tree algorithm"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#define CELL(matrix, y, x) ((matrix)[6 * (y) + (x)])

// State of a cell: bitmask where in lower 4 bits: if the bit is set,
// then there is a wall in this direction
// 3, 2. 1, 0 bits correspond to down, left, up, right directions respectively
#define RIGHT (1 << 0)
#define UP (1 << 1)
#define LEFT (1 << 2)
#define DOWN (1 << 3)
// 4'th bit of a bitmask is used to mark the cell as visited
#define VISITED (1 << 4)

// Generate initial labyrinth
uint8_t *alloc_labyrinth() {
    uint8_t *labyrinth = malloc(6 * 6 * sizeof(*labyrinth));
    if (labyrinth == NULL)
        return NULL;
    for (int y = 0; y < 6; y++)
        for (int x = 0; x < 6; x++)
            CELL(labyrinth, y, x) = RIGHT | UP | LEFT | DOWN;
    return labyrinth;
}

// return -1 if all cells are visited
int get_non_visited_cell(uint8_t *labyrinth) {
    for (int y = 0; y < 6; y++)
        for (int x = 0; x < 6; x++)
            if (!(CELL(labyrinth, y, x) & VISITED))
                return y * 6 + x;
    return -1;
}

void gen_labyrinth(uint8_t *labyrinth) {
    int seed = 1231242;
    int total_walls = 5*6 * 2;
    int broken_walls = 0;
    do {
        srandom(seed++);
        int cur_cell = 0;
        do {
            do {
                int y = cur_cell / 6;
                int x = cur_cell % 6;
                CELL(labyrinth, y, x) |= VISITED;
                if (y == 5 && x == 5) // can't go anywhere
                    break;
                int direction; // 1 << (0, 1, 2, 3 <-> right, up, left, down)
                if (y == 5) { // can't go down
                    direction = RIGHT;
                } else if (x == 5)  { // can't go right
                    direction = DOWN;
                } else { // random direction
                    if (random() % 2 == 0)
                        direction = RIGHT;
                    else
                        direction = DOWN;
                }
                
                CELL(labyrinth, y, x) &= ~direction;
                if (direction == RIGHT) {
                    x++;
                    cur_cell++;
                    CELL(labyrinth, y, x) &= ~LEFT;
                } else { // direction == DOWN
                    y++;
                    cur_cell += 6;
                    CELL(labyrinth, y, x) &= ~UP;
                }
                broken_walls++;
                if (CELL(labyrinth, y, x) & VISITED)
                    break;
            } while (cur_cell != 35);
            cur_cell = get_non_visited_cell(labyrinth);
        } while (cur_cell != -1);
        // printf("%lf\n", (double)broken_walls / total_walls);
    } while ((double)broken_walls / total_walls > 0.85);
}

void print_labyrinth(uint8_t *labyrinth) {
    printf("#############\n");
    for (int y = 0; y < 6; y++) {
        printf("#.");
        for (int x = 1; x < 6; x++) {
            char wall = CELL(labyrinth, y, x) & LEFT ? '#' : '.';
            printf("%c.", wall);
        }
        printf("#\n");
        for (int x = 0; x < 6; x++) {
            char wall = CELL(labyrinth, y, x) & DOWN ? '#' : '.';
            printf("#%c", wall);
        }
        printf("#\n");
    }
}

int main() {
    uint8_t *labyrinth = alloc_labyrinth();
    if (labyrinth == NULL) {
        fprintf(stderr, "Failed to alloc labyrinth: %s\n", strerror(errno));
        exit(1);
    }
    gen_labyrinth(labyrinth);
    print_labyrinth(labyrinth);
    free(labyrinth);
    return 0;
}
