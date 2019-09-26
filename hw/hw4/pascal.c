//
// Created by Karter Krueger on 9/26/19.
//
#include <stdio.h>
#include <stdlib.h>

unsigned long** build_Pascal(unsigned N);
void show_Pascal(unsigned long** P, unsigned N);
void destroy_Pascal(unsigned long** P, unsigned N);

/* OUTPUT
 * output should be organized into columns, so that the width of each column is just wide enough to display the largest integer in that column
 */

int main() {
    unsigned n = 25;
    unsigned long** triangle = build_Pascal(n);
    show_Pascal(triangle, n);
    destroy_Pascal(triangle, n);
}

// function to build the first N rows of Pascal's triangle, where N is given as a parameter.
unsigned long** build_Pascal(unsigned N) {
    unsigned long** out = malloc(N * sizeof(unsigned long*));
    unsigned i, j;
    for (i = 0; i < N; i++) {
        out[i] = malloc((i + 1) * sizeof(unsigned long));
        for (j = 0; j <= i; j++) {
            if (j == 0 || j == i || i < 1) out[i][j] = 1;
            else out[i][j] = out[i-1][j-1] + out[i-1][j];
        }
    }
    return out;
}

// function to display the first N rows of Pascal's triangle, where N and the triangle are given as parameters
void show_Pascal(unsigned long** P, unsigned N) {
    unsigned w, i, j;
    int *widths = calloc(N, sizeof(int));
    for (w = 0; w < N; w++) {
        unsigned long num = P[N-1][w];
        while ((num /= 10) > 9) widths[w]++;
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j <= i; j++) {
            fprintf(stdout, "%*lu ", widths[j]+2, P[i][j]);
        }
        fprintf(stdout, "\n");
    }
}

// function to destroy (free the allocated memory for) the first N rows of Pascal's triangle
void destroy_Pascal(unsigned long** P, unsigned N) {
    unsigned i = 0;
    while (i < N) free(P[i++]);
    free(P);
    *P = NULL;
}
