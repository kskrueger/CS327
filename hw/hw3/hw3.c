#include<stdio.h>
#include<stdlib.h>

void initialize(char digits[], unsigned numdigits, unsigned value);
int add(char result[], char num1[], char num2[], unsigned numdigits);
void print_int(char digits[], unsigned numdigits);

int main() {
	char* digits;
	initialize(digits, 10, 0);
	int i;
	for (i = 0; i < 10; i++) {
		printf(digits[i]);
	}
}

void initialize(char digits[], unsigned numdigits, unsigned value) {
	int i;
	for (i = 0; i < numdigits; i++) {
		digits[i] = malloc(sizeof(char));
		digits[i] = ' ';
	}
}

int add(char result[], char num1[], char num2[], unsigned numdigits) {


}

void print_int(char digits[], unsigned numdigits) {

}
 
