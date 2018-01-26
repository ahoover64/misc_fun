#include<stdio.h>

// Takes 2 digit hex values and converts them to numbers and ascii values
int main() {
	int history = -1;
	unsigned int num = 0;
	while(num != history) {
		history = num;
		scanf("%02x", &num);
		printf("Base 10: %i\n", num);
		printf("As a character (may not be visible): %c\n", (char) num); // Print as a character
	}
	return 0;
}
