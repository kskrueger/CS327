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
    unsigned n = 0;
    fprintf(stdout, "Enter a size N for Pascal's triangle: \n");
    fscanf(stdin, "%d", &n);
    unsigned long** triangle = build_Pascal(n);
    show_Pascal(triangle, n);
    destroy_Pascal(triangle, n);
}

// function to build the first N rows of Pascal's triangle, where N is given as a parameter.
unsigned long** build_Pascal(unsigned N) {
    // initialize out with a chunk of memory for pointers to arrays of unsigned long
    unsigned long** out = malloc(N * sizeof(unsigned long*));
    unsigned i, j; // counters for loops
    for (i = 0; i < N; i++) { // loop through for each line of the pascal triangle
        out[i] = malloc((i + 1) * sizeof(unsigned long)); // reserve memory for each row of the triangle
        for (j = 0; j <= i; j++) { // fill each value in each row
            if (j == 0 || j == i || i < 1) out[i][j] = 1; // if it's the first row, or start/end column, set to 1
            else out[i][j] = out[i-1][j-1] + out[i-1][j]; // otherwise, set the value to the sum of those above
        }
    }
    return out; // return the pointer (that points to an array of pointers)
}

// function to display the first N rows of Pascal's triangle, where N and the triangle are given as parameters
void show_Pascal(unsigned long** P, unsigned N) {
    unsigned w, i, j; // counters for loops
    int *widths = calloc(N, sizeof(int)); // initialize an array of widths (of the columns) to all 0s in a size of N
    for (w = 0; w < N; w++) { // for each column
        unsigned long num = P[N-1][w]; // get the largest value, from the last row of the triangle
        while ((num /= 10) > 9) widths[w]++; // determine the number of digits in the numbers, and increment in widths
    }
    for (i = 0; i < N; i++) { // loop for each row/line of triangle
        for (j = 0; j <= i; j++) { // loop for each value in each row
            fprintf(stdout, "%*lu ", widths[j]+2, P[i][j]); // print each number, with the correct width
        }
        fprintf(stdout, "\n"); // new line after each row
    }
}

// function to destroy (free the allocated memory for) the first N rows of Pascal's triangle
void destroy_Pascal(unsigned long** P, unsigned N) {
    unsigned i = 0; // loop counter
    while (i < N) free(P[i++]); // free each row pointer
}
