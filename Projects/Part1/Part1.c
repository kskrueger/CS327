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

struct point_node {
    int c;
    int r;
    struct point_node* next;
};
typedef struct point_node* point;

struct move_node {
    int length;
    point point;
    struct move_node* next;
};
typedef struct move_node* move;

void initialize_array(int rows, int cols, char array[rows][cols]);
int scan_in_array(int rows, int cols, char array[rows][cols]);
void array_count(int rows, int cols, char array[rows][cols],
        int* red_kings, int* red_pawns, int* black_kings, int* black_pawns);
int check_board(int rows, int cols, char array[rows][cols]);
int search_word(char *word);
move scan_moves();
void print_array(int rows, int cols, char array[rows][cols]);
int getLineNumber();

        int main() {
    int index;
    char board[ROWS][COLS];
    int red_kings, red_pawns, black_kings, black_pawns;
    initialize_array(ROWS, COLS, board);

    int capture_on = -1, multiple_jumps_on = -1, turn_red = -1, board_flipped;
    if ((index = search_word("RULES:")) == -1) {
        fprintf(stderr, "Error near line %d: expecting 'RULES:'\n", getLineNumber());
        return -1;
    } else {
        if (search_word("no capture") != -1) {
            capture_on = 0;
        } else if (search_word("capture") != -1) {
            capture_on = 1;
        } else {
            fprintf(stderr, "Error near line %d: expecting 'capture' or 'no capture'\n", getLineNumber());
            return -1;
        }

        if (search_word("multiple jumps") != -1) {
            multiple_jumps_on = 1;
        } else if (search_word("single jumps") != -1) {
            multiple_jumps_on = 0;
        } else {
            fprintf(stderr, "Error: expecting 'multiple jumps' or 'single jumps'\n");
            return -1;
        }
    }

    if ((index = search_word("TURN:")) == -1) {
        fprintf(stderr, "Error near line %d: expecting 'TURN:'\n", getLineNumber());
        return -1;
    } else {
        if (search_word("red") != -1) {
            turn_red = 1;
        } else if (search_word("black") != -1) {
            turn_red = 0;
        } else {
            fprintf(stderr, "Error: expecting 'red' or 'black' after 'TURN:'\n");
        }
    }

    if (search_word("BOARD:") == -1) {
        fprintf(stderr, "Error near line %d: expecting 'BOARD:'\n", getLineNumber());
        return -1;
    }
    if (scan_in_array(ROWS, COLS, board) == -1) {
        fprintf(stderr, "Error: issue with scanning the board\n");
        return -1;
    }

    board_flipped = check_board(ROWS, COLS, board);
    if (board_flipped == -1) {
        fprintf(stderr, "Error: Board was invalid\n");
        return -1;
    }

    if ((index = search_word("MOVES:")) == -1) {
        fprintf(stderr, "Error near line %d: expecting 'MOVES:'\n", getLineNumber());
        return -1;
    } else {
        // Moves will likely be used more in future parts I assume, so this should be ready for future
        /* PRINT POINTS
        move head;
        head = scan_moves(input_file, index);
        while (head->next != NULL) {
            while (head->point->next != NULL) {
                printf("%d: %d, %d\n", head->length, head->point->r, head->point->c);
                head->point = head->point->next;
            }
            head = head->next;
        }*/
    }

    array_count(ROWS, COLS, board, &red_kings, &red_pawns, &black_kings, &black_pawns);

    fprintf(stdout, "VALID INPUT\n");
    fprintf(stdout, "Initial configuration\n");
    fprintf(stdout, "Turn: %s\n", turn_red ? "red" : "black");
    fprintf(stdout, "Red: %d kings, %d pawns\n", red_kings, red_pawns);
    fprintf(stdout, "Black: %d kings, %d pawns\n", black_kings, black_pawns);
    //if (board_flipped) fprintf(stdout, "Flipped board");
    //print_array(ROWS, COLS, board);
}

void initialize_array(int rows, int cols, char array[rows][cols]) {
    int r, c;
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            array[r][c] = 'x';
        }
    }
}

int scan_in_array(int rows, int cols, char array[rows][cols]) {
    char input = fgetc(stdin);
    int r = 0;
    int c = 0;
    for (r = 0; r < rows && input != EOF; r++) {
        for (c = 0; c < cols && input != EOF;) {
            if (input == '\"' || input == 'b' || input == 'B' || input == 'r' || input == 'R'|| input == '.') {
                array[r][c] = input;
                c++;
            }
            input = fgetc(stdin);
        }
    }
    if (c != cols) {
        return -1;
    }
    return 0;
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

int search_word(char *word) {
    int found = 0;
    char ch = fgetc(stdin);
    int length = strlen(word);
    int index = 0;
    int comment = 0;
    while (!found && ch != EOF) {
        index = ftell(stdin);
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
                ch = fgetc(stdin);
            }
        }
        fseek(stdin, index, SEEK_SET);
        ch = fgetc(stdin);
    }
    if (ch == EOF || !found) {
        fseek(stdin, 0, SEEK_SET);
        return -1;
    }
    fseek(stdin, index-1+length, SEEK_SET);
    return index-1;
    //return 0;
}

move scan_moves(int start) {
    move head = malloc(sizeof(struct move_node));
    head->length = 0;
    head->point = NULL;
    head->next = NULL;
    move current = head;

    int index = start;
    char ch = fgetc(stdin);
    while (ch != EOF) {
        if ((index = search_word("->")) == -1) break;
        int count = 0;
        char letter;
        int number;
        ch = fgetc(stdin);
        point currentpt = malloc(sizeof(struct point_node));
        current->point = currentpt;
        for (int i = 0; ch != ' ' && ch != EOF && ch != '\n' && ch != '\t'; index+=4) {
            fseek(stdin, index-2+i, SEEK_SET);
            fscanf(stdin, "%c%d", &letter, &number);
            ch = fgetc(stdin);
            count++;
            currentpt->r = number;
            currentpt->c = letter - 'a';
            point nextpt = malloc(sizeof(struct point_node));
            currentpt->next = nextpt;
            currentpt = currentpt->next;
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

void print_array(int rows, int cols, char array[rows][cols]) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            fprintf(stdout, " %c ", array[r][c]);
            if (c < cols - 1) {
                fprintf(stdout, "|");
            }
        }
        if (r < rows - 1) {
            fprintf(stdout, "\n---+---+---+---+---+---+---+---\n");
        }
    }
    fprintf(stdout, "\n");
}

int getLineNumber() {
    int start_index = ftell(stdin);
    fseek(stdin, 0, SEEK_SET);
    int count = 0;
    int i;
    for (i = 0; ftell(stdin) <= start_index; i++) {
        if (fgetc(stdin) == '\n') count++;
    }
    return count;
}

/* OUTPUT
 * VALID INPUT
 * Initial configuration:
 * Turn: red
 * Red: 0 kings, 12 pawns
 * Black: 0 kings, 12 pawns
 */
