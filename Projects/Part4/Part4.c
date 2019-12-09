//
// Created by Karter Krueger on 12/7/19.
//

#include <termbox.h>
#include <stdio.h>

int main() {
    /* Switch out of normal stdio mode */
    tb_init();

    struct tb_cell cell;
    cell.fg = TB_YELLOW;
    cell.bg = TB_RED;
    cell.ch = 'H';
/*
  Great news!  Turns out we don't need to use
    tb_utf8_char_to_unicode()
  after all (see next line, commented out)
*/
/*
  tb_utf8_char_to_unicode(&cell.ch, "H");
*/

    tb_put_cell(tb_width() / 2, tb_height() / 2, &cell);

    int cx, cy;

    cx = cy = 0;

    for (;;) {
        tb_set_cursor(cx, cy);

        tb_present();

        /*
          Grab any event
        */
        struct tb_event event;
        tb_poll_event(&event);

        if (TB_KEY_ESC == event.key) break;
        if (TB_KEY_DELETE == event.key) {
            cx--;
            continue;
        }
        if (TB_KEY_ARROW_DOWN == event.key) {
            cy++;
            continue;
        }
        if (TB_KEY_ARROW_UP == event.key) {
            cy--;
            continue;
        }
        if (TB_KEY_ARROW_RIGHT == event.key) {
            cx++;
            continue;
        }
        if (TB_KEY_ARROW_LEFT == event.key) {
            cx--;
            continue;
        }

        cell.ch = event.ch;
        tb_put_cell(cx++, cy, &cell);
    }

    tb_shutdown();

    printf("Stuff\n");

    return 0;
}


