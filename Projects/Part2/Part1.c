//
// Created by Karter Krueger on 9/10/19.
//
#include "Checkers.h"

int main() {
    scan_input();

    fprintf(stdout, "VALID INPUT\n"); // print standard required output
    fprintf(stdout, "Initial configuration:\n");
    fprintf(stdout, "Turn: %s\n", turn_red ? "red" : "black");
    fprintf(stdout, "Red: %d kings, %d pawns\n", red_kings, red_pawns);
    fprintf(stdout, "Black: %d kings, %d pawns\n", black_kings, black_pawns);
}

