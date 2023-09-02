/*
 * MPU9250.c
 *
 *  Created on: Aug 24, 2023
 *      Author: Khayyir M
 */
#include "MPU9250.h"
#include "main.h"
#include "spi.h"

extern SPI_HandleTypeDef hspi1;
static mag_adjust[3];
static void activate_mpu9250() {
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
}
static void deactivate_mpu9250() {
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}
void MPU9250_Init(){
	uint8_t temp_data[7];
	mpu9250_write_reg(GYRO_CONFIG_REG, GYRO_FS_500); //gyro config
	mpu9250_write_reg(ACCEL_CONFIG_REG, ACCEL_FS_4G); //accel config
	mpu9250_write_reg(PWR_MGMT1_REG, PWR_MGMT1_PLL_EXSLP); //exit sleep mode
	//Enable I2C master setting 400KHz
	mpu9250_write_reg(USR_CTRL_REG, USR_CTRL_REG_I2CEN);
	mpu9250_write_reg(I2C_MSTR_CTRL_REG, I2C_MSTR_CTRL_I2CCLK_400);
	ak8963_write_reg(AK8963_CTRL_REG, PWR_DWN_MAGNETOMETER);
	HAL_Delay(10);
	ak8963_write_reg(AK8963_CTRL_REG, FUSE_ROM_MODES);
	HAL_Delay(10);
	ak8963_write_reg(AK8963_SEN_ONSET_REG, mag_adjust, 3);
	ak8963_write_reg(AK8963_CTRL_REG, PWR_DWN_MAGNETOMETER);
	HAL_Delay(10);
	ak8963_write_reg(AK8963_CTRL_REG, AK8963_CONT_MEAS2);
	HAL_Delay(10);
	ak8963_read_reg(AK8963_DATA_ONSET_REG, temp_data, 7);
	HAL_Delay(10);
}

void ak8963_write_reg(uint8_t reg, uint8_t data) {
	mpu9250_write_reg(I2C_SLV0_ADDR_REG, AK8963_ADDRESS);
	mpu9250_write_reg(I2C_SLV0_REG_REG, reg);
	mpu9250_write_reg(I2C_SLV0_DATA_REG, data);
	mpu9250_write_reg(I2C_SLV0_CTRL_REG, (I2C_SLV0_CTRL_EN|0x01));
}

void ak8963_read_reg(uint8_t start_reg, uint8_t *data, uint8_t len) {
	mpu9250_write_reg(I2C_SLV0_ADDR_REG, 0x80|AK8963_ADDRESS);
	mpu9250_write_reg(I2C_SLV0_REG_REG, start_reg);
	mpu9250_write_reg(I2C_SLV0_CTRL_REG, (I2C_SLV0_CTRL_EN|len));
	HAL_Delay(10);
	mpu9250_read_reg(EXT_SENS_DATA_00_REG, data, len);

}

void mpu9250_read_sensor(mpu9250_data *data_imu) {
	uint8_t data[20];
	activate_mpu9250();
	uint8_t temp_data = 0x80|ONSET_DATA_REG;
	HAL_SPI_Transmit(&hspi1, &temp_data, 1, 100);
	HAL_SPI_Receive(&hspi1, data, 20, 100);
	deactivate_mpu9250();

	data_imu -> x_accel = ((int16_t)data[0]<<8)+data[1];
	data_imu -> y_accel = ((int16_t)data[2]<<8)+data[3];
	data_imu -> z_accel = ((int16_t)data[4]<<8)+data[5];

	data_imu -> x_gyro = ((int16_t)data[8]<<8)+data[9];
	data_imu -> y_gyro = ((int16_t)data[10]<<8)+data[11];
	data_imu -> z_gyro = ((int16_t)data[12]<<8)+data[13];

	data_imu -> x_mag = ((int16_t)data[15]<<8)+data[14];
	data_imu -> y_mag = ((int16_t)data[17]<<8)+data[16];
	data_imu -> z_mag = ((int16_t)data[19]<<8)+data[18];
//	printf("accelerometer : %d, %d, and %d \n", data_imu -> x_accel, data_imu -> y_accel, data_imu -> z_accel);
//	printf("gyroscope : %d, %d, and %d \n", data_imu -> x_gyro, data_imu -> y_gyro, data_imu -> z_gyro);
	printf("magnetometer : %d, %d, and %d \n", data_imu -> x_mag, data_imu -> y_mag, data_imu -> z_mag);
}
void mpu9250_write_reg(uint8_t reg, uint8_t data) {
	activate_mpu9250();
	HAL_SPI_Transmit(&hspi1, &reg, 1, 100);
	HAL_SPI_Receive(&hspi1, &data, 1, 100);
	deactivate_mpu9250();
}

void mpu9250_read_reg(uint8_t reg, uint8_t*data, uint8_t len) {
	uint8_t tempdata = 0x80|reg;
	activate_mpu9250();
	HAL_SPI_Transmit(&hspi1, &tempdata, 1, 100);
	HAL_SPI_Receive(&hspi1, data, len, 100);
	deactivate_mpu9250();
}

