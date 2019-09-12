#include<stdio.h>
#define DEBUG 0

void initialize(char digits[], unsigned numdigits, unsigned value);
int add(char result[], char num1[], char num2[], unsigned numdigits);
void print_int(char digits[], unsigned numdigits);

int main() {
	int size = 4;
	char result[size];
	char in1[size];
	char in2[size];
	initialize(in1, size, 9998);
	initialize(in2, 1, 1);
	if (add(result, in1, in2, size) != 0) {
	    printf("Overflow!\n");
	}
	print_int(result, size);
	return 0;
}

void initialize(char digits[], unsigned numdigits, unsigned value) {
	sprintf(digits, "%d", value);

}

int add(char result[], char num1[], char num2[], unsigned numdigits) {
	int i;
	int carry = 0;
	printf("num1: %s, num2: %s\n", num1, num2);
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
	for (i = 0; i < numdigits; i++) {
	    printf("%c", digits[i]);
	    if ((numdigits-1-i) % 3 == 0 && i != numdigits-1) {
	        printf(",");
	    }
	}
	printf("\n");
}
 
