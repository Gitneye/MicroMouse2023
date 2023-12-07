#ifndef _IMU_H
#define _IMU_H

#define MPU 0x68 // MPU6050 I2C address

struct IMUData {
	float accX;
	float accY;
	float accZ; 

	float gyroX;
	float gyroY;
	float gyroZ;

	float accAngleX;
	float accAngleY;

	float gyroAngleX;
	float gyroAngleY;
	float gyroAngleZ;

	float roll;
	float pitch;
	float yaw;
};

void initialise_imu();

void read_imu(struct IMUData *imuData);

#endif //_IMU_H