#include <Arduino.h>
#include <Wire.h>
#include "imu.h"

#define IMU_SAMPLES 400

float accErrorX = 0;
float accErrorY = 0;
float gyroErrorX = 0;
float gyroErrorY = 0;
float gyroErrorZ = 0;

float elapsedTime = 0;
float currentTime = 0;
float previousTime = 0;

static void initialise_imu_error() {
	// We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
	// Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
	// Read accelerometer values IMU_SAMPLES times
	for (int i = 0; i < IMU_SAMPLES; i++) {
		Wire.beginTransmission(MPU);
		Wire.write(0x3B);
		Wire.endTransmission(false);
		Wire.requestFrom(MPU, 6, true);
		
		float accX = (Wire.read() << 8 | Wire.read()) / 16384.0;
		float accY = (Wire.read() << 8 | Wire.read()) / 16384.0;
		float accZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

		// Sum all readings
		accErrorX = accErrorX + ((atan((accY) / sqrt(pow((accX), 2) + pow((accZ), 2))) * 180 / PI));
		accErrorY = accErrorY + ((atan(-1 * (accX) / sqrt(pow((accY), 2) + pow((accZ), 2))) * 180 / PI));
	}

	//Divide the sum by IMU_SAMPLES to get the error value
	accErrorX = accErrorX / IMU_SAMPLES;
	accErrorY = accErrorY / IMU_SAMPLES;

	// Read gyro values IMU_SAMPLES times
	for (int i = 0; i < IMU_SAMPLES; i++) {
		Wire.beginTransmission(MPU);
		Wire.write(0x43);
		Wire.endTransmission(false);
		Wire.requestFrom(MPU, 6, true);

		float gyroX = Wire.read() << 8 | Wire.read();
		float gyroY = Wire.read() << 8 | Wire.read();
		float gyroZ = Wire.read() << 8 | Wire.read();

		// Sum all readings
		gyroErrorX = gyroErrorX + (gyroX / 131.0);
		gyroErrorY = gyroErrorY + (gyroY / 131.0);
		gyroErrorZ = gyroErrorZ + (gyroZ / 131.0);
	}
	//Divide the sum by IMU_SAMPLES to get the error value
	gyroErrorX = gyroErrorX / IMU_SAMPLES;
	gyroErrorY = gyroErrorY / IMU_SAMPLES;
	gyroErrorZ = gyroErrorZ / IMU_SAMPLES;
}

void initialise_imu() {
	//IMU Comms Initialisation
	Wire.begin();                      // Initialize comunication
	Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
	Wire.write(0x6B);                  // Talk to the register 6B
	Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
	Wire.endTransmission(true);        //end the transmission

	initialise_imu_error();
}

void read_imu(struct IMUData *imuData) {
	// === Read acceleromter data === //
	Wire.beginTransmission(MPU);
	Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
	Wire.endTransmission(false);
	Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers

	//For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
	imuData->accX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
	imuData->accY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
	imuData->accZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value

	// Calculating Roll and Pitch from the accelerometer data
	imuData->accAngleX = (atan(imuData->accY / sqrt(pow(imuData->accX, 2) + pow(imuData->accZ, 2))) * 180 / PI) + accErrorX; // accErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
	imuData->accAngleY = (atan(-1 * imuData->accX / sqrt(pow(imuData->accY, 2) + pow(imuData->accZ, 2))) * 180 / PI) + accErrorY; // accErrorY ~(-1.58)

	// === Read gyroscope data === //
	previousTime = currentTime;        // Previous time is stored before the actual time read
	currentTime = millis();            // Current time actual time read
	elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds

	Wire.beginTransmission(MPU);
	Wire.write(0x43); // Gyro data first register address 0x43
	Wire.endTransmission(false);
	Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers

	imuData->gyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
	imuData->gyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
	imuData->gyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;

	// Correct the outputs with the calculated error values
	imuData->gyroX = imuData->gyroX - gyroErrorX; // gyroErrorX ~(-0.56)
	imuData->gyroY = imuData->gyroY - gyroErrorY; // gyroErrorY ~(2)
	imuData->gyroZ = imuData->gyroZ - gyroErrorZ; // gyroErrorZ ~ (-0.8)

	// Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
	imuData->gyroAngleX = imuData->gyroAngleX + imuData->gyroX * elapsedTime; // deg/s * s = deg
	imuData->gyroAngleY = imuData->gyroAngleY + imuData->gyroY * elapsedTime;
	imuData->yaw =  imuData->yaw + imuData->gyroZ * elapsedTime;

	// Complementary filter - combine acceleromter and gyro angle values
	imuData->roll = 0.95 * imuData->gyroAngleX + 0.05 * imuData->accAngleX;
	imuData->pitch = 0.95 * imuData->gyroAngleY + 0.05 * imuData->accAngleY;
}
