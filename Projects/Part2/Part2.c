//
// Created by Karter Krueger on 9/10/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Part1.c"

void read_args(int argc, char* argv[]);
void move_piece(int start_r, int start_c, int end_r, int end_c);
void print_board_full(int rows, int cols, char array[rows][cols]);
void make_moves();

int n_moves = -1;
char *filename = NULL;
int moves_made = 0;

int main(int argc, char* argv[]) {
    read_args(argc, argv);
    scan_input();

    // print_board_full(ROWS, COLS, board);
    make_moves();

    // fprintf(stdout, "New Board:\n");
    // print_board_full(ROWS, COLS, board);

    fprintf(stdout, "Moves made: %d\n", moves_made);

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
        if (n_moves == -1 && !strcmp(argv[i], "-m")) {
            if (++i >= argc || !sscanf(argv[i], "%d", &n_moves)) {
                fprintf(stderr, "Argument error: no value for '-m' flag");
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
    // if (n_moves != -1) fprintf(stdout, "Number: %d\n", n_moves);
    if (filename != NULL) {
        // fprintf(stdout, "Input filename: %s\n", filename);
        freopen(filename, "r", stdin);
    }
}

//  print the checkers board array (not required)
void print_board_full(int rows, int cols, char array[rows][cols]) {
    fprintf(stdout, "    a   b   c   d   e   f   g   h\n");
    fprintf(stdout, "  +---+---+---+---+---+---+---+---+\n");
    int row = 0, col = 0;
    for (row = 0; row < rows; row++) {
        fprintf(stdout, "%d |", ROWS-row);
        for (col = 0; col < cols; col++) {
            fprintf(stdout, " %c ", array[row][col]);
            fprintf(stdout, "|");
        }
        fprintf(stdout, " %d", ROWS-row);
        if (row < rows) fprintf(stdout, "\n  +---+---+---+---+---+---+---+---+\n");
    }
    fprintf(stdout, "    a   b   c   d   e   f   g   h\n");
}

void make_moves() {
    move current = moves;
    int m, p;
    for (m = 0; current->next->next != NULL; m++) {
        point currPoint = current->point;
        for (p = 0; currPoint->next->next != NULL; p++) {
            if (!multiple_jumps_on && (current->length > 2 || p > 1)) {
                fprintf(stderr, "ERROR: Multiple jumps attempted while off\n");
                continue;
            }
            int r = currPoint->r;
            int c = currPoint->c;
            int jump_r = currPoint->next->r;
            int jump_c = currPoint->next->c;
            if ((c+r)%2) {
                //fprintf(stdout, "Move %d: %c%d (%d,%d) to %c%d (%d,%d)\n", moves_made+1, c+'a', r, r, c, jump_c+'a', jump_r, jump_r, jump_c);
                fprintf(stdout, "Move %d: %c%d->%c%d\n", moves_made+1, c+'a', r, jump_c+'a', jump_r);
                move_piece(r, c, jump_r, jump_c);
                //print_board_full(ROWS, COLS, board);
            } else {
                fprintf(stdout, "ERROR: Move %d on RED square at %d,%d\n", moves_made+1, c, r);
                exit(1);
            }
            currPoint = currPoint->next;
            if (++moves_made >= n_moves && n_moves != -1) return;
        }
        current = current->next;
    }
}

char get_piece(int r, int c) {
    return board[ROWS-r][c];
}

void change_piece(int r, int c, char v) {
    board[ROWS-r][c] = v;
}

void move_piece(int start_r, int start_c, int end_r, int end_c) {
    char start_piece = get_piece(start_r, start_c);
    char end_piece = get_piece(end_r, end_c);
    if (end_piece != '.') fprintf(stderr, "ERROR, at move %d: Cannot move to non-empty square\n", moves_made+1);
    if (start_piece == 'r' && (end_r-start_r) < 0) fprintf(stderr, "ERROR, at move %d: Pawn cannot move backwards\n", moves_made+1);
    if (start_piece == 'b' && (end_r-start_r) > 0) fprintf(stderr, "ERROR, at move %d: Pawn cannot move backwards\n", moves_made+1);
    change_piece(end_r, end_c, start_piece);
    change_piece(start_r, start_c, '.');
    if (get_piece(end_r, end_c) == 'r' && end_r == 8) change_piece(end_r, end_c, 'R');
    if (get_piece(end_r, end_c) == 'b' && end_r == 1) change_piece(end_r, end_c, 'B');
    if (abs(start_r-end_r) == 2 && abs(start_c-end_c) == 2) {
        //fprintf(stdout, "CAPTURED, on move %d, at r%d, c%d\n", moves_made+1, (start_r+end_r)/2, (start_c-end_c)/2);
        change_piece((start_r+end_r)/2, (start_c+end_c)/2, '.');
    }
}
