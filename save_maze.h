#ifndef _SAVE_MAZE_H
#define _SAVE_MAZE_H 

#include <EEPROM.h>

#define MAZE_ADDRESS sizeof(uint8_t)

#define MAZE_SAVED (!EEPROM.read(0))
#define SAVE_MAZE(maze) (EEPROM.put(0, (uint8_t) 0), EEPROM.put(MAZE_ADDRESS, (maze)))
#define LOAD_MAZE(maze) EEPROM.get(MAZE_ADDRESS, (maze))
#define RESET_MAZE() EEPROM.put(0, (uint8_t) 0xFF)

#endif //_SAVE_MAZE_H