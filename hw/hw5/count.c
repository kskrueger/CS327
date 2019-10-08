//
// Created by Karter Krueger on 10/6/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct node {
    char word[32];
    unsigned int count;
    struct node* next;
};

struct node* addToList(struct node* Listptr, char* word);

int main() {
    struct node *my_list = NULL;
    char word[32]; // word has max size of 32
    char c;
    int i = 0;
    // scan while characters to read (not EOF)
    while (fscanf(stdin, "%c", &c) == 1) {
        if (isalpha(c)) { // check if character is alpha (letter)
            word[i++] = c; // fill the word
            if (i > 31) { // ERROR if word longer than 32
                fprintf(stderr, "ERROR: word longer than 32\n");
                exit(1);
            }
        } else {
            word[i] = '\0'; // set last character to NULL (end of string)
            if (i > 0) my_list = addToList(my_list, word); // add word to the list
            i = 0;
        }
    }

    // print out the list of count and word pairs
    while (my_list != NULL) {
        fprintf(stdout, "%d %s\n", my_list->count, my_list->word);
        my_list = my_list->next;
    }
    return 0;
}

struct node *addToList(struct node *Listptr, char *word) {
    if (strlen(word) > 32) { // check for size over 32 to be safe
        fprintf(stderr, "ERROR: word longer than 32");
        exit(1);
    }
    if (Listptr == NULL) { // if Listptr is NULL, make a new one
        Listptr = malloc(sizeof(struct node));
        strcpy(Listptr->word, word); // copy the word into the node
        Listptr->count = 1;
        Listptr->next = NULL;
        return Listptr;
    }
    struct node *head = Listptr; // save the head node address
    while (1) { // loop until end of list is reached or word count is incremented
        if (!strcmp(Listptr->word, word)) {
            Listptr->count++;
            return head;
        }
        if (Listptr->next != NULL) Listptr = Listptr->next;
        else break;
    }
    // if it makes it this far, make a new node and set word and count
    Listptr->next = malloc(sizeof(struct node));
    strcpy(Listptr->next->word, word);
    Listptr->next->count = 1;
    Listptr->next->next = NULL;
    return head;
}
