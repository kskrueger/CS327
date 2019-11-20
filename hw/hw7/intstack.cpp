//
// Created by Karter Krueger on 11/19/19.
//

#include <cstdlib>
#include <cctype>
#include <cstdio>

class intstack {
private:
    struct node {
        int value;
        struct node *next;
    };
    struct node *head;

public:
    intstack() {
        head = (node *)(malloc(sizeof(struct node)));
        head->next = nullptr;
    };
    ~intstack() {
        while (bool()) {
            pop();
        }
        free(head);
    };

    void push(int val);
    int pop();

    explicit operator bool() {
        node* curr = head;
        while (curr->next != nullptr) curr = curr->next;
        return curr != head;
    };
};

void intstack::push(int val) {
    node* curr = head;
    while (curr->next != nullptr) curr = curr->next;
    curr->next = (node*) malloc(sizeof(node));
    curr->next->value = val;
    curr->next->next = nullptr;
}

int intstack::pop() {
    node* curr = head;
    node* prev = head;
    while (curr->next != nullptr) {
        prev = curr;
        curr = curr->next;
    }
    int num = curr->value;
    if (curr != head) free(curr);
    prev->next = nullptr;
    return num;
}

int main(int argc, char* argv[]) {
    intstack numStack;
    for (int i = 1; i < argc; i++) {
        if (isdigit(argv[i][0]) || argv[i][0] == '-') {
            int d = std::atoi(argv[i]);
            numStack.push(d);
        } else {
            int b = numStack.pop();
            int a = numStack.pop();
            switch (argv[i][argv[i][0] == '\\' ? 1 : 0]) {
                case '-':
                    numStack.push(a - b);
                    break;
                case '+':
                    numStack.push(a + b);
                    break;
                case '*':
                    numStack.push(a * b);
                    break;
                case '/':
                    numStack.push(a / b);
                    break;
                case '%':
                    numStack.push(a % b);
                    break;
            }
        }
    }
    int final = numStack.pop();
    printf("%d\n", final);
}
