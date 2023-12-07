#include <Arduino.h>
#include <stdlib.h>
#include <stdint.h>
#include "constants_and_types.h"
#include "maze.h"
#include "flood_fill.h"

 
/* cell_in_direction()
 * -------------------
 * Returns a pointer to the cell in the given direction away from pos. If such 
 * a cell in outside the confines of the maze, returns NULL.
 * 
 * maze: Array of maze cells
 * pos: Based cell used when calculating result 
 * direction: Direction to move away from pos
 * 
 * Return: Pointer to cell in direction away from pos, NULL if out-of-bounds
 */
static struct Cell *cell_in_direction(struct Cell *maze, struct Cell *pos, uint8_t direction) {
	switch (direction) {
		case NORTH:
		  	if (pos >= maze + MAZE_WIDTH) {
				pos -= MAZE_WIDTH;	
				return pos;
			}
			break;
		case EAST:
		  	if (POS_X(maze, pos) < MAZE_WIDTH - 1) {
				pos += 1;
				return pos;
			}
			break;
		case SOUTH:
		  	if (pos < maze + MAZE_WIDTH * (MAZE_WIDTH - 1)) {
				pos += MAZE_WIDTH;	
				return pos;
			}
			break;
		case WEST:
		  	if (pos > maze) {
				pos -= 1;
				return pos;
			}
			break;
	}
	return 0;
}

/* get_accessible()
 * ----------------
 * Stores all cells that can be reached from cell (are not blocked by a wall) 
 * in buffer. Stores the number of such cells in num.
 * 
 * buffer: Buffer of struct Cell** pointers used to store result
 * num: Pointer to int used to store number of accessible cells
 * maze: Array of maze cells
 * cell: Pointer to cell which resultant cells are accessible from
 */
static void get_accessible(
		struct Cell **buffer, 
		uint8_t *num, 
		struct Cell *maze, 
		struct Cell *cell) {
	*num = 0;
	for (uint8_t direction = 1 << 0; direction < 1 << 4; direction <<= 1) {
		if (cell->walls & direction) {
			continue;
		}

		struct Cell *child = cell_in_direction(maze, cell, direction);

		if (child) {
			buffer[*num] = child;
			(*num)++;
		}
	}
}

void update_walls(struct Cell *maze, struct Cell *pos, uint8_t walls) {
	pos->walls |= walls;

	for (uint8_t direction = 1 << 0; direction < 1 << 4; direction <<= 1) {
		if (!(walls & direction)) {
			continue;
		}

		struct Cell *toUpdate = cell_in_direction(maze, pos, direction);
		uint8_t opposite = (direction >= 1 << 2) ? direction >> 2 : direction << 2;
		toUpdate->walls |= opposite;
	}

}

void update_distance(struct Cell *maze, struct Cell *target) {
	struct Buffer {
		uint8_t size;
		struct Cell **data;
	};

	uint8_t listed[MAZE_WIDTH * MAZE_WIDTH] = {0};

	uint8_t which = 0;
	struct Buffer buffers[2] = {0};

	buffers[which].size = 2;
	buffers[which].data = (struct Cell **) malloc(sizeof(struct Cell *) * buffers[which].size);
	buffers[which].data[0] = target;
	buffers[which].data[1] = 0;

	listed[target - maze] = 1;

	buffers[1 - which].size = 1;
	buffers[1 - which].data = (struct Cell **) malloc(sizeof(struct Cell *) * buffers[which].size);
	buffers[1 - which].data[0] = 0 ;

	uint8_t distance = 0;

	while (*(buffers[which].data)) {
		struct Buffer *parents = &buffers[which];
		struct Buffer *children = &buffers[1 - which];

		uint8_t next = 0;
		for (struct Cell **cell = parents->data; *cell; cell++) {
			(*cell)->distanceTo = distance;

			uint8_t num;
			struct Cell *toAdd[4];
			get_accessible(toAdd, &num, maze, (*cell));
			for (uint8_t i = 0; i < num; i++) {
				if (listed[toAdd[i] - maze]) {
					continue;
				}

				listed[toAdd[i] - maze] = 1;
				children->data[next] = toAdd[i];
				next++;

				if (next >= children->size) {
					children->size += MAZE_WIDTH;
					children->data = (struct Cell **) realloc(children->data, sizeof(struct Cell *) * children->size);
				}
			}
		}
		distance++;
		children->data[next] = 0;
		which = 1 - which;
	}
	free(buffers[0].data);
	free(buffers[1].data);
}

struct Cell *to_move(struct Cell *maze, struct Cell *pos, uint8_t orientation) {
	struct Cell *toMove = NULL;
	uint8_t minDistance = 0xFF;

	for (uint8_t direction = 1 << 0; direction < 1 << 4; direction <<= 1) {
		if (pos->walls & direction) {
			continue;
		}
		
		struct Cell *child = cell_in_direction(maze, pos, direction);
		if ((child->distanceTo < minDistance) 
				|| (child->distanceTo == minDistance && direction == orientation)) {
			minDistance = child->distanceTo;
			toMove = child;
		}
	}
	return toMove;
}

uint8_t get_direction(struct Cell *maze, struct Cell *to, struct Cell *from) {
	int16_t xDiff = POS_X(maze, to) - POS_X(maze, from);
	int16_t yDiff = POS_Y(maze, to) - POS_Y(maze, from);

	if (abs(xDiff) > abs(yDiff)) {
		if (xDiff > 0) {
			return EAST;
		}
		return WEST;
	}

	if (yDiff > 0) {
		return SOUTH;
	}
	return NORTH;
}
