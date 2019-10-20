//
// Created by Karter Krueger on 9/10/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Part1.c"

int main(int argc, char* argv[]) {
    /* loop through arguments
     *  for each "-e", check for e-filename after (doesn't start with '-')
     *  for each "-h", check for h-filename
     *  "-m" followed by N number of moves should be played
     */

    int n = -1;
    char *filename = NULL;
    for (int i = 1; i < argc && argv[i] != NULL; i++) {
        if (n == -1 && !strcmp(argv[i], "-m")) {
            if (++i >= argc || !sscanf(argv[i], "%d", &n)) {
                fprintf(stderr, "Argument error: no value for '-n' flag");
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
    if (n != -1) fprintf(stdout, "%d\n", n);
    if (filename != NULL) {
        fprintf(stdout, "Input filename: %s\n", filename);
        freopen(filename, "r", stdin);
    }
    scan_input();

    //fprintf(stdout, "c, r: %d %d\n", (*moves)->point->c, (*moves)->point->r);

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-e")) {

        } else if (!strcmp(argv[i], "-h")) {

        }
    }

    return 0;
}
