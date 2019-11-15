//
// Created by Karter Krueger on 11/12/19.
//

#include "Checkers.h"

int main(int argc, char* argv[]) {
    read_args(argc, argv);
    scan_input();
    int count_of_moves = count_moves();
    if (n_moves == -1 || n_moves > count_of_moves) n_moves = count_of_moves;

    make_moves();
    //if (!illegal_move) fprintf(stdout, "All moves are legal\n");
    //print_board_full(stdout, ROWS, COLS, board);

    start_recursive_score(board, 1);

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-e")) {
            fileE(argv, &i);
        } else if (!strcmp(argv[i], "-h")) {
            fileH(argv, &i);
        }
    }

    return 0;
}
