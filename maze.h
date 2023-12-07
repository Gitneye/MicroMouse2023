#ifndef _MAZE_H
#define _MAZE_H

#include <stdint.h>
#include "constants_and_types.h"


/* initialise_maze()
 * -----------------
 * Initialised maze buffer with only boundary walls, no interior walls. 
 * Distances are set.
 * 
 * maze: Buffer of cells to be initialised
 */
void initialise_maze(struct Cell *maze);

/* print_cell()
 * ------------
 * Prints the given cell. Used to visualise the walls of the particular cell.
 * 
 * cell: Pointer to cell to be printed
 */
void print_cell(struct Cell *cell);

/* print_maze()
 * ------------
 * Prints maze and mouse to the serial port. Assumes maze is consistent. 
 * mousePos and mouseOrientation can both be 0, if so then mouse won't be 
 * printed.
 * 
 * maze: Array of maze cells
 * mousePos: Pointer to the cell the mouse is currently in
 * mouseOrientation: Current orientaiton of mouse
 */
void print_maze(
	struct Cell *maze, 
	struct Cell *mousePos, 
	uint8_t mouseOrientation);

#endif //_MAZE_H
