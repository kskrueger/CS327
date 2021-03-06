//
// Created by Karter Krueger on 10/22/19.
//

#ifndef CS327_CHECKERS_H
#define CS327_CHECKERS_H

#endif //CS327_CHECKERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// move and point structures to likely be used in later later parts
typedef struct point_node {
    int c;
    int r;
    struct point_node* next;
} *point;

typedef struct move_node {
    int length;
    point point;
    struct move_node* next;
} *move;

const int COLS = 8;
const int ROWS = 8;
int line_num = 1;

void error(char *key);
void skip_whitespace();
int search(char *word);
int search2(char *word0, char* word1, int* out);
int scan_board(int rows, int cols, char array[rows][cols]);
void print_board(int rows, int cols, char array[rows][cols]);
void array_count(int rows, int cols, char array[rows][cols],
                 int* red_kings, int* red_pawns, int* black_kings, int* black_pawns);
move scan_moves();
void read_args(int argc, char* argv[]);
int count_moves();
void move_piece(int start_r, int start_c, int end_r, int end_c);
void print_board_full(FILE*, int rows, int cols, char array[rows][cols]);
void make_moves();

int n_moves = -1;
char *filename = NULL;
int moves_made = 0;
int illegal_move = 0;

char board[8][8]; // initialize the board array
int red_kings = 0, red_pawns = 0, black_kings = 0, black_pawns = 0; // initialize variables
int capture_on = -1, multiple_jumps_on = -1, turn_red = -1, errorOut = 0;
int board_flipped = 0;

move moves;

void scan_input() {
    // scan for each keyword and print error if it fails
    if (!(search("RULES:"))) error("RULES:");
    if (!(search2("no capture", "capture", &capture_on))) error("capture' or 'no capture");
    if (!(search2("single jumps", "multiple jumps", &multiple_jumps_on))) error("multiple jumps' or 'single jumps");
    if (!(search("TURN:"))) error("TURN:");
    if (!(search2("black", "red", &turn_red))) error("red' or 'black");
    if (!(search("BOARD:"))) error("BOARD:");
    if ((errorOut = scan_board(8, 8, board))) {
        if (errorOut == -1) fprintf(stderr, "ERROR near line %d: unexpected end of file", line_num);
        else if (errorOut == 1) fprintf(stderr, "ERROR near line %d: board error, pattern invalid", line_num);
        else if (errorOut == 2) fprintf(stderr, "ERROR near line %d: board input error, invalid character", line_num);
        exit(1);
    }
    if (!(search("MOVES:"))) error("MOVES:");

    moves = scan_moves();
    move current = moves;
    int m, p;
    for (m = 0; current->next != NULL; m++) {
        point currPoint = current->point;
        for (p = 0; currPoint->next != NULL; p++) {
            if (currPoint->c >= 8 || currPoint->c < 0 || currPoint->r > 8 || currPoint->r <= 0) {
                fprintf(stderr, "ERROR near line %d: move %d at jump %d is out of bounds/invalid\n", line_num, m, p);
                exit(1);
            }
            currPoint = currPoint->next;
        }
        current = current->next;
    }

    array_count(ROWS, COLS, board, &red_kings, &red_pawns, &black_kings, &black_pawns); // count the numbers on board
}

// print an error message to template
void error(char *key) {
    fprintf(stderr, "Error near line %d: expecting '%s'\n", line_num, key);
    exit(1);
}

// skip over whitespaces and comments
void skip_whitespace() {
    char c;
    int comment = 0;
    while (fscanf(stdin, "%c", &c) == 1 && c!= EOF && (c == ' ' || c == '\t' || c == '\n' || c == '\r'
    || c == '#' || c == '-' || c == '+' || c == '|' || comment)) {
        if (c == '\n') line_num++; // increment line number
        if (comment) comment = (c != '\n'); // look for end of comment
        else comment = (c == '#'); // look for start of comment
    }
    ungetc(c, stdin);
}

// search for a word in the stdin input
int search(char *word) {
    skip_whitespace(); // skip over whitespace and comments
    char c;
    int i = 0;
    // loop through to search for word and increment line number for each \n character
    while (fscanf(stdin, "%c", &c) == 1 && c!= EOF && (c == word[i++])) if (c == '\n') line_num++;
    ungetc(c, stdin); // put the last char back so next function can use it
    return i-1 == strlen(word); // return 1 if word was found
}

// search for two words in the stdin input (words must start with different first char, which is fine for this)
int search2(char *word0, char* word1, int *word) {
    skip_whitespace(); // skip over whitespace and comments
    char c;
    // select word 0 or 1 based on if the first letter matches (good enough for this project)
    int i = 0;
    *word = (fscanf(stdin, "%c", &c) == 1 && c == word1[0]);
    ungetc(c, stdin);
    // loop through to search for word and increment line number for each \n character
    while (!(*word) && fscanf(stdin, "%c", &c) == 1 && c!= EOF && (c == word0[i++])) if (c == '\n') line_num++;
    while ((*word) && fscanf(stdin, "%c", &c) == 1 && c!= EOF && (c == word1[i++])) if (c == '\n') line_num++;
    ungetc(c, stdin); // put the last char back so next function can use it
    return (i-1 == (!(*word) ? strlen(word0):strlen(word1))); // return 1 or 2 if word was found
}

// scan the board into the array and check for board red/black errors along the way
int scan_board(int rows, int cols, char array[rows][cols]) {
    int row = 0, col = 0, scans = 1;
    char c;
    skip_whitespace(); // skip whitespace to start
    if (fscanf(stdin, "%c", &c) != 1) return -1; // return -1 if not reading more input
    int black_square = (c == '\"'); // check if top left corner is black or red for FLIPPED board
    board_flipped = 1;
    for (row = 0; row < rows && c != EOF && scans == 1; row++, black_square = !black_square) {  // invert black_square each loop
        skip_whitespace();
        for (col = 0; col < cols && c != EOF && scans == 1;) {
            if (c == '\n') line_num++;
            if (c == '\"' || c == 'b' || c == 'B' || c == 'r' || c == 'R'|| c == '.') {
                array[row][col++] = c; // put value into array
                if (!(black_square = !black_square)) {
                    if (c != '\"') return 1; // exit if fault in pattern
                } else if (c != 'r' && c != 'R' && c != 'b' && c != 'B' && c != '.') return 1; // exit if pattern fault
            }
            if ((scans = fscanf(stdin, "%c", &c)) != 1) return -1; // return -1 if not reading more input
            if (c != '\"' && c != 'b' && c != 'B' && c != 'r' && c != 'R'&& c != '.'
                && c != ' ' && c != '\n' && c != '+' && c != '|' && c != '-') return 2; // exit if invalid character
        }
    }
    return 0; // return 0 for good if it reaches this point
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
void print_board(int rows, int cols, char array[rows][cols]) {
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

move scan_moves() {
    move head = malloc(sizeof(struct move_node));
    head->length = 0;
    head->point = NULL;
    head->next = NULL;
    move current = head;

    skip_whitespace();

    char c = ' ';
    int scans1 = 3, scans2 = 1;
    while (scans1 == 3 && scans2 == 1 && c != EOF) {
        int count = 0;
        char letter;
        int number;
        point currentpt = malloc(sizeof(struct point_node));
        currentpt->r = currentpt->c = -1;
        currentpt->next = NULL;
        current->point = currentpt;

        skip_whitespace();
        while((scans1 = fscanf(stdin, "%c%d%c", &letter, &number, &c)) == 3) {
            count++;
            currentpt->r = number;
            currentpt->c = letter - 'a';
            point nextpt = malloc(sizeof(struct point_node));
            nextpt->r = nextpt->c = -1;
            nextpt->next = NULL;
            currentpt->next = nextpt;
            currentpt = currentpt->next;
            if ((c != '-') || (scans2 = fscanf(stdin, "%c", &c)) != 1) break;
            if (c != '>') return NULL;
        }
        current->length = count;
        move next = malloc(sizeof(struct move_node));
        current->next = next;
        current = current->next;
    }
    current->next = NULL;
    return head;
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
                fprintf(stderr, "Argument error: no value for '-m' flag\n");
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
    if (filename != NULL) {
        freopen(filename, "r", stdin);
    }
}

int count_moves() {
    int move_count = 0;
    move curr = moves;
    while (curr->next->next != NULL) {
        move_count++;
        curr = curr->next;
    }
    return move_count;
}

//  print the checkers board array
void print_board_full(FILE* file, int rows, int cols, char array[rows][cols]) {
    fprintf(file, "    a   b   c   d   e   f   g   h\n");
    fprintf(file, "  +---+---+---+---+---+---+---+---+\n");
    int row = 0, col = 0;
    for (row = 0; row < rows; row++) {
        fprintf(file, "%d |", ROWS-row);
        for (col = 0; col < cols; col++) {
            fprintf(file, " %c ", array[row][col]);
            fprintf(file, "|");
        }
        fprintf(file, " %d", ROWS-row);
        if (row < rows) fprintf(file, "\n  +---+---+---+---+---+---+---+---+\n");
    }
    fprintf(file, "    a   b   c   d   e   f   g   h\n");
}

void make_moves() {
    move current = moves;
    int m, p;
    for (m = 0; current->next->next != NULL; m++) {
        point currPoint = current->point;
        for (p = 0; currPoint->next->next != NULL && moves_made < n_moves; p++) {
            int r = currPoint->r;
            int c = currPoint->c;
            int jump_r = currPoint->next->r;
            int jump_c = currPoint->next->c;
            if (!multiple_jumps_on && (current->length > 2 || p > 1)) {
                fprintf(stderr, "ERROR: Multiple jumps attempted while off\n");
                fprintf(stdout, "Move %d is illegal: %c%d->%c%d\n", moves_made+1, c+'a', r, jump_c+'a', jump_r);
                illegal_move = moves_made + 1;
            }
            if (((c+r)%2) == board_flipped) {
                //fprintf(stdout, "Move %d: %c%d->%c%d, Turn: %s\n", moves_made+1, c+'a', r, jump_c+'a', jump_r, turn_red?"red":"black");
                move_piece(r, c, jump_r, jump_c);
                //print_board_full(stdout, ROWS, COLS, board);
            } else {
                fprintf(stderr, "ERROR: Move %d on RED square at %d,%d\n", moves_made+1, c, r);
                fprintf(stdout, "Move %d is illegal: %c%d->%c%d\n", moves_made+1, c+'a', r, jump_c+'a', jump_r);
                illegal_move = moves_made + 1;
            }
            currPoint = currPoint->next;
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
    int curr_red = -1;
    if (start_piece == 'r' || start_piece == 'R') {
        curr_red = 1;
    } else if (start_piece == 'b' || start_piece == 'B') {
        curr_red = 0;
    }
    if (curr_red != turn_red) {
        fprintf(stderr, "ERROR, at move %d: Move out of turn\n", moves_made + 1);
        fprintf(stdout, "Move %d is illegal: %c%d->%c%d\n", moves_made + 1, start_c + 'a', start_r, end_c + 'a', end_r);
        illegal_move = moves_made + 1;
        return;
    } else if (((end_c + end_r) % 2) != board_flipped) {
        fprintf(stderr, "ERROR, at move %d: Cannot move to RED square\n", moves_made+1);
        fprintf(stdout, "Move %d is illegal: %c%d->%c%d\n", moves_made+1, start_c+'a', start_r, end_c+'a', end_r);
        illegal_move = moves_made + 1;
        return;
    } else if (end_piece != '.') {
        fprintf(stderr, "ERROR, at move %d: Cannot move to non-empty square\n", moves_made+1);
        fprintf(stdout, "Move %d is illegal: %c%d->%c%d\n", moves_made+1, start_c+'a', start_r, end_c+'a', end_r);
        illegal_move = moves_made + 1;
        return;
    } else if (start_piece == 'r' && (end_r-start_r) <= 0) {
        fprintf(stderr, "ERROR, at move %d: Pawn cannot move backwards\n", moves_made+1);
        fprintf(stdout, "Move %d is illegal: %c%d->%c%d\n", moves_made+1, start_c+'a', start_r, end_c+'a', end_r);
        illegal_move = moves_made + 1;
        return;
    } else if (start_piece == 'b' && (end_r - start_r) >= 0) {
        fprintf(stderr, "ERROR, at move %d: Pawn can only move forward\n", moves_made+1);
        fprintf(stdout, "Move %d is illegal: %c%d->%c%d\n", moves_made+1, start_c+'a', start_r, end_c+'a', end_r);
        illegal_move = moves_made + 1;
        return;
    } else if (abs(end_r - start_r) != abs(end_c - start_c)) {
        fprintf(stderr, "ERROR, at move %d: Can only move at diagonal\n", moves_made+1);
        fprintf(stdout, "Move %d is illegal: %c%d->%c%d\n", moves_made+1, start_c+'a', start_r, end_c+'a', end_r);
        illegal_move = moves_made + 1;
        return;
    } else if ((abs(end_r - start_r) > 2) || (abs(end_c - start_c) > 2)) {
        fprintf(stderr, "ERROR, at move %d: Cannot move that far\n", moves_made+1);
        fprintf(stdout, "Move %d is illegal: %c%d->%c%d\n", moves_made+1, start_c+'a', start_r, end_c+'a', end_r);
        illegal_move = moves_made + 1;
        return;
    } else if (start_piece == '.') {
        fprintf(stderr, "ERROR, at move %d: Not a valid piece at start\n", moves_made + 1);
        fprintf(stdout, "Move %d is illegal: %c%d->%c%d\n", moves_made + 1, start_c + 'a', start_r, end_c + 'a', end_r);
        illegal_move = moves_made + 1;
        return;
    } else if (abs(start_r-end_r) == 2 && abs(start_c-end_c) == 2) {
        int captured_r = (start_r+end_r)/2;
        int captured_c = (start_c+end_c)/2;
        char jumped_piece = get_piece(captured_r, captured_c);
        if ((curr_red && (jumped_piece == 'b' || jumped_piece == 'B')) || (!curr_red  && (jumped_piece == 'r' || jumped_piece == 'R'))) {
            change_piece(captured_r, captured_c, '.');
        } else {
            fprintf(stderr, "ERROR, at move %d: Cannot jump over that square\n", moves_made + 1);
            fprintf(stdout, "Move %d is illegal: %c%d->%c%d\n", moves_made + 1, start_c + 'a', start_r, end_c + 'a', end_r);
            illegal_move = moves_made + 1;
            return;
        }
    }
    change_piece(end_r, end_c, start_piece);
    change_piece(start_r, start_c, '.');
    if (get_piece(end_r, end_c) == 'r' && end_r == 8) change_piece(end_r, end_c, 'R');
    if (get_piece(end_r, end_c) == 'b' && end_r == 1) change_piece(end_r, end_c, 'B');
    turn_red = !turn_red;
    moves_made++;
}
