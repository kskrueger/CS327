//
// Created by Karter Krueger on 9/10/19.
//
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

void error(char *key);
void skip_whitespace();
int search(char *word);
int search2(char *word0, char* word1, int* out);
int scan_board(int rows, int cols, char array[rows][cols]);
void print_array(int rows, int cols, char array[rows][cols]);
void array_count(int rows, int cols, char array[rows][cols],
                 int* red_kings, int* red_pawns, int* black_kings, int* black_pawns);
move scan_moves();

const int COLS = 8;
const int ROWS = 8;
int line_num = 1;

char board[8][8]; // initialize the board array
int red_kings = 0, red_pawns = 0, black_kings = 0, black_pawns = 0; // initialize variables
int capture_on = -1, multiple_jumps_on = -1, turn_red = -1, errorOut = 0;

move moves;

void scan_input() {
    // scan for each keyword and print error if it fails
    if (!(search("RULES:"))) error("RULES:");
    if (!(search2("no capture", "capture", &capture_on))) error("capture' or 'no capture");
    if (!(search2("single jumps", "multiple jumps", &multiple_jumps_on))) error("multiple jumps' or 'single jumps");
    if (!(search("TURN:"))) error("TURN:");
    if (!(search2("black", "red", &turn_red))) error("red' or 'black");
    if (!(search("BOARD:"))) error("BOARD:");
    if ((errorOut = scan_board(8,8, board))) {
        if (errorOut == -1) fprintf(stderr, "ERROR near line %d: unexpected end of file", line_num);
        else if (errorOut == 1) fprintf(stderr, "ERROR near line %d: board error, pattern invalid", line_num);
        else if (errorOut == 2) fprintf(stderr, "ERROR near line %d: board input error, invalid character", line_num);
        exit(1);
    }
    if (!(search("MOVES:"))) error("MOVES:");

    moves = scan_moves();
    move current = moves;
    int m = 0, p = 0;
    while (current->next != NULL) {
        m++;
        while (current->point->next != NULL) {
            p++;
            if (current->point->c > 8 || current->point->c <= 0 || current->point->r > 8 || current->point->r <= 0)
                fprintf(stderr, "ERROR near line %d: move %d at jump %d is out of bounds/invalid\n", line_num, m, p);
            printf("%d: %d, %d\n", current->length, current->point->r, current->point->c);
            current->point = current->point->next;
        }
        current = current->next;
    }

    array_count(ROWS, COLS, board, &red_kings, &red_pawns, &black_kings, &black_pawns); // count the numbers on board
    fprintf(stdout, "VALID INPUT\n"); // print standard required output
    fprintf(stdout, "Initial configuration:\n");
    fprintf(stdout, "Turn: %s\n", turn_red ? "red" : "black");
    fprintf(stdout, "Red: %d kings, %d pawns\n", red_kings, red_pawns);
    fprintf(stdout, "Black: %d kings, %d pawns\n", black_kings, black_pawns);
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
    while (fscanf(stdin, "%c", &c) == 1 && c!= EOF && (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '#' || comment)) {
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
    for (row = 0; row < rows && c != EOF && scans == 1; row++, black_square = !black_square) {  // invert black_square each loop
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
        current->point = currentpt;

        skip_whitespace();
        while((scans1 = fscanf(stdin, "%c%d%c", &letter, &number, &c)) == 3) {
            count++;
            currentpt->r = number;
            currentpt->c = letter - 'a';
            point nextpt = malloc(sizeof(struct point_node));
            currentpt->next = nextpt;
            currentpt = currentpt->next;
            if ((c != '-') || (scans2 = fscanf(stdin, "%c", &c)) != 1) break;
            if (c != '>') return NULL;
        }
        currentpt->next = NULL;
        current->length = count;
        move next = malloc(sizeof(struct move_node));
        current->next = next;
        current = current->next;
    }
    current->next = NULL;
    return head;
}
