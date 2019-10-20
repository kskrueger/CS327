//
// Created by Karter Krueger on 9/20/19.
//
#import <stdio.h>
#include <stdlib.h>
#include <string.h>

/*struct data1 {
    int number;
    struct data1* next;
};

typedef struct data1 *nodept;

struct data {
    int number;
    nodept point;
    struct data* next;
};

typedef struct data *node;

int main() {
    node head = malloc(sizeof(struct data));
    head->number = -1;
    head->point = NULL;
    head->next = NULL;

    node current = head;
    for (int i = 0; i < 5; i++) {
        current->number = i;
        node next = malloc(sizeof(struct data));

        nodept currentpt = malloc(sizeof(struct data1));
        current->point = currentpt;
        for (int x = 0; x < 3; x++) {
            currentpt->number = x;
            nodept nextpt = malloc(sizeof(struct data1));
            currentpt->next = nextpt;
            currentpt = currentpt->next;
        }
        currentpt->next = NULL;

        current->next = next;
        current = current->next;
    }
    current->next = NULL;

    while (head->next != NULL) {
        while (head->point->next != NULL) {
            printf("i: %d x: %d\n", head->number, head->point->number);
            head->point = head->point->next;
        }
        head = head->next;
    }
}*/

char* combine(char* str1, char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int size = len1 < len2 ? len1 : len2;
    size*=2;
    while (!(size%2)) size--;
    char* out = malloc((size+1) * sizeof(char));
    for (int i = 0; i < size; i++) {
        if (!(i % 2)) {
            out[i] = *(str1++);
        } else {
            out[i] = *(str2++);
        }
    }
    out[size] = 0;
    return out;
}

int main () {
    char* out = combine("HloWrd", "el ol!");
    printf("%s", out);
}