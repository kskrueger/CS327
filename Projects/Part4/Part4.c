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

int main(int argc, char *argv[]) {
    read_args(argc, argv);
    scan_input();
    int count_of_moves = count_moves();
    if (n_moves == -1 || n_moves > count_of_moves) n_moves = count_of_moves;

    init_screen();

    moving_cursor();

    //waitKey(TB_KEY_ESC);

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
        fprintf(stderr, "Window size not big enough");
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
        fprintf(stderr, "Window size not big enough");
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

void plot_board(char** array) {
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
void list_moves() {
    int midX = tb_width() / 2;
    int line = 7;
    int left = 1;

    char **myBoard = malloc(sizeof(char*)*8);
    for (int i = 0; i < 8; i++) myBoard[i] = malloc(sizeof(char)*8);
    copy_board_ref(board, myBoard);

    move current = moves;
    int error = 0;
    int m, p;
    for (m = 0; current->next->next != NULL; m++) {
        point currPoint = current->point;
        for (p = 0; currPoint->next->next != NULL && moves_made < n_moves; p++) {
            int r = currPoint->r;
            int c = currPoint->c;
            int jump_r = currPoint->next->r;
            int jump_c = currPoint->next->c;

            char *str = malloc(sizeof(char) * 6);
            sprintf(str, "%c%d->%c%d", currPoint->c + 'a', currPoint->r, currPoint->next->c + 'a', currPoint->next->r);

            int color = TB_GREEN;
            if (error) color = TB_WHITE;
            else if (move_noerror(r, c, jump_r, jump_c, myBoard) == -1) {
                color = TB_RED;
                error = 1;
            }

            if (left) {
                printStringLeft(str, 0, line, color, TB_DEFAULT);
            } else {
                printStringRight(str, midX - 2, line, color, TB_DEFAULT);
                line++;
            }
            left = !left;

            currPoint = currPoint->next;
        }
        current = current->next;
    }
    maxLine = line;
}

void init_screen() {
    tb_init();

    int midX = tb_width() / 2;
    colorRange(0, tb_width(), 0, 1, TB_CYAN);
    printStringLeft(filename, 0, 0, TB_BLACK, TB_CYAN);
    printStringLeft("up key, down key: move cursor", 0, 1, TB_DEFAULT, TB_DEFAULT);
    printStringLeft("ESC: quit", 0, 2, TB_DEFAULT, TB_DEFAULT);

    printStringLeft("Black", 0, 5, TB_DEFAULT, TB_DEFAULT);
    printStringRight("Red", midX - 2, 5, TB_DEFAULT, TB_DEFAULT);
    printStringLeft(":", 0, 6, TB_DEFAULT, TB_DEFAULT);
    printStringRight(":", midX - 2, 6, TB_DEFAULT, TB_DEFAULT);

    int gridX = (tb_width() - midX) / 8;
    int gridY = tb_height() / 8;
    int color;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            color = ((i + j) % 2) ? TB_WHITE : TB_RED;
            colorRange(midX + j * gridX, midX + (j + 1) * gridX, 1 + i * gridY, 1 + (i + 1) * gridY, color);
        }
    }

    char **startBoard = malloc(sizeof(char*)*8);
    for (int i = 0; i < 8; i++) startBoard[i] = malloc(sizeof(char)*8);
    copy_board_ref(board, startBoard);
    plot_board(startBoard);
    list_moves();
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
            if (line>7 || side == 1) {
                num--;
                if (side) {
                    side = 0;
                } else {
                    line--;
                    side = 1;
                }
            }
        } else if (TB_KEY_ARROW_DOWN == event.key) {
            if (line<maxLine) {
                num++;
                if (!side) {
                    side = 1;
                } else {
                    line++;
                    side = 0;
                }
            }
        } else if (TB_KEY_ARROW_LEFT == event.key) {
            if (xPos == 4) xPos -= 3;
            else if (side && xPos == 0) {
                side = 0;
                xPos = 0;
            }
            else if (xPos > 0) xPos--;
        } else if (TB_KEY_ARROW_RIGHT == event.key) {
            if (xPos == 1) xPos += 3;
            else if (!side && xPos == 5) {
                side = 1;
                xPos = 0;
            }
            else if (xPos < 5) xPos++;
        }
        tb_set_cursor(xPos+side*(midX-7), line);
        moveBoard(num, board);

        tb_present();
    }
}

void moveBoard(int n, char boardIn[8][8]) {
    char **myBoard = malloc(sizeof(char*)*8);
    for (int i = 0; i < 8; i++) myBoard[i] = malloc(sizeof(char)*8);
    copy_board_ref(boardIn, myBoard);
    //move_m(myBoard, nextMove);

    colorRange(0, tb_width(), 65, tb_height(), TB_DEFAULT);
    int num = n;
    /*move curr = moves;
    int j;
    int x = 0;
    int y = 65;
    while (curr->next->next != NULL && num-- > 0) {
        point currPoint = curr->point;
        while (currPoint->next != NULL) {
            //fprintf(file, "%c%d", currPoint->c+'a', currPoint->r);
            char* str = malloc(sizeof(char) * 3);
            sprintf(str, "%c%d", currPoint->c + 'a', currPoint->r);
            if (currPoint->next) {
                move_noerror(currPoint->r, currPoint->c, currPoint->next->r, currPoint->next->c, myBoard);
            }
            if (x+2 >= tb_width()) {
                y++;
                x = 0;
            }
            printStringLeft(str, x, y, TB_DEFAULT, TB_DEFAULT);
            x+=2;
            currPoint = currPoint->next;
            if (currPoint->next != NULL) {
                //fprintf(file, "->");
                printStringLeft("->", x, y, TB_DEFAULT, TB_DEFAULT);
                x+=2;
            }
        }
        curr = curr->next;
        printStringLeft(" ", x, y, TB_DEFAULT, TB_DEFAULT);
        x++;
    }*/

    move curr = moves;
    while (curr->next->next != NULL && n-- > 0) {
        move_noerror(curr->point->r, curr->point->c, curr->point->next->r, curr->point->next->c, myBoard);
        /*point currPoint = curr->point;
        while (currPoint->next != NULL) {
            currPoint = currPoint->next;
        }*/
        curr = curr->next;
    }
    plot_board(myBoard);
}
