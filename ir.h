#ifndef _IR_H
#define _IR_H

#include <stdint.h>

#define IR_RR A0
#define IR_FR A1
#define IR_FL A2
#define IR_LL A6

void initialise_ir();

int read_ir(uint8_t ir, uint8_t sampleSize);

uint8_t sense_walls(uint8_t mouseOrientation);

#endif //_IR_H