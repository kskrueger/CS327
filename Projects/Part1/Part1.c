//
// Created by Karter Krueger on 9/10/19.
//
#include <stdio.h>
#include <string.h>

/* INPUTS
 * ": empty red square
 * .: empty black square
 * b: black pawn
 * B: black king
 * r: red pawn
 * R: red king
 * ->: move coordinates
 * RULES:
 *  <capture> or <no capture>           # don't enforce pieces must be captured rule
 *  <single jumps> or <multiple jumps>  # allow multiple jumps
 * TURN:
 *  <red> or <black>
 * BOARD:
 *  "b"b"b"bb"b"b...
 * MOVES:
 *  e3->d4
 *  d4->b6
 */

char board[8][8];

void initialize_array(int rows, int cols, char array[rows][cols]);
void scan_in_array(int rows, int cols, char array[rows][cols], FILE *fptr);
void print_array(int rows, int cols, char array[rows][cols]);

int main() {
    initialize_array(8, 8, board);

    FILE *input_file = fopen("/Users/kskrueger/Documents/Classes/CS327/Code/cs327/Projects/Part1/test_input.txt", "r");
    if (input_file == NULL) {
        printf("Can't open file\n");
    }
    char out;
    fscanf(input_file, "BOARD%c", &out);

    scan_in_array(8, 8, board, input_file);

    print_array(8, 8, board);
    fclose(input_file);
}

void initialize_array(int rows, int cols, char array[rows][cols]) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            array[r][c] = 'x';
        }
    }
}

void scan_in_array(int rows, int cols, char array[rows][cols], FILE *fptr) {
    char input = fgetc(fptr);
    for (int r = 0; r < rows && input != EOF; r++) {
        for (int c = 0; c < cols && input != EOF;) {
            if (input == '\"' || input == 'b' || input == 'B' || input == 'r' || input == 'R'|| input == '.') {
                array[r][c] = input;
                c++;
            }
            input = fgetc(fptr);
        }
    }
}

void print_array(int rows, int cols, char array[rows][cols]) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf(" %c ", array[r][c]);
            if (c < cols - 1) {
                printf("|");
            }
        }
        if (r < rows - 1) {
            printf("\n---+---+---+---+---+---+---+---\n");
        }
    }
}

/* OUTPUT
 * VALID INPUT
 * Initial configuration:
 * Turn: red
 * Red: 0 kings, 12 pawns
 * Black: 0 kings, 12 pawns
 */
