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

void init_board();

int main() {
    init_board();

    waitKey(TB_KEY_ESC);

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

void init_board() {
    tb_init();
    //tb_set_cursor(0, 0);

    int midX = tb_width() / 2;
    colorRange(0, tb_width(), 0, 1, TB_CYAN);
    printStringLeft("up, dn, pf up, pf dn: move cursor", 0, 1, TB_DEFAULT, TB_DEFAULT);
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
            colorRange(midX+j*gridX, midX+(j+1)*gridX, 1+i*gridY, 1+(i+1)*gridY, color);
        }
    }

    tb_present();
}
