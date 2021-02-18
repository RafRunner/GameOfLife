#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>

#define MY_KEY_ENTER 10

short** allocate_matrix(int rows, int columns) {
	short** out = calloc(rows, sizeof(short**));

	for (int i = 0; i < rows; i++) {
		out[i] = calloc(columns, sizeof(short*));
	}

	return out;
}

void free_matrix(short** matrix, int rows) {
	for (int i = 0; i < rows; i++) {
		free(matrix[i]);
		matrix[i] = NULL;
	}

	free(matrix);
	matrix = NULL;
}

int find_live_neighbors(short** grid, int row, int column, int rows, int columns) {
	int live_neighbors = 0;

	for (int i = 0; i < 3; i++) {
		int check_row = row - 1 + i;
		if (check_row < 0 || check_row == rows) {
			continue;
		}
		
		for (int j = 0; j < 3; j++) {			
			int check_column = column - 1 + j;
			if ((i == 1 && j == 1) || check_column < 0 || check_column == columns) {
				continue;
			}

			if (grid[check_row][check_column]) {
				live_neighbors++;
			}
		}
	}

	return live_neighbors;
}

short** game_of_life(short** grid, int rows, int columns) {
	short** next_iteration = allocate_matrix(rows, columns);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			int live_neighbors = find_live_neighbors(grid, i, j, rows, columns);

			if (grid[i][j]) {
				if (live_neighbors == 2 || live_neighbors == 3) {
					next_iteration[i][j] = 1;
				}
			}
			else if (live_neighbors == 3) {
				next_iteration[i][j] = 1;
			}
		}
	}

	return next_iteration;
}

void print_matrix(short** matrix, int rows, int columns) {
	move(0, 0);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (matrix[i][j]) {
				addch('X');
			}
			else {
				addch(' ');
			}
		}
	}
}

void edit_grid(short** grid, int rows, int columns) {
	int x = columns / 2, y = rows / 2;
	
	// Add blinking cursor
	curs_set(1);
	print_matrix(grid, rows, columns);
	move(y, x);
	
	int input = getch();

	while (input != KEY_ENTER && input != MY_KEY_ENTER) {
		switch (input) {
			case KEY_DOWN:
				if (y < rows - 1) y++;
				break;
			case KEY_UP:
				if (y > 0) y--;
				break;
			case KEY_RIGHT:
				if (x < columns - 1) x++;
				break;
			case KEY_LEFT:
				if (x > 0) x--;
				break;

			// C or c for clearing the screen
			case 67:
			case 99:
				for (int i = 0; i < rows; i++) {
					for (int j = 0; j < columns; j++) {
						grid[i][j] = 0;
					}
				}
				print_matrix(grid, rows, columns);
				move(y, x);
				break;

			// Q or q to quit
			case 81:
			case 113:
				endwin();
				exit(0);
			
			case KEY_BACKSPACE:
				addch(' ');
				grid[y][x] = 0;
				break;
			default:
				grid[y][x] = 1;
				addch('X');
		}
		
		move(y, x);
		input = getch();
	}

	// Remove blinking cursor
	curs_set(0);
}

void execute_game(short** grid, int rows, int columns) {
	print_matrix(grid, rows, columns);

	while (1) {
		short** next_iteration = game_of_life(grid, rows, columns);

		print_matrix(next_iteration, rows, columns);

		free_matrix(grid, rows);
		grid = next_iteration;

		char input = getch();
		if (input == 'q' || input == 'Q') {
			break;
		}

		if (input == 'e' || input == 'E') {
			edit_grid(grid, rows, columns);
		}
	}
}

int main() {
	int rows;
	int columns;
	
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	getmaxyx(stdscr, rows, columns);
	noecho();

	short** start_grid = allocate_matrix(rows, columns);

	edit_grid(start_grid, rows, columns);

	execute_game(start_grid, rows, columns);

	endwin();

	return 0;
}
