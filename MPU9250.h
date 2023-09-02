/*
 * MPU9250.h
 *
 *  Created on: Aug 24, 2023
 *      Author: Khayyir M
 */
#include "main.h"
#include "stdio.h"
#ifndef INC_MPU9250_H_
#define INC_MPU9250_H_


#define GYRO_CONFIG_REG 27
#define ACCEL_CONFIG_REG 28
#define PWR_MGMT1_REG 107
#define I2C_MSTR_CTRL_REG 36
#define USR_CTRL_REG 106

#define GYRO_FS_250 0
#define GYRO_FS_500 0x08
#define GYRO_FS_1000 0x10
#define GYRO_FS_2000 0x18

#define ACCEL_FS_2G 0
#define ACCEL_FS_4G 0x08
#define ACCEL_FS_8G 0x10
#define ACCEL_FS_16G 0x18

#define I2C_MSTR_CTRL_I2CCLK_400 0x0d
#define USR_CTRL_REG_I2CEN 0x20

#define PWR_MGMT1_PLL_EXSLP 0x01

#define I2C_SLV0_ADDR_REG 37
#define I2C_SLV0_REG_REG 38
#define I2C_SLV0_CTRL_REG 39
#define I2C_SLV0_DATA_REG 99

#define I2C_SLV0_CTRL_EN 0x80
#define EXT_SENS_DATA_00_REG 49
#define AK8963_ADDRESS 0x0C
#define AK8963_CTRL_REG 0x0a
#define PWR_DWN_MAGNETOMETER 0x00
#define FUSE_ROM_MODES 0x0F
#define AK8963_SEN_ONSET_REG 0x10
#define AK8963_CONT_MEAS2 0x16
#define AK8963_DATA_ONSET_REG 0x03
#define ONSET_DATA_REG 59
typedef struct{
	int16_t x_accel;
	int16_t y_accel;
	int16_t z_accel;
	int16_t x_gyro;
	int16_t y_gyro;
	int16_t z_gyro;
	int16_t x_mag;
	int16_t y_mag;
	int16_t z_mag;
}mpu9250_data;
//static void activate_mpu9250();
//static void deactivate_mpu9250();
void MPU9250_Init();


void mpu9250_write_reg(uint8_t reg, uint8_t data);
void mpu9250_read_reg(uint8_t reg, uint8_t *data, uint8_t len);
void mpu9250_read_sensor(mpu9250_data *data_imu);

#endif /* INC_MPU9250_H_ */
