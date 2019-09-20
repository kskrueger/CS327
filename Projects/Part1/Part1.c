//
// Created by Karter Krueger on 9/10/19.
//
#include <stdio.h>
#include <stdlib.h>
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

const int ROWS = 8;
const int COLS = 8;

struct point {
    int c;
    int r;
};

void initialize_array(int rows, int cols, char array[rows][cols]);
int scan_in_array(int rows, int cols, char array[rows][cols], FILE *fptr);
void array_count(int rows, int cols, char array[rows][cols],
        int* red_kings, int* red_pawns, int* black_kings, int* black_pawns);
int check_board(int rows, int cols, char array[rows][cols]);
int search_word(char *word, FILE *file, int start);
struct point* scan_move(FILE *file, int start, int *length);
void print_array(int rows, int cols, char array[rows][cols]);

int main() {
    int index;
    char board[ROWS][COLS];
    int red_kings, red_pawns, black_kings, black_pawns;
    initialize_array(ROWS, COLS, board);
    FILE *input_file = fopen("/Users/kskrueger/Documents/Classes/CS327/Code/cs327/Projects/Part1/test_input3.txt", "r");
    if (input_file == NULL) {
        printf("RULES: Can't open file\n");
        return -1;
    }

    int capture_on = -1, multiple_jumps_on = -1, turn_red = -1, board_flipped;
    if ((index = search_word("RULES:", input_file, 0)) == -1) {
        printf("ERROR: RULES not found!\n");
    } else {
        if (search_word("no capture", input_file, index) != -1) {
            capture_on = 0;
        } else if (search_word("capture", input_file, index) != -1) {
            capture_on = 1;
        } else {
            printf("ERROR: Capture rule not stated\n");
            return -1;
        }

        if (search_word("multiple jumps", input_file, index) != -1) {
            multiple_jumps_on = 1;
        } else if (search_word("single jumps", input_file, index) != -1) {
            multiple_jumps_on = 0;
        } else {
            printf("ERROR: Jumps rule not stated\n");
            return -1;
        }
    }

    if ((index = search_word("TURN:", input_file, 0)) == -1) {
        printf("ERROR: TURN not found!\n");
        return -1;
    } else {
        if (search_word("red", input_file, index) != -1) {
            turn_red = 1;
        } else if (search_word("black", input_file, index) != -1) {
            turn_red = 0;
        } else {
            printf("ERROR: Turn not stated\n");
        }
    }

    if (search_word("BOARD:", input_file, index) == -1) {
        printf("ERROR: BOARD not found\n");
        return -1;
    }
    if (scan_in_array(ROWS, COLS, board, input_file) == -1) {
        printf("ERROR: Board scan error\n");
        return -1;
    }

    board_flipped = check_board(ROWS, COLS, board);
    if (board_flipped == -1) {
        printf("ERROR: Board invalid\n");
        return -1;
    }

    int moves_length = 0;
    struct point *moves;
    if ((index = search_word("MOVES:", input_file, 0)) == -1) {
        printf("ERROR: MOVES not found\n");
        return -1;
    } else {
        // TODO: array of move arrays, (point** ??)
        moves = scan_move(input_file, index, &moves_length);
    }
    /*for (int i = 0; i < moves_length; i++) {
        printf("%d, %d\n", moves[i].r, moves[i].c);
    }*/

    array_count(ROWS, COLS, board, &red_kings, &red_pawns, &black_kings, &black_pawns);

    printf("VALID INPUT\n");
    printf("Initial configuration\n");
    printf("Turn: %s\n", turn_red ? "red" : "black");
    printf("Red: %d kings, %d pawns\n", red_kings, red_pawns);
    printf("Black: %d kings, %d pawns\n", black_kings, black_pawns);
    if (board_flipped) printf("Flipped board");

    //print_array(ROWS, COLS, board);
    fclose(input_file);
}

void initialize_array(int rows, int cols, char array[rows][cols]) {
    int r, c;
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            array[r][c] = 'x';
        }
    }
}

int scan_in_array(int rows, int cols, char array[rows][cols], FILE *fptr) {
    char input = fgetc(fptr);
    int r = 0;
    int c = 0;
    for (r = 0; r < rows && input != EOF; r++) {
        for (c = 0; c < cols && input != EOF;) {
            if (input == '\"' || input == 'b' || input == 'B' || input == 'r' || input == 'R'|| input == '.') {
                array[r][c] = input;
                c++;
            }
            input = fgetc(fptr);
        }
    }
    if (c != cols) {
        return -1;
    }
}

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

int check_board(int rows, int cols, char array[rows][cols]) {
    int black_square = 0;
    if (array[0][0] == '\"') {
        black_square = 1;
    }
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            char ch = array[r][c];
            if (black_square) {
                if (ch != '\"') return -1;
            } else {
                if (ch != 'r' && ch != 'R' && ch != 'b' && ch != 'B' && ch != '.') return -1;
            }
            black_square = !black_square;
        }
        black_square = !black_square;
    }
    return !black_square;
}

int search_word(char *word, FILE *file, int start) {
    fseek(file, start, SEEK_SET);
    int found = 0;
    char ch = fgetc(file);
    int length = strlen(word);
    int index = 0;
    int comment = 0;
    while (!found && ch != EOF) {
        index = ftell(file);
        if (ch == '#' || comment) {
            comment = 1;
            if (ch == '\n') comment = 0;
        } else if (ch == word[0]) {
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
    if (ch == EOF) {
        fseek(file, 0, SEEK_SET);
        return -1;
    }
    fseek(file, index-1+length, SEEK_SET);
    return index-1;
}

struct point* scan_move(FILE *file, int start, int *length) {
    int index = search_word("->", file, start);
    fseek(file, index, SEEK_SET);
    char ch = fgetc(file);
    int count = 1;
    while (ch != ' ' && ch != EOF && ch != '\n' && ch != '\t') {
        if (ch == '-') count++;
        ch = fgetc(file);
    }

    struct point *moves = malloc(sizeof(struct point) * count);
    char letter;
    int number;
    for (int i = 0; i < count; i++) {
        fseek(file, index-2+i*4, SEEK_SET);
        fscanf(file, "%c%d", &letter, &number);
        moves[i].r = number;
        moves[i].c = letter - 'a';
    }
    *length = count;
    return moves;
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
