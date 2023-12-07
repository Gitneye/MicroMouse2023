#ifndef _SENSOR_DATA_H
#define _SENSOR_DATA_H

#ifdef SD_NO_SENSORS

#include "constants_and_types.h"

#define SENSE_WALLS(x, y) SENSOR_DATA[y][x].walls

// Maze data from micromouse rules maze
const struct Cell SENSOR_DATA[MAZE_WIDTH][MAZE_WIDTH] = {
	{{.walls = 9},  {.walls = 8}, {.walls = 14},  {.walls = 9}, {.walls = 10}, {.walls = 10}, {.walls = 10}, {.walls = 10}, {.walls = 12}},
	{{.walls = 5},  {.walls = 3}, {.walls = 10},  {.walls = 6}, {.walls = 11}, {.walls = 10}, {.walls = 12},  {.walls = 9},  {.walls = 6}},
	{{.walls = 1}, {.walls = 10}, {.walls = 14},  {.walls = 9}, {.walls = 10}, {.walls = 10},  {.walls = 4},  {.walls = 3}, {.walls = 12}},
	{{.walls = 1}, {.walls = 14},  {.walls = 9},  {.walls = 6},  {.walls = 9}, {.walls = 12},  {.walls = 3}, {.walls = 10},  {.walls = 6}},
	{{.walls = 3}, {.walls = 12},  {.walls = 1}, {.walls = 14},  {.walls = 5},  {.walls = 5},  {.walls = 9}, {.walls = 12}, {.walls = 13}},
	{{.walls = 9},  {.walls = 6},  {.walls = 3}, {.walls = 12},  {.walls = 5},  {.walls = 5},  {.walls = 5},  {.walls = 5},  {.walls = 5}},
	{{.walls = 3},  {.walls = 8}, {.walls = 12},  {.walls = 5},  {.walls = 5},  {.walls = 5},  {.walls = 5},  {.walls = 5},  {.walls = 5}},
	{{.walls = 9},  {.walls = 4},  {.walls = 5},  {.walls = 3},  {.walls = 6},  {.walls = 5},  {.walls = 5},  {.walls = 3},  {.walls = 6}},
	{{.walls = 7},  {.walls = 7},  {.walls = 3}, {.walls = 10}, {.walls = 14},  {.walls = 3},  {.walls = 2}, {.walls = 10}, {.walls = 14}}
};

#endif //SD_NO_SENSORS
#endif //_SENSOR_DATA_H