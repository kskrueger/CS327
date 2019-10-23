//
// Created by Karter Krueger on 9/10/19.
//
#include "Checkers.h"

int main(int argc, char* argv[]) {
    read_args(argc, argv);
    scan_input();
    int count_of_moves = count_moves();
    if (n_moves == -1 || n_moves > count_of_moves) n_moves = count_of_moves;

    fprintf(stdout, "Input file has %d moves\n", count_of_moves);
    fprintf(stdout, "Processing %d moves\n", n_moves);
    //print_board_full(stdout, ROWS, COLS, board);
    make_moves();

    if (!illegal_move) fprintf(stdout, "All moves are legal\n");

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-e")) {
            FILE* file = fopen(argv[++i], "w");

            fprintf(file, "RULES: %s capture %s jumps ", capture_on ? "" : "no", multiple_jumps_on ? "multiple" : "single");
            fprintf(file, "TURN: %s ", turn_red ? "red" : "black");
            fprintf(file, "BOARD: ");
            int x, y;
            for (y = 0; y < ROWS; y++) {
                for (x = 0; x < COLS; x++) {
                    fprintf(file, "%c", board[y][x]);
                }
            }
            fprintf(file, " MOVES: ");
            move curr = moves;
            int j;
            for (j = 0; j < n_moves-illegal_move; j++) curr = curr->next;
            while (curr->next->next != NULL) {
                point currPoint = curr->point;
                while (currPoint->next != NULL) {
                    fprintf(file, "%c%d", currPoint->c+'a', currPoint->r);
                    currPoint = currPoint->next;
                    if (currPoint->next != NULL) {
                        fprintf(file, "->");
                    }
                }
                curr = curr->next;
            }
            fprintf(file, "\n");
            fclose(file);
        } else if (!strcmp(argv[i], "-h")) {
            FILE* file = fopen(argv[++i], "w");
            print_board_full(file, ROWS, COLS, board);
            fprintf(file, "\n");
            move curr = moves;
            int j;
            for (j = 0; j < n_moves-illegal_move; j++) curr = curr->next;
            while (curr->next->next != NULL) {
                point currPoint = curr->point;
                while (currPoint->next != NULL) {
                    fprintf(file, "%c%d", currPoint->c+'a', currPoint->r);
                    currPoint = currPoint->next;
                    if (currPoint->next != NULL) {
                        fprintf(file, "->");
                    }
                }
                fprintf(file, "\n");
                curr = curr->next;
            }
            fclose(file);
        }
    }

    return 0;
}
