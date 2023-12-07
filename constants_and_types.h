#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>

#define MAZE_WIDTH 9

#define NORTH 8
#define EAST  4
#define SOUTH 2
#define WEST  1

struct Cell {
	uint8_t walls;
	uint8_t distanceTo;
	uint8_t visited;
};

#endif // _TYPES_H