#ifndef _FLOOD_FILL_H
#define _FLOOD_FILL_H

#include "constants_and_types.h"

/* Returns the x or y position of the given cell. maze and cell are of type 
 * struct cell *.
 */
#define POS_X(maze, cell) (uint8_t)(((cell) - (maze)) % MAZE_WIDTH)
#define POS_Y(maze, cell) (uint8_t)(((cell) - (maze)) / MAZE_WIDTH)

/* update_walls()
 * --------------
 * Adds given walls to the existing walls of pos and updates the walls of cells
 * that share a wall with pos.
 *
 * maze: Array of maze cells
 * pos: Pointer to cell in maze
 * walls: Walls to be added to pos and neighbours
 */
void update_walls(struct Cell *maze, struct Cell *pos, uint8_t walls);

/* update_distance()
 * -----------------
 * Using flood fill, updates the distance from each cell to the target cell.
 * 
 * maze: Array of maze cells to be updated
 * target: Cell used as destination in distance measurements
 */
void update_distance(struct Cell *maze, struct Cell *target);

/* to_move()
 * ---------
 * Returns the cell the mouse should move to, given their distances. 
 * Preferences cells in the direction of orientation when there is a tie.
 * 
 * maze: Array of maze cells 
 * pos: Current cell occupied by mouse
 * orientation: Orientation of mouse in cell
 */
struct Cell *to_move(struct Cell *maze, struct Cell *pos, uint8_t orientation);

/* get_direction()
 * ---------------
 * Returns the closets direction matching the vector between to and from.
 * 
 * maze: Array of maze cells
 * to: Cell used as tip of vector
 * from: Cell used as base of vector
 */
uint8_t get_direction(struct Cell *maze, struct Cell *to, struct Cell *from);

#endif //_FLOOD_FILL_H
