#include<stdio.h>
#include <math.h>
#define DEBUG 0

void initialize(char digits[], unsigned numdigits, unsigned value);
int add(char result[], char num1[], char num2[], unsigned numdigits);
void print_int(char digits[], unsigned numdigits);

int main() {
	int size = 50;
	char n1[size];
	char n2[size];
	char n3[size];
	initialize(n1, size, 0);
    initialize(n2, size, 1);
    printf("F(   0): %d\n", 0);
    printf("F(   1): %d\n", 1);
    int i = 2;
    while (1) {
        if (add(n3, n1, n2, size)) break;
        printf("F(%4d): ", i++);
        print_int(n3, size);
        if (add(n1, n2, n3, size)) break;
        printf("F(%4d): ", i++);
        print_int(n1, size);
        if (add(n2, n3, n1, size)) break;
        printf("F(%4d): ", i++);
        print_int(n2, size);
    }
	return 0;
}

void initialize(char digits[], unsigned numdigits, unsigned value) {
    int i;
    for (i = 0; i < numdigits; i++) {
        digits[i] = '0' + (int) (((int)value/ (int)pow(10, (numdigits-1-i)))%10);
    }
}

int add(char result[], char num1[], char num2[], unsigned numdigits) {
	int i;
	int carry = 0;
	// printf("num1: %s, num2: %s\n", num1, num2);
	for(i = numdigits - 1; i >= 0; i--) {
		int number = num1[i] - '0' + num2[i] - '0' + carry;
		// printf("%d + %d + %d = %d\n", num1[i] - '0', num2[i] - '0', carry, number);
		if (number > 9) {
			carry = 1;
			number -= 10;
			// printf("Carry, num = :%d\n", number);
		} else {
            carry = 0;
        }
		result[i] = (number + '0');
		if (i == 0 && carry == 1) {
            return 1;
		}
	}
	return 0;
}

void print_int(char digits[], unsigned numdigits) {
	int i;
	int zeros = 1;
	for (i = 0; i < numdigits; i++) {
	    if (digits[i] != '0') zeros = 0;
	    if (!zeros) {
            printf("%c", digits[i]);
            if ((numdigits-1-i) % 3 == 0 && i != numdigits-1) {
                printf(",");
            }
	    }
	}
	printf("\n");
}
 
