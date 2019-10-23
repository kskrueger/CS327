//
// Created by Karter Krueger on 9/10/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Part1.c"

void read_args(int argc, char* argv[]);
void move_piece(int start_c, int start_r, int end_c, int end_r);

int n = -1;
char *filename = NULL;

int main(int argc, char* argv[]) {
    read_args(argc, argv);
    scan_input();

    print_board(ROWS, COLS, board);

    move current = moves;
    int m, p;
    for (m = 0; current->next->next != NULL; m++) {
        point currPoint = current->point;
        for (p = 0; currPoint->next->next != NULL; p++) {
            if (!multiple_jumps_on && p > 1) fprintf(stderr, "ERROR: Multiple jumps attempted while off\n");
            int r = currPoint->r;
            int c = currPoint->c;
            int jump_r = currPoint->next->r;
            int jump_c = currPoint->next->c;
            if ((c+r)%2) {
                fprintf(stdout, "Move: %d,%d to %d,%d\n", r, c, jump_r, jump_c);
                move_piece(r, c, jump_r, jump_c);
            } else {
                printf("RED: %d,%d\n", c, r);
            }
            currPoint = currPoint->next;
        }
        current = current->next;
    }

    fprintf(stdout, "New Board:\n");
    print_board(ROWS, COLS, board);

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-e")) {

        } else if (!strcmp(argv[i], "-h")) {

        }
    }

    return 0;
}

void read_args(int argc, char* argv[]) {
    /* loop through arguments
    *  for each "-e", check for e-filename after (doesn't start with '-')
    *  for each "-h", check for h-filename
    *  "-m" followed by N number of moves should be played
    */
    for (int i = 1; i < argc && argv[i] != NULL; i++) {
    if (n == -1 && !strcmp(argv[i], "-m")) {
    if (++i >= argc || !sscanf(argv[i], "%d", &n)) {
    fprintf(stderr, "Argument error: no value for '-n' flag");
    exit(1);
    }
    } else if (!strcmp(argv[i], "-m")) {
    i++;
    fprintf(stderr, "Multiple '-m' commands, assuming first\n");
    } else if (filename == NULL && argv[i][0] != '-') {
    filename = malloc(sizeof(char) * (strlen(argv[i])+1));
    strcpy(filename, argv[i]);
    }
    }
    if (n != -1) fprintf(stdout, "Number: %d\n", n);
    if (filename != NULL) {
    fprintf(stdout, "Input filename: %s\n", filename);
    freopen(filename, "r", stdin);
    }
}

char get_piece(int r, int c) {
    return board[ROWS-r][c];
}

void change_piece(int r, int c, char v) {
    board[ROWS-r][c] = v;
}

void move_piece(int start_r, int start_c, int end_r, int end_c) {
    change_piece(end_r, end_c, get_piece(start_r, start_c));
    change_piece(start_r, start_c, '.');
    if (get_piece(end_r, end_c) == 'r' && end_r == 8) change_piece(end_r, end_c, 'R');
    if (get_piece(end_r, end_c) == 'b' && end_r == 1) change_piece(end_r, end_c, 'B');
    if (capture_on && abs(start_r-end_r) == 2 && abs(start_c-end_c) == 2) {
        change_piece((start_r+end_r)/2, (start_c-end_c)/2, '.');
    }
}
