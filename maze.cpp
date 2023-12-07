#include <Arduino.h>
#include <stdio.h>
#include "constants_and_types.h"
#include "maze.h"

#define LINES_PER_CELL 3

void initialise_maze(struct Cell *maze) {
	memset(maze, 0, sizeof(*maze) * MAZE_WIDTH * MAZE_WIDTH);

	// Initialise walls
	for (int i = 0; i < MAZE_WIDTH; i++) {
		maze[i].walls |= NORTH;
		maze[i * MAZE_WIDTH].walls |= WEST;
		maze[(MAZE_WIDTH - 1) * MAZE_WIDTH + i].walls |= SOUTH;
		maze[i * MAZE_WIDTH + MAZE_WIDTH - 1].walls |= EAST;
	}

	// Initialise distances;
	for (int row = 0; row < MAZE_WIDTH; row++) {
		for (int col = 0; col < MAZE_WIDTH; col++) {
			maze[row * MAZE_WIDTH + col].distanceTo = abs(MAZE_WIDTH/2 - row) + abs(MAZE_WIDTH/2 - col);
		}
	}
}

void print_cell(struct Cell *cell) {
	Serial.print((cell->walls & NORTH) ? "+---+" : "+   +");
	Serial.print("\n");

	Serial.print((cell->walls & WEST) ? "|   " : "   ");
	Serial.print((cell->walls & EAST) ? "|" : " ");
	Serial.print("\n");

	Serial.print((cell->walls & SOUTH) ? "+---+" : "+   +");
	Serial.print("\n");
}

void print_maze(struct Cell *maze, struct Cell *mousePos, uint8_t mouseOrientation) {
	for (uint8_t row = 0; row < MAZE_WIDTH; row++) {
		for (uint8_t line = 0; line < LINES_PER_CELL - 1; line++) {
			struct Cell *cell;
			for (uint8_t col = 0; col < MAZE_WIDTH; col++) {
				cell = &maze[row * MAZE_WIDTH + col];
				if (line == 0) {
					// Top of cell
					Serial.print((cell->walls & NORTH) ? "+---" : "+   ");
				} else {
					// Middle of cell
					Serial.print((cell->walls & WEST) ? "|" : " ");
					
					// Cell centre
					char info[4];
					snprintf(info, 4, " %-2i", cell->distanceTo);
					info[0] = cell->visited ? '*' : ' ';
					if (cell == mousePos) {
						switch (mouseOrientation) {
							case NORTH:
								info[0] = '^';
								break;
							case EAST:
								info[0] = '>';
								break;
							case SOUTH:
								info[0] = 'v';
								break;
							case WEST:
								info[0] = '<';
								break;
							default:
								info[0] = 'm';
								break;
						}
					}
					Serial.print(info);
				}			
			}
			if (line == 0) {
					Serial.println("+");
			} else {
				 Serial.println((cell->walls & EAST) ? "|" : " ");
			}
		}
	}

	for (int col = 0; col < MAZE_WIDTH; col++) {
		struct Cell *cell = &maze[(MAZE_WIDTH - 1) * MAZE_WIDTH + col];
		Serial.print((cell->walls & SOUTH) ? "+---" : "+   ");
	}
	Serial.println("+");	
}