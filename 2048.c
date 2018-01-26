/* 2048 (Generalized)
 * This file will create a game of 2048 if run without any other parameters
 *
 * With a board dimension and a base, the game will create a game with a square
 * of that dimension, and tiles will spawn according to that base. It should be
 * noted that you must have the correct base number of tiles to combine them
 * (e.g. a base of 3 requires 3 adjacent tiles to collapse to a new tile)
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

void clear() {
	system("clear");
}

// Creates a random tile on the board
void initialize_board(int* board, int dim, int base) {
	for(int i = 0; i < dim*dim; i++)
		board[i] = -1;
	srand(time(NULL));
	int p = rand() % (dim * dim);
	int t = base + (base * base - base) * ((rand() % 2));
	board[p] = t;
}

// Prints out the board
void print_board(int* board, int dim) {
	int col_spaces = 1;
	for(int i = 0; i < dim*dim; i++) {
		if (floor(log10(abs(board[i]))) + 1 >= col_spaces)
			col_spaces = floor(log10(abs(board[i]))) + 2;
	}

	for(int i = 0; i < dim; i++) {
		printf("+");
		for (int k = col_spaces; k > 0; k--)
			printf("-");
	}
	printf("+\n");

	int digits = 0;
	for(int i = 0; i < dim; i++) {
		printf("|");
		for (int j = 0; j < dim; j++) {
			digits = floor(log10(abs(board[dim*i + j]))) + 1;
			for (int k = col_spaces; k > digits; k--)
				printf(" ");
			if (board[i*dim + j] == -1)
				printf(" |");
			else
				printf("%d|", board[dim*i + j]);
		}

		printf("\n");
		for(int i = 0; i < dim; i++) {
			printf("+");
			for (int k = col_spaces; k > 0; k--)
				printf("-");
		}
		printf("+\n");
	}
}

// Check that there is a move that can be done
int is_move(int* board, int dim, int base) {
	for (int i = 0; i < dim*dim; i++) {
		if(board[i] == -1)
			return 1;
	}
	// Check for tiles that can combine
	int cnt = 0;
	int curr = 0;
	for (int i = 0; i < dim; i++) {
		// Check ith row
		cnt = 1;
		curr = board[dim*i];
		for (int j = 1; j < dim; j++) {
			if (cnt == base) {
				return 1;
			}
			if (curr == board[dim*i + j]) {
				cnt++;
			}
			else {
				cnt = 1;
				curr = board[dim*i + j];
			}
		}

		// Check ith col
		cnt = 1;
		curr = board[i];
		for (int j = 1; j < dim; j++) {
			if (cnt == base) {
				return 1;
			}
			if (curr == board[dim*j + i]) {
				cnt++;
			}
			else {
				cnt = 1;
				curr = board[dim*j + i];
			}
		}
	}
	return 0;
}

// Wait for directional input and update the board
// Returns the number of updates
int make_move(int* board, int dim, int base, char move) {
	int* new = malloc(dim * sizeof(int));
	int ptr;
	int tile;
	int equ = 1;
	int chng = 0;
	switch(move) {
		case 'j': // up
			// This method is still bugged
			// traverse all columns
			for (int i = 0; i < dim ; i++) {
				// Figure out what the new column will be by treating it as a stack
				// reset the new arr
				for (int j = 0; j < dim; j++)
					new[j] = -1;
				ptr = 0;
				for (int j = 0; j < dim; j++) {
					if (board[dim*j + i] == -1)
						continue;
					new[ptr] = board[dim*j + i];
					tile = board[dim*j + i];
					while(ptr >= base-1) {
						// Try to combine things
						equ = 1;
						for (int k = 0; k < base; k++) // Check that there are at least base of the same tile
							equ = equ & (new[ptr - k] == tile);
						if (equ == 0)
							break;
						// otherwise combine the tiles
						new[ptr-base+1] = base * tile;
						tile = base * tile;
						// Change other parts to -1
						for (int k = 0; k < base - 1; k++)
							new[ptr-k] = -1;
						// change pointer to continue collapse
						ptr = ptr-base+1;
					}
					ptr++;
				}
				// Update the board
				for (int j = 0; j < dim; j++) {
					if (board[dim*j + i] != new[j])
						chng++;
					board[dim*j + i] = new[j];
				}	
			}
			break;
		case 'k': // down
			// traverse all columns
			for (int i = 0; i < dim ; i++) {
				// Figure out what the new column will be by treating it as a stack
				// reset the new arr
				for (int j = 0; j < dim; j++)
					new[j] = -1;
				ptr = 0;
				for (int j = dim-1; j >= 0; j--) {
					if (board[dim*j + i] == -1)
						continue;
					new[ptr] = board[dim*j + i];
					tile = board[dim*j + i];
					while(ptr >= base-1) {
						// Try to combine things
						equ = 1;
						for (int k = 0; k < base; k++) // Check that there are at least base of the same tile
							equ = equ & (new[ptr - k] == tile);
						if (equ == 0)
							break;
						// otherwise combine the tiles
						new[ptr-base+1] = base * tile;
						tile = base * tile;
						// Change other parts to -1
						for (int k = 0; k < base - 1; k++)
							new[ptr-k] = -1;
						// change pointer to continue collapse
						ptr = ptr-base+1;
					}
					ptr++;
				}
				// Update the board
				for (int j = 0; j < dim; j++) {
					if (board[dim*j + i] != new[dim - j - 1])
						chng++;
					board[dim*j + i] = new[dim - j - 1];
				}
			}
			break;
		case 'l': // right
			for (int j = 0; j < dim ; j++) {
				// Figure out what the new will be by treating it as a stack
				// reset the new arr
				for (int i = 0; i < dim; i++)
					new[i] = -1;
				ptr = 0;
				for (int i = dim-1; i >= 0; i--) {
					if (board[dim*j + i] == -1)
						continue;
					new[ptr] = board[dim*j + i];
					tile = board[dim*j + i];
					while(ptr >= base-1) {
						// Try to combine things
						equ = 1;
						for (int k = 0; k < base; k++) // Check that there are at least base of the same tile
							equ = equ & (new[ptr - k] == tile);
						if (equ == 0)
							break;
						// otherwise combine the tiles
						new[ptr-base+1] = base * tile;
						tile = base * tile;
						// Change other parts to -1
						for (int k = 0; k < base - 1; k++)
							new[ptr-k] = -1;
						// change pointer to continue collapse
						ptr = ptr-base+1;
					}
					ptr++;
				}
				// Update the board
				for (int i = 0; i < dim; i++) {
					if (board[dim*j + i] != new[dim - i - 1])
						chng++;
					board[dim*j + i] = new[dim - i - 1];
				}
			}
			break;
		case 'h': // left
			for (int j = 0; j < dim ; j++) {
				// Figure out what the new will be by treating it as a stack
				// reset the new arr
				for (int i = 0; i < dim; i++)
					new[i] = -1;
				ptr = 0;
				for (int i = 0; i < dim; i++) {
					if (board[dim*j + i] == -1)
						continue;
					new[ptr] = board[dim*j + i];
					tile = board[dim*j + i];
					while(ptr >= base-1) {
						// Try to combine things
						equ = 1;
						for (int k = 0; k < base; k++) // Check that there are at least base of the same tile
							equ = equ & (new[ptr - k] == tile);
						if (equ == 0)
							break;
						// otherwise combine the tiles
						new[ptr-base+1] = base * tile;
						tile = base * tile;
						// Change other parts to -1
						for (int k = 0; k < base - 1; k++)
							new[ptr-k] = -1;
						// change pointer to continue collapse
						ptr = ptr-base+1;
					}
					ptr++;
				}
				// Update the board
				for (int i = 0; i < dim; i++) {
					if (board[dim*j + i] != new[i])
						chng++;
					board[dim*j + i] = new[i];
				}				
			}
			break;
		default:
			break;
	}
	free(new);
	return chng;

}

void spawn_tile(int* board, int dim, int base) {
	int open_spot = 0;
	for (int i = 0; i < dim*dim; i++)
		open_spot = open_spot | (board[i] == -1);
	if (open_spot == 0)
		return;
	int pos = rand() % (dim * dim);
	while (board[pos] != -1)
		pos = rand() % (dim * dim);
	board[pos] = base + (base * base - base) * ((rand() % 2));
	//board[pos] = 7;
}

int main(int argc, char* argv[]) {
	int dim = 4;
	int base = 2;
	if (argc >= 2)
		sscanf(argv[1], "%i", &dim);
	if (argc >= 3)
		sscanf(argv[2], "%i", &base);
	if (dim < base) {
		printf("Bad parameters, you won't be able to combine anything\n");
		return 1;
	}
	int* board = malloc(sizeof(int) * dim * dim);
	char move;
	initialize_board(board, dim, base);
	clear();
	print_board(board, dim);
	int chng = 0;
	while(is_move(board, dim, base) != 0) {
		char move = getchar();
		if (move == -1)  // EOF Character
			break;
		if (move == 10) // EOL character
			continue;
		if (make_move(board, dim, base, move) > 0)
			spawn_tile(board, dim, base);
		clear();
		print_board(board, dim);
	}
	if (is_move(board, dim, base) == 0)
		printf("Game over! There are no more possible moves.\n");

	free(board);
}
