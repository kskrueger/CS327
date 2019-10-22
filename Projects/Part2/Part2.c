//
// Created by Karter Krueger on 9/10/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Part1.c"

void read_args(int argc, char* argv[]);

int n = -1;
char *filename = NULL;

int main(int argc, char* argv[]) {
    read_args(argc, argv);
    scan_input();

    printf("Point %d, %d\n", moves->point->r, moves->point->c);

    move* backup = &moves;
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

    move move1 = *backup;
    printf("Point %d, %d\n", move1->point->r, move1->point->c);

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-e")) {

        } else if (!strcmp(argv[i], "-h")) {

        }
    }

    return 0;
}

/* loop through arguments
*  for each "-e", check for e-filename after (doesn't start with '-')
*  for each "-h", check for h-filename
*  "-m" followed by N number of moves should be played
*/
void read_args(int argc, char* argv[]) {
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
    if (n != -1) fprintf(stdout, "Number: %d\n", n);
    if (filename != NULL) {
    fprintf(stdout, "Input filename: %s\n", filename);
    freopen(filename, "r", stdin);
    }
}
