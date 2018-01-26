#include<stdio.h>

int main() {
	char c = 'a';
	int space = 0;
	int length = 0;
	while(c != -1) {
		c = getchar();
		if (c != 0x0a && c != -1) {
			printf("%02x", (int) c);
			length = length + 1;
		}
		else if (c == 0x0a) {
			printf("\n%i\n", length);
			space = 0;
			length = 0;
			continue;
		}
		if (space == 1)
			printf(" ");
		space = space ^ 1;
	}
	return 0;
}
