#include <Arduino.h>
#include <Encoder.h>
#include "drive.h"
#include "imu.h"

// Motor A Definitions
#define fwA 4
#define bwA 3
#define enA 5
// Motor B Definitions
#define fwB 7
#define bwB 8
#define enB 9

// Encoder definitions
#define encoAA 10
#define encoAB 11
#define encoBA 12
#define encoBB 13

#define pulsesPerRevolution 420.0
#define wheelDiameter 45.0
#define pulsesPerMM (pulsesPerRevolution/wheelDiameter)
#define cellPulseLength 1493.3333

struct MotorPins {
	uint8_t forward;
	uint8_t backward;
	uint8_t enable;
};

static struct MotorPins left = {.forward = fwA, .backward = bwA, .enable = enA};
static struct MotorPins right = {.forward = fwB, .backward = bwB, .enable = enB};

Encoder leftEncoder(encoAA, encoAB);
Encoder rightEncoder(encoBA, encoBB);

void motor_initialise() {
	pinMode(left.forward, OUTPUT);
	pinMode(left.backward, OUTPUT);
	pinMode(left.enable, OUTPUT);

	pinMode(right.forward, OUTPUT);
	pinMode(right.backward, OUTPUT);
	pinMode(right.enable, OUTPUT);

	motor_off();
}

void motor_off() {
	digitalWrite(left.forward, LOW);
	digitalWrite(left.backward, LOW);

	digitalWrite(right.forward, LOW);
	digitalWrite(right.backward, LOW);
}

void motor_speed(enum Motor motor, float speed) {
	switch (motor) {
		case LEFT:
			digitalWrite(left.forward, (speed > 0) ? HIGH : LOW);
			digitalWrite(left.backward, (speed > 0) ? LOW : HIGH);
			analogWrite(left.enable, abs(speed)/100 * 255);
			break;
		case RIGHT:
			digitalWrite(right.forward, (speed > 0) ? HIGH : LOW);
			digitalWrite(right.backward, (speed > 0) ? LOW : HIGH);
			analogWrite(right.enable, abs(speed)/100 * 255);
			break;
	}
}

void rotate(float angle) {
	struct IMUData base = {0};
	struct IMUData compare = {0};
	read_imu(&base);
	read_imu(&compare);

	compare.roll += angle;
	motor_speed(LEFT, (angle > 0) ? -100 : 100);
	motor_speed(RIGHT, (angle > 0) ? 100 : -100);
	while (base.roll <= compare.roll) {
		read_imu(&base);
	}
	motor_speed(LEFT, 0);
	motor_speed(RIGHT, 0);
}

void drive_to_next_cell() {
	long leftPulses = leftEncoder.read();
	long rightPulses = rightEncoder.read();
	motor_speed(LEFT, 100);
	motor_speed(RIGHT, 100);
	while ((leftPulses + cellPulseLength < leftEncoder.read()) && (rightPulses + cellPulseLength < rightEncoder.read())) {
		if (leftPulses + cellPulseLength >= leftEncoder.read()) {
			motor_speed(LEFT, 0);
		}

		if (rightPulses + cellPulseLength >= rightEncoder.read()) {
			motor_speed(RIGHT, 0);
		}
	}
}