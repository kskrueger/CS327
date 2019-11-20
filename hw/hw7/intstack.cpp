//
// Created by Karter Krueger on 11/19/19.
//

#include <cstdlib>
#include <cctype>
#include <cstdio>

/* --- Instructions ---
 * Compile as normal and input the postfix with spaces between all numbers and operators
 * for example, "./intstack 3 4 \* 5 % -6 7 \* +"
 * will return "-40"
 * */

class intstack {
private:
    struct node {
        int value;
        struct node *next;
    };
    struct node *head;

public:
    intstack() {
        // initialize head node
        head = (node *)(malloc(sizeof(struct node)));
        head->next = nullptr;
    };
    ~intstack() {
        // pop out all values (frees them)
        while (bool()) pop();
        // free head
        free(head);
    };

    void push(int val);
    int pop();

    explicit operator bool() {
        // loop through list and return if last node is head
        node* curr = head;
        while (curr->next != nullptr) curr = curr->next;
        return curr != head;
    };
};

void intstack::push(int val) {
    node* curr = head;
    // loop until end of list
    while (curr->next != nullptr) curr = curr->next;
    // make new node and initialize (next = null)
    curr->next = (node*) malloc(sizeof(node));
    curr->next->value = val;
    curr->next->next = nullptr;
}

int intstack::pop() {
    node* curr = head;
    node* prev = head;
    // loop until end of list
    while (curr->next != nullptr) {
        prev = curr;
        curr = curr->next;
    }
    // get value of last node
    int num = curr->value;
    // free the old node
    if (curr != head) free(curr);
    prev->next = nullptr;
    return num;
}

int main(int argc, char* argv[]) {
    // make an intstack for numbers to be stored in
    intstack numStack;
    // loop through all argument inputs
    for (int i = 1; i < argc; i++) {
        // push numbers onto the numStack (negative too)
        if (isdigit(argv[i][0]) || argv[i][0] == '-') {
            int d = std::atoi(argv[i]);
            numStack.push(d);
        } else {
            // pop two numbers off the stack (right then left)
            int b = numStack.pop();
            int a = numStack.pop();
            // switch for operators and perform calculation, push back onto stack
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
    // pop final answer off the stack and print
    int final = numStack.pop();
    printf("%d\n", final);
}
