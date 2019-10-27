//
// Created by Karter Krueger on 10/26/19.
//
#include <stdio.h>
#include <stdlib.h>

struct node {
    int num;
    struct node* next;
};

void error(char* error_text);
struct node* v_nodes[9] = { NULL };
struct node** n_nodes;
int line_num = 1;

int main() {
    int n, i;
    fscanf(stdin, "%d", &n); // scan the number of nodes to create
    // create a list of nodes to size, initialize each one, & error if malloc fail
    if (!(n_nodes = malloc(n * sizeof(struct node*)))) error("malloc fail");
    for (i= 0; i < n; i++) {
        if (!(n_nodes[i] = malloc(sizeof(struct node)))) error("malloc fail");
        n_nodes[i]->next = NULL;
        n_nodes[i]->num = i;
    }

    char type;
    int p1, p2;
    // loop and continue to scan in the commands with src and dst nodes
    while (fscanf(stdin, " %c %d->%d", &type, &p1, &p2) == 3) {
        line_num++;
        if (type == 'L') {
            if (p1 < 1 || p1 > n || p2 < 0 || p2 > n) error("src or dest out of bounds");
            n_nodes[p1-1]->next = p2 ? n_nodes[p2-1] : NULL;
        } else if (type == 'V') {
            if (p1 < 1 || p1 > 9 || p2 < 0 || p2 > n) error("src or dest out of bounds");
            if (v_nodes[p1-1] == 0 && !(v_nodes[p1-1] = malloc(sizeof(struct node*)))) error("malloc fail");
            v_nodes[p1-1]->next = p2 ? n_nodes[p2-1] : NULL;
        } else error("invalid input type");
    }

    char n_marked[n];
    for (i= 0; i < n; i++) n_marked[i] = 0;

    int j=0, k=0;
    // scan through variable nodes (0-9) to mark all nodes that are referenced
    for (i = 0; i < 9; i++) {
        struct node* temp = v_nodes[i];
        if (!temp) break;
        temp = temp->next;
        while (temp && !n_marked[temp->num]) {
            n_marked[temp->num] = 1;
            temp = temp->next;
            j++;
        }
    }
    fprintf(stdout, "Reachable:" );
    for (i = 0; i < n; i++) {
        if (n_marked[i]) {
            fprintf(stdout, " %d", i+1);
            if (k++ < j-1) fprintf(stdout, ",");
        }
    }
    fprintf(stdout, "\n");
    k = 0;
    fprintf(stdout, "Unreachable:");
    for (i = 0; i < n; i++) {
        if (!n_marked[i]) {
            fprintf(stdout, " %d", i+1);
            if (k++ < n-j-1) fprintf(stdout, ",");
        }
    }
    fprintf(stdout, "%c\n", 8);
    return 0;
}

void error(char* error_text) {
    fprintf(stderr, "ERROR near line %d: %s\n", line_num, error_text);
    exit(1);
}