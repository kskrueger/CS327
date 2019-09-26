//
// Created by Karter Krueger on 9/10/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(char *key, const int *line_num);
void skip_whitespace(int *line_num);
int search(char *word, int *line_num);
int search2(char *word0, char* word1, int *line_num, int* out);
int scan_board(int rows, int cols, char array[rows][cols], int *line_num);
void print_array(int rows, int cols, char array[rows][cols]);
void array_count(int rows, int cols, char array[rows][cols],
                 int* red_kings, int* red_pawns, int* black_kings, int* black_pawns);

const int COLS = 8;
const int ROWS = 8;

int main() {
    char board[8][8]; // initialize the board array
    int red_kings = 0, red_pawns = 0, black_kings = 0, black_pawns = 0; // initialize variables
    int capture_on = -1, multiple_jumps_on = -1, turn_red = -1, line_num = 1;

    // scan for each keyword and print error if it fails
    if (!(search("RULES:", &line_num))) error("RULES:", &line_num);
    if (!(search2("no capture", "capture", &line_num, &capture_on))) error("capture' or 'no capture", &line_num);
    if (!(search2("single jumps", "multiple jumps", &line_num, &multiple_jumps_on))) error("multiple jumps' or 'single jumps", &line_num);
    if (!(search("TURN:", &line_num))) error("TURN:", &line_num);
    if (!(search2("black", "red", &line_num, &turn_red))) error("red' or 'black", &line_num);
    if (!(search("BOARD:", &line_num))) error("BOARD:", &line_num);
    if (!(scan_board(8,8, board, &line_num))) {
        fprintf(stderr, "ERROR near line %d: board input/scan error", line_num);
        exit(1);
    }

    array_count(ROWS, COLS, board, &red_kings, &red_pawns, &black_kings, &black_pawns); // count the numbers on board
    fprintf(stdout, "VALID INPUT\n"); // print standard required output
    fprintf(stdout, "Initial configuration:\n");
    fprintf(stdout, "Turn: %s\n", turn_red ? "red" : "black");
    fprintf(stdout, "Red: %d kings, %d pawns\n", red_kings, red_pawns);
    fprintf(stdout, "Black: %d kings, %d pawns\n", black_kings, black_pawns);
}

// print an error message to template
void error(char *key, const int *line_num) {
    fprintf(stderr, "Error near line %d: expecting '%s'\n", *line_num, key);
    exit(1);
}

// skip over whitespaces and comments
void skip_whitespace(int *line_num) {
    char c;
    int comment = 0;
    while (fscanf(stdin, "%c", &c) && c!= EOF && (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '#' || comment)) {
        if (c == '\n') (*line_num)++; // increment line number
        if (comment) comment = (c != '\n'); // look for end of comment
        else comment = (c == '#'); // look for start of comment
    }
    ungetc(c, stdin);
}

// search for a word in the stdin input
int search(char *word, int *line_num) {
    skip_whitespace(line_num); // skip over whitespace and comments
    char c;
    int i = 0;
    // loop through to search for word and increment line number for each \n character
    while (fscanf(stdin, "%c", &c) && c!= EOF && (c == word[i++])) if (c == '\n') (*line_num)++;
    ungetc(c, stdin); // put the last char back so next function can use it
    return i-1 == strlen(word); // return 1 if word was found
}

// search for two words in the stdin input (words must start with different first char, which is fine for this)
int search2(char *word0, char* word1, int *line_num, int *word) {
    skip_whitespace(line_num); // skip over whitespace and comments
    char c;
    // select word 0 or 1 based on if the first letter matches (good enough for this project)
    int i = 0;
    *word = (fscanf(stdin, "%c", &c) && c == word1[0]);
    ungetc(c, stdin);
    // loop through to search for word and increment line number for each \n character
    while (!(*word) && fscanf(stdin, "%c", &c) && c!= EOF && (c == word0[i++])) if (c == '\n') (*line_num)++;
    while ((*word) && fscanf(stdin, "%c", &c) && c!= EOF && (c == word1[i++])) if (c == '\n') (*line_num)++;
    ungetc(c, stdin); // put the last char back so next function can use it
    return (i-1 == (!(*word) ? strlen(word0):strlen(word1))); // return 1 or 2 if word was found
}

// scan the board into the array and check for board red/black errors along the way
int scan_board(int rows, int cols, char array[rows][cols], int *line_num) {
    int row = 0, col = 0;
    char c;
    skip_whitespace(line_num); // skip whitespace to start
    fscanf(stdin, "%c", &c);
    int black_square = (c == '\"'); // check if top left corner is black or red for FLIPPED board
    for (row = 0; row < rows && c != EOF; row++, black_square = !black_square) {  // invert black_square each loop
        for (col = 0; col < cols && c != EOF;) {
            if (c == '\n') (*line_num)++;
            if (c == '\"' || c == 'b' || c == 'B' || c == 'r' || c == 'R'|| c == '.') {
                array[row][col++] = c; // put value into array
                if (!(black_square = !black_square)) {
                    if (c != '\"') return 0; // exit if fault in pattern
                } else if (c != 'r' && c != 'R' && c != 'b' && c != 'B' && c != '.') return 0; // exit if pattern fault
            }
            fscanf(stdin, "%c", &c);
            if (c != '\"' && c != 'b' && c != 'B' && c != 'r' && c != 'R'&& c != '.'
                && c != ' ' && c != '\n' && c != '+' && c != '|' && c != '-') return 0; // exit if invalid character
        }
    }
    return 1; // return 1 for good if it reaches this point
}

// count the pieces
void array_count(int rows, int cols, char array[rows][cols],
        int* red_kings, int* red_pawns, int* black_kings, int* black_pawns) {
    int r, c;
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            if (array[r][c] == 'r') (*red_pawns)++;
            if (array[r][c] == 'R') (*red_kings)++;
            if (array[r][c] == 'b') (*black_pawns)++;
            if (array[r][c] == 'B') (*black_kings)++;
        }
    }
}

//  print the checkers board array (not required)
void print_array(int rows, int cols, char array[rows][cols]) {
    int row = 0, col = 0;
    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            fprintf(stdout, " %c ", array[row][col]);
            if (col < cols - 1) fprintf(stdout, "|");
        }
        if (row < rows - 1) fprintf(stdout, "\n---+---+---+---+---+---+---+---\n");
    }
    fprintf(stdout, "\n");
}
