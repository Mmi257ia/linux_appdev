#include <ncurses.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

#define min(a, b) ((a) < (b) ? (a) : (b))

struct window_state {
    char *text;
    unsigned text_length;
    unsigned y_total, x_total;
    unsigned y_offset, x_offset;
};

// Write filename at the top of 'frame'
static void write_title(WINDOW *frame, char *title) {
    unsigned title_length = strlen(title);
    unsigned title_begin_x = (COLS - title_length) / 2;
    wmove(frame, 0, title_begin_x - (title_begin_x > 0 ? 1 : 0));
    wprintw(frame, " %s ", title);
}

// Get number length
static unsigned number_length(unsigned num, unsigned base) {
    unsigned res = 1;
    while (num > base) {
        num /= base;
        ++res;
    }
    return res;
}
#define dec_number_length(num) number_length((num), 10)

// Write current offset in text at the right bottom corner of 'frame'
static void write_coords(WINDOW *frame, unsigned y, unsigned x) {
    // <num>; <num>
    unsigned coords_length = dec_number_length(y) + dec_number_length(x) + 2;
    unsigned coords_begin_x = COLS - coords_length - 4;
    wmove(frame, LINES - 1, coords_begin_x);
    wprintw(frame, " %u; %u ", y, x);
}

// Redraw frame with given information
static void redraw_frame(WINDOW *frame, char *title, unsigned y, unsigned x) {
    werase(frame);
    box(frame, 0, 0);
    write_title(frame, title);
    write_coords(frame, y, x);
    wrefresh(frame);
}

// Construct frame
static WINDOW *make_frame(char *title) {
    WINDOW *res = newwin(0, 0, 0, 0);
    redraw_frame(res, title, 0, 0);
    return res;
}

// Return offset of the first symbol of next line after given offset.
// This function may return 'length' which means the end of file is reached
static unsigned next_line(char *text, unsigned length, unsigned offset) {
    char *end = text + length;
    char *p = text + offset;
    while (*p != '\n' && p < end) ++p;
    return p - text + 1;
}

// Redraw text in 'win' window given its current 'state'
static void redraw_text(WINDOW *win, struct window_state *state) {
    werase(win);
    unsigned cur_offset = 0, cur_line = 0;
    // Skip hidden lines
    while (cur_offset < state->text_length && cur_line < state->y_offset) {
        cur_offset = next_line(state->text, state->text_length, cur_offset);
        ++cur_line;
    }
    // Drawing needed lines
    unsigned cur_drawed_line = 0;
    while (cur_offset < state->text_length && cur_drawed_line < state->y_total) {
        unsigned next_line_offset = next_line(state->text, state->text_length, cur_offset);
        // write 1) to the end of the current line (excluding '\n') 2) inside the window
        unsigned limit_to_write = min(next_line_offset - 1, cur_offset + state->x_offset + state->x_total - 1);
        for (unsigned i = cur_offset + state->x_offset; i < limit_to_write; ++i)
            wprintw(win, "%c", state->text[i]);
        wprintw(win, "\n");
        cur_offset = next_line_offset;
        ++cur_drawed_line;
    }
    wrefresh(win);
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    char *filename = argv[1];
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("Error while opening file");
        return 1;
    }
    fseek(f, 0L, SEEK_END);
    long file_length = ftell(f);
    // If the file is empty, f_mapped just won't be accessed, so it can be NULL
    void *f_mapped = NULL;
    if (file_length > 0) {
        f_mapped = mmap(NULL, file_length, PROT_READ, MAP_PRIVATE, fileno(f), 0);
        if (f_mapped == MAP_FAILED) {
            perror("Error on mmap");
            fclose(f);
            return 1;
        }
    }

    // setting up windows
    WINDOW *frame, *win;
    initscr();
    noecho();
    cbreak();
    refresh();

    // making frame box
    frame = make_frame(filename);

    // main window
    win = newwin(LINES - 2, COLS - 2, 1, 1);
    keypad(win, true);
    // wrefresh(win);

    struct window_state state;
    state.text = f_mapped;
    state.text_length = file_length;
    state.x_offset = state.y_offset = 0;
    getmaxyx(win, state.y_total, state.x_total);
    redraw_text(win, &state);

    // Main key dispatch loop
    for (int c = wgetch(win); c != 27; c = wgetch(win)) {
        switch (c) {
            case ' ':
            case KEY_DOWN:
                ++state.y_offset;
                break;
            case KEY_NPAGE: // page down
                state.y_offset += state.y_total;
                break;
            case KEY_UP:
                if (state.y_offset > 0)
                    --state.y_offset;
                break;
            case KEY_PPAGE: // page up
                if (state.y_offset >= state.y_total)
                    state.y_offset -= state.y_total;
                else
                    state.y_offset = 0;
                break;
            case KEY_RIGHT:
                ++state.x_offset;
                break;
            case KEY_LEFT:
                if (state.x_offset > 0)
                    --state.x_offset;
                break;
            default: break;
        }
        redraw_frame(frame, filename, state.y_offset, state.x_offset);
        redraw_text(win, &state);
    }

    delwin(win);
    delwin(frame);
    endwin();
    if (f_mapped && munmap(f_mapped, file_length)) {
        perror("Error while unmapping file");
        return 1;
    }
    if (fclose(f)) {
        perror("Error while closing file");
        return 1;
    }
    return 0;
}