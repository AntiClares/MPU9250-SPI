# MPU9250-SPI

Ganti SPI1_CS_GPIO_Port di main.h
pinnya juga

paste di code begin 0
typedef struct {
	float yaw;
	float pitch;
	float roll;
}EulerAngles;
void estimate_angle(mpu9250_data imu_data, EulerAngles *euler_angles) {
	float ay_az_sqrt = sqrt((float)imu_data.y_accel * imu_data.y_accel +
			imu_data.z_accel * imu_data.z_accel);
	float ax_ay_az_sqrt = sqrt(ay_az_sqrt * ay_az_sqrt + (float)imu_data.x_accel * imu_data.x_accel);
	euler_angles -> pitch = atan2(imu_data.x_accel, ay_az_sqrt)*57.29;
	euler_angles -> roll = atan2(-imu_data.y_accel, -imu_data.z_accel)*57.29;
	float sinroll = ((float)-imu_data.y_accel)/ ay_az_sqrt;
	float cosroll = ((float)-imu_data.z_accel)/ ay_az_sqrt;
	float sinpitch = ((float)imu_data.x_accel)/ ax_ay_az_sqrt;
	float cospitch = (float)ay_az_sqrt / ax_ay_az_sqrt;

	euler_angles -> yaw = atan2((float)imu_data.z_mag * sinroll - (float)imu_data.y_mag * cosroll,
			(float)imu_data.x_mag * cospitch + sinpitch * ((float)imu_data.z_mag * cosroll +
			(float)imu_data.y_mag * sinroll))*57.29;

//	printf("angle : %f, %f, and %f \n", euler_angles -> yaw, euler_angles -> pitch, euler_angles -> roll);
}

paste ini di code begin 2
  MPU9250_Init();
  mpu9250_data mpu_data;
  EulerAngles euler_angles;
  float xyaw = euler_angles.yaw;
  float xpitch = euler_angles.pitch;
  float xroll = euler_angles.roll;

paste ini di while
	  mpu9250_read_sensor(&mpu_data);
	  estimate_angle(mpu_data, &euler_angles);
	  x_magnet = mpu_data.x_mag;
	  y_magnet = mpu_data.y_mag;
	  z_magnet = mpu_data.z_mag;
	  HAL_Delay(1000);
