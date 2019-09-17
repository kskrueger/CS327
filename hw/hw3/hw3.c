// HW3 CREATED BY KARTER KRUEGER on 9/10/19
#include<stdio.h>
#include <math.h>

void initialize(char digits[], unsigned numdigits, unsigned value);
int add(char result[], char num1[], char num2[], unsigned numdigits);
void print_int(char digits[], unsigned numdigits);

int main() {
	int size = 50; // number of digits for the large integers
	char n1[size]; // large integer 1
	char n2[size]; // large integer 2
	char n3[size]; // large integer 3
	initialize(n1, size, 0); // load 0 into integer 1
    initialize(n2, size, 1); // load 1 into integer 2
    printf("F(   0): %d\n", 0); // print first term
    printf("F(   1): %d\n", 1); // print second term
    int i = 2;
    // loop through the fib process with adding numbers
    //rotating through the three integers, until a sum causes an overflow error
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

// initialize the array with the int value
void initialize(char digits[], unsigned numdigits, unsigned value) {
    int i;
    // load integer digits into the char array using the mod operator and int division to get one at a time
    for (i = 0; i < numdigits; i++) {
        digits[i] = '0' + (int) (((int)value/ (int)pow(10, (numdigits-1-i)))%10);
    }
}

// add two input numbers (in char arrays) into an output (char array)
int add(char result[], char num1[], char num2[], unsigned numdigits) {
	int i;
	int carry = 0;
	// add the numbers into the result array, one digit at a time, and carry to the next digit when needed
	for(i = numdigits - 1; i >= 0; i--) {
	    // subtract the '0' char value to get the number value
		int number = num1[i] - '0' + num2[i] - '0' + carry;
		if (number > 9) {
			carry = 1;
			number -= 10;
		} else {
            carry = 0;
        }
		// add the '0' char value to get the correct char representation
		result[i] = (number + '0');
		if (i == 0 && carry == 1) {
            return 1;
		}
	}
	return 0;
}

// print the large integer one digit at a time to the console and add , in every 3 places
void print_int(char digits[], unsigned numdigits) {
	int i;
	int zeros = 1;
	for (i = 0; i < numdigits; i++) {
	    if (digits[i] != '0') zeros = 0;
	    if (!zeros) {
            printf("%c", digits[i]);
            // check mod 3 to add a , every 3
            if ((numdigits-1-i) % 3 == 0 && i != numdigits-1) {
                printf(",");
            }
	    }
	}
	printf("\n");
}
 
