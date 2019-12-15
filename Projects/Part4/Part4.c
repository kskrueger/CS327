//
// Created by Karter Krueger on 12/7/19.
//

#include <termbox.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Checkers.h"

int printStringLeft(char *word, int x, int y, int fgColor, int bgColor);

int printStringRight(char *word, int x, int y, int fgColor, int bgColor);

void waitKey(int key);

void colorRange(int startX, int endX, int startY, int endY, int bgColor);

void init_screen();

void moving_cursor();

void moveBoard(int n, char boardIn[8][8]);

move *getMove(int num);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Arguments must include a filepath!\n");
        exit(1);
    }
    read_args(argc, argv);
    scan_input();
    int count_of_moves = count_moves();
    if (n_moves == -1 || n_moves > count_of_moves) n_moves = count_of_moves;

    //waitKey(TB_KEY_ESC);

    init_screen();

    moving_cursor();

    tb_shutdown();
    printf("End\n");
    return 0;
}

int printStringLeft(char *word, int x, int y, int fgColor, int bgColor) {
    struct tb_cell cell;
    cell.fg = fgColor;
    cell.bg = bgColor;

    int len = strlen(word);
    if (x < 0 || x + len > tb_width() || y < 0 || y > tb_height()) {
        tb_shutdown();
        fprintf(stderr, "Window size not big enough\n");
        exit(1);
    }
    for (int i = 0; i < len; i++) {
        cell.ch = word[i];
        tb_put_cell(x++, y, &cell);
    }
    tb_present();
    return x;
}

int printStringRight(char *word, int x, int y, int fgColor, int bgColor) {
    struct tb_cell cell;
    cell.fg = fgColor;
    cell.bg = bgColor;

    int len = strlen(word);
    if (x - len < 0 || x > tb_width() || y < 0 || y > tb_height()) {
        tb_shutdown();
        fprintf(stderr, "Window size not big enough\n");
        exit(1);
    }
    for (int i = len - 1; i >= 0; i--) {
        cell.ch = word[i];
        tb_put_cell(x--, y, &cell);
    }
    tb_present();
    return x;
}

void waitKey(int key) {
    while (1) {
        struct tb_event event;
        tb_poll_event(&event);
        if (key == event.key) break;
    }
}

void colorRange(int startX, int endX, int startY, int endY, int bgColor) {
    struct tb_cell cell;
    cell.bg = bgColor;
    cell.fg = TB_DEFAULT;
    cell.ch = ' ';
    for (int x = startX; x < endX; x++) {
        for (int y = startY; y < endY; y++) {
            tb_put_cell(x, y, &cell);
        }
    }
    tb_present();
}

void plot_board(char **array) {
    int midX = tb_width() / 2;
    int gridX = (tb_width() - midX) / 8;
    int gridY = tb_height() / 8;
    int color;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2) {
                if (tolower(array[i][j]) == 'b' || tolower(array[i][j]) == 'r') {
                    color = (tolower(array[i][j]) == 'r') ? TB_RED : TB_BLACK;
                    printStringLeft(isupper(array[i][j]) ? "@" : "O", midX + j * gridX + gridX / 2,
                                    1 + i * gridY + gridY / 2, color, TB_WHITE);
                } else {
                    printStringLeft(" ", midX + j * gridX + gridX / 2,
                                    1 + i * gridY + gridY / 2, TB_DEFAULT, TB_WHITE);
                }
            }
        }
    }
    tb_present();
}

int maxLine;
int maxMoves = 0;

void list_moves(int start_move, int end_move) {
    int midX = tb_width() / 2;
    int line = 7;
    int left = 1;

    char **myBoard = malloc(sizeof(char *) * 8);
    for (int i = 0; i < 8; i++) myBoard[i] = malloc(sizeof(char) * 8);
    copy_board_ref(board, myBoard);

    if (start_move > 0) {
        printStringLeft(":     ", 0, 6, TB_DEFAULT, TB_DEFAULT);
        printStringRight(":     ", midX - 2, 6, TB_DEFAULT, TB_DEFAULT);
    } else {
        printStringLeft("------", 0, 6, TB_DEFAULT, TB_DEFAULT);
        printStringRight("------", midX - 2, 6, TB_DEFAULT, TB_DEFAULT);
    }
    if (end_move < maxMoves - 2) {
        printStringLeft(":     ", 0, tb_height() - 1, TB_DEFAULT, TB_DEFAULT);
        printStringRight(":     ", midX - 2, tb_height() - 1, TB_DEFAULT, TB_DEFAULT);
    } else {
        printStringLeft("------", 0, tb_height() - 1, TB_DEFAULT, TB_DEFAULT);
        printStringRight("------", midX - 2, tb_height() - 1, TB_DEFAULT, TB_DEFAULT);
    }

    int error = 0;
    int move_count = 0;
    turn_red = 0;
    for (move current = moves; current->next->next != NULL; current = current->next) {
        for (point currPoint = current->point; currPoint->next->next != NULL; currPoint = currPoint->next) {
            int r = currPoint->r;
            int c = currPoint->c;
            int jump_r = currPoint->next->r;
            int jump_c = currPoint->next->c;

            char *str = malloc(sizeof(char) * 6);
            sprintf(str, "%c%d->%c%d", currPoint->c + 'a', currPoint->r, currPoint->next->c + 'a', currPoint->next->r);

            int result = move_noerror(r, c, jump_r, jump_c, myBoard);
            int color = TB_GREEN;
            if (error) color = TB_WHITE;
            else if (result == -1) {
                color = TB_RED;
                error = 1;
            }

            //char *temp = malloc(sizeof(char) * 30);
            //sprintf(temp, "mc:%d sm:%d ln:%d mx:%d", move_count, start_move, line, maxLine);
            //printStringLeft(temp, 10, 4, TB_DEFAULT, TB_DEFAULT);
            if (move_count >= start_move && line < maxLine) {
                if (left) {
                    printStringLeft(str, 0, line, color, TB_DEFAULT);
                } else {
                    printStringRight(str, midX - 2, line, color, TB_DEFAULT);
                    line++;
                }
                left = !left;
            }
        }
        move_count++;
    }
}

int start_move = 0;
int end_move = 0;

void init_screen() {
    tb_init();
    maxMoves = count_moves();

    int midX = tb_width() / 2;
    colorRange(0, tb_width(), 0, 1, TB_CYAN);
    printStringLeft(filename, 0, 0, TB_BLACK, TB_CYAN);
    printStringLeft("up key, down key, left & right move edit cursor", 0, 1, TB_DEFAULT, TB_DEFAULT);
    printStringLeft("Type a-g and 0-8 to edit move with cursor", 0, 2, TB_DEFAULT, TB_DEFAULT);
    printStringLeft("ESC: quit, s: save", 0, 3, TB_DEFAULT, TB_DEFAULT);

    printStringLeft("Black", 0, 5, TB_DEFAULT, TB_DEFAULT);
    printStringRight("Red", midX - 2, 5, TB_DEFAULT, TB_DEFAULT);
    printStringLeft("------", 0, 6, TB_DEFAULT, TB_DEFAULT);
    printStringRight("------", midX - 2, 6, TB_DEFAULT, TB_DEFAULT);

    int gridX = (tb_width() - midX) / 8;
    int gridY = tb_height() / 8;
    int color;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            color = ((i + j) % 2) ? TB_WHITE : TB_RED;
            colorRange(midX + j * gridX, midX + (j + 1) * gridX, 1 + i * gridY, 1 + (i + 1) * gridY, color);
        }
    }

    // init move nums
    start_move = 0;
    maxLine = count_moves() / 2 + 7;
    if (maxLine > tb_height()) maxLine = tb_height() - 1;
    end_move = (maxLine - 7) * 2;

    char **startBoard = malloc(sizeof(char *) * 8);
    for (int i = 0; i < 8; i++) startBoard[i] = malloc(sizeof(char) * 8);
    copy_board_ref(board, startBoard);
    plot_board(startBoard);
    list_moves(start_move, end_move);
    tb_present();
}

void moving_cursor() {
    int midX = tb_width() / 2;
    int line = 7;
    int side = 0;
    int xPos = 0;
    int num = 1;

    tb_set_cursor(0, line);
    tb_present();

    while (1) {
        struct tb_event event;
        tb_poll_event(&event);

        if (TB_KEY_ESC == event.key) break;
        else if (TB_KEY_ARROW_UP == event.key) {
            if (line > 7 || side == 1) {
                num--;
                if (side) {
                    side = 0;
                } else {
                    line--;
                    side = 1;
                }
            } else if (line == 7 && start_move >= 2) {
                start_move -= 2;
                end_move -= 2;
                num -= 2;
            }
            list_moves(start_move, end_move);
        } else if (TB_KEY_ARROW_DOWN == event.key) {
            if (line < maxLine) {
                num++;
                if (!side) {
                    side = 1;
                } else {
                    line++;
                    side = 0;
                }
            } else if (line == maxLine && end_move < maxMoves) {
                start_move += 2;
                end_move += 2;
                num += 2;
            }
            list_moves(start_move, end_move);
        } else if (TB_KEY_ARROW_LEFT == event.key) {
            if (xPos == 4) xPos -= 3;
            else if (side && xPos == 0) {
                num--;
                side = 0;
                xPos = 0;
            } else if (xPos > 0) xPos--;
        } else if (TB_KEY_ARROW_RIGHT == event.key) {
            if (xPos == 1) xPos += 3;
            else if (!side && xPos == 5) {
                num++;
                side = 1;
                xPos = 0;
            } else if (xPos < 5) xPos++;
        } else if ((xPos == 0 || xPos == 4) && event.ch >= 'a' && event.ch <= 'h') {
            if (xPos == 0) (*getMove(num))->point->c = event.ch - 'a';
            else (*getMove(num))->point->next->c = event.ch - 'a';
            list_moves(start_move, end_move);
            colorRange(strlen(filename) + 5, tb_width(), 0, 1, TB_CYAN);
            printStringLeft("(Edited... press s to save)", strlen(filename) + 5, 0, TB_BLACK, TB_CYAN);
        } else if ((xPos == 1 || xPos == 5) && event.ch >= '0' && event.ch <= '8') {
            if (xPos == 1) (*getMove(num))->point->r = event.ch - '0';
            else (*getMove(num))->point->next->r = event.ch - '0';
            list_moves(start_move, end_move);
            colorRange(strlen(filename) + 5, tb_width(), 0, 1, TB_CYAN);
            printStringLeft("(Edited... press s to save)", strlen(filename) + 5, 0, TB_BLACK, TB_CYAN);
        } else if (event.ch == 's') {
            writeFile(filename);
            colorRange(strlen(filename) + 5, tb_width(), 0, 1, TB_CYAN);
            printStringLeft("(file saved)", strlen(filename) + 5, 0, TB_BLACK, TB_CYAN);
        }
        tb_set_cursor(xPos + side * (midX - 7), line);
        moveBoard(num, board);

        tb_present();
    }
}

void moveBoard(int n, char boardIn[8][8]) {
    char *str = malloc(sizeof(char) * 3);
    sprintf(str, "%d  ", n);
    printStringLeft(str, 1, 4, TB_DEFAULT, TB_DEFAULT);
    char **myBoard = malloc(sizeof(char *) * 8);
    for (int i = 0; i < 8; i++) myBoard[i] = malloc(sizeof(char) * 8);
    copy_board_ref(boardIn, myBoard);

    move curr = moves;
    while (curr->next->next != NULL && n-- > 0) {
        move_noerror(curr->point->r, curr->point->c, curr->point->next->r, curr->point->next->c, myBoard);
        curr = curr->next;
    }
    plot_board(myBoard);
}

move *getMove(int num) {
    if (num == 1) return &moves;
    int move_count = 0;
    move curr = moves;
    while (curr->next->next != NULL) {
        move_count++;
        if (move_count >= num - 1) break;
        curr = curr->next;
    }
    return &curr->next;
}
