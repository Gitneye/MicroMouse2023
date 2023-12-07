#include <Arduino.h>
#include <stdint.h>
#include "constants_and_types.h"
#include "maze.h"
#include "flood_fill.h"
#include "save_maze.h"
#include "drive.h"
#include "ir.h"
#include "imu.h"

#define SD_NO_SENSORS
#include "sensor_data.h"

#define SAMPLES 50


void setup() {
	Serial.begin(9600);

	motor_initialise();
	initialise_ir();
	initialise_imu();
	
	struct Cell maze[MAZE_WIDTH * MAZE_WIDTH];
	initialise_maze(maze);

	struct Cell *centre = &maze[MAZE_WIDTH * (MAZE_WIDTH/2)  + (MAZE_WIDTH/2)];
	struct Cell *start = &maze[MAZE_WIDTH * 0  + 0];
	struct Cell *goal = centre;

	struct Cell *mousePos = start;
	uint8_t mouseOrientation = EAST;

	//#define VERBOSE

	if (MAZE_SAVED) {
		LOAD_MAZE(maze);
		Serial.println("Loaded maze");
		print_maze(maze, mousePos, mouseOrientation);
		RESET_MAZE();
	}

	uint16_t iteration = 0;
	while (iteration < 150) { 
		iteration++;
		
		mousePos->visited = 1;

		update_walls(maze, mousePos, sense_walls(mouseOrientation));
		update_distance(maze, goal);

		#ifdef VERBOSE
		print_maze(maze, mousePos, mouseOrientation);
		#endif

		if (mousePos == goal) {
			if (goal == start) {
				update_distance(maze, centre);
				break;
			}
			goal = start;
			update_distance(maze, goal);
		}

		struct Cell *toMove = to_move(maze, mousePos, mouseOrientation);
		uint8_t toOrient = get_direction(maze, toMove, mousePos);

		if (toOrient != mouseOrientation) {
			uint8_t count = 0;
			while (mouseOrientation != toOrient) {
				mouseOrientation = (mouseOrientation >> 1) | (mouseOrientation << 3);
				count++;
			}
			float angle = (count == 3) ? 90 : count * (-90);
			rotate(angle);

			#ifdef VERBOSE
			print_maze(maze, mousePos, mouseOrientation);
			#endif
		}
		drive_to_next_cell();
		mousePos = toMove;
	}
	print_maze(maze, mousePos, mouseOrientation);
	Serial.println(iteration);
	SAVE_MAZE(maze);
}

void loop() {

}
