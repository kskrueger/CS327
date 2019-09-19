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
int search_word(char *word, FILE *file);
void print_array(int rows, int cols, char array[rows][cols]);

int main() {
    initialize_array(8, 8, board);

    FILE *input_file = fopen("/Users/kskrueger/Documents/Classes/CS327/Code/cs327/Projects/Part1/test_input.txt", "r");
    if (input_file == NULL) {
        printf("Can't open file\n");
    }
    search_word("BOARD:", input_file);


    search_word("BOARD:", input_file);
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

int search_word(char *word, FILE *file) {
    fseek(file, 0, SEEK_SET);
    int found = 0;
    char ch = fgetc(file);
    int length = strlen(word);
    int index = 0;
    while (!found && ch != EOF) {
        index = ftell(file);
        if (ch == word[0]) {
            int i;
            for (i = 0; i < length; i++) {
                if (ch != word[i]) {
                    found = 0;
                    break;
                } else {
                    found = 1;
                }
                ch = fgetc(file);
            }
        }
        fseek(file, index, SEEK_SET);
        ch = fgetc(file);
    }
    fseek(file, index-1+length, SEEK_SET);
    return index-1;
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
