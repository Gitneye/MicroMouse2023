#include <Arduino.h>
#include "constants_and_types.h"
#include "ir.h"

#define IR_SAMPLES 100

int irBackgroundRR = 0;
int irBackgroundFR = 0;
int irBackgroundFL = 0;
int irBackgroundLL = 0;

void initialise_ir() {
	irBackgroundRR = analogRead(IR_RR);
	
	irBackgroundFL = analogRead(IR_FL);
	irBackgroundLL = analogRead(IR_LL);
}

int read_ir(uint8_t ir, uint8_t sampleSize) {
	int background = 0;
	switch (ir) {
		case IR_RR:
			background = irBackgroundRR;
			break;
		case IR_FR:
			background = irBackgroundFR;
			break;
		case IR_FL:
			background = irBackgroundFL;
			break;
		case IR_LL:
			background = irBackgroundLL;
			break;
	}
	
	long sum = 0;
	for (int i = 0; i < sampleSize; i++) {
		sum += analogRead(ir) - background;
	}

	return sum / sampleSize;
}

uint8_t sense_walls(uint8_t mouseOrientation) {
	uint8_t walls = 0;
	walls |= (read_ir(IR_LL, IR_SAMPLES) >= 500) ? WEST : 0; 
	walls |= ((read_ir(IR_FL, IR_SAMPLES) >= 700) && (read_ir(IR_FR, IR_SAMPLES) >= 700)) ? NORTH : 0; 
	walls |= (read_ir(IR_RR, IR_SAMPLES) >= 500) ? EAST : 0; 
	
	while (mouseOrientation >= NORTH) {
		walls = (walls >> 1) | (walls << 3);
		mouseOrientation <<= 1;
	}
	return walls;
}