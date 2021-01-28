#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>


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

void print_matrix(short** matrix, int rows, int columns) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (matrix[i][j]) {
				printw("X ");
			}
			else {
				printw("_ ");
			}
		}
		printw("\n");
	}
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

void execute_game(short** start_grid, int rows, int columns) {
	short** grid = start_grid;
	
	print_matrix(grid, rows, columns);
	move(0, 0);

	for (int i = 0; i < 100; i++) {
		short** next_iteration = game_of_life(grid, rows, columns);
	
		print_matrix(next_iteration, rows, columns);
		move(0, 0);

		free_matrix(grid, rows);
		grid = next_iteration;

		getch();
	}
}

int main() {
	int rows;
	int columns;
	
	initscr();
	raw();
	keypad(stdscr, TRUE);
	getmaxyx(stdscr, columns, rows);
	noecho();
	curs_set(0);

	short** start_grid = allocate_matrix(rows, columns);

	start_grid[0][1] = 1;
	start_grid[1][2] = 1;
	start_grid[2][0] = 1;
	start_grid[2][1] = 1;
	start_grid[2][2] = 1;

	execute_game(start_grid, rows, columns);

	endwin();

	return 0;
}
