#ifndef _MPU9250_H__
#define _MPU9250_H__

#include "stm32f10x.h"
#include "Global.h"
#define XBM_R2D    57.29577951f
typedef struct mpu_data
{
	signed short axc_x;
	signed short axc_y;
	signed short axc_z;
	signed short temp;
	signed short gyro_x;
	signed short gyro_y;
	signed short gyro_z;
	// AK8975
	signed short mag_x;
	signed short mag_y;
	signed short mag_z;
	
}MPU_DATA ;

extern MPU_DATA mpu_data_orign;
extern float mag_param[3][2];



extern void init_mpu9250(void);
extern void get_mpu9250_dat(void);
void mpu_data_prepare(void);
void sensor_process(void);

extern float Angle[3];

#define ADDR_MPU9250				0xD0

#define	SMPLRT_DIV					0x19	//陀螺仪采样率,典型值:0x07(125Hz)
#define	CONFIG							0x1A	//低通滤波频率,典型值:0x06(5Hz)
#define	GYRO_CONFIG					0x1B	//陀螺仪自检及测量范围,典型值:0x18(不自检,2000deg/s)
#define	ACCEL_CONFIG	      0x1C	//加速度计自检、测量范围及高通滤波频率，典型值:0x01(不自检,2G,5Hz)
#define	ACCEL_XOUT_H	      0x3B
#define	ACCEL_XOUT_L	      0x3C
#define	ACCEL_YOUT_H	      0x3D
#define	ACCEL_YOUT_L	      0x3E
#define	ACCEL_ZOUT_H	      0x3F
#define	ACCEL_ZOUT_L	      0x40
#define	TEMP_OUT_H					0x41
#define	TEMP_OUT_L					0x42
#define	GYRO_XOUT_H					0x43
#define	GYRO_XOUT_L					0x44
#define	GYRO_YOUT_H					0x45
#define	GYRO_YOUT_L					0x46
#define	GYRO_ZOUT_H					0x47
#define	GYRO_ZOUT_L					0x48
#define	PWR_MGMT_1					0x6B	//电源管理,典型值:0x00(正常启用)
#define	WHO_AM_I	        	0x75	//IIC地址寄存器(默认数值0x68,只读)


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                IIC Interface                               //
//      SCL --> PB6                                                           //
//      SDA --> PB7                                                           //
////////////////////////////////////////////////////////////////////////////////
/*--------------- define ---------------*/



#define SDA_OUT		IIC_SCL_OUT()
#define SDA_IN		IIC_SDA_IN()

#define SDA_HIGH	GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define SDA_LOW		GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define SCL_HIGH	GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define SCL_LOW		GPIO_ResetBits(GPIOB, GPIO_Pin_10)

#define READ_SDA	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

#define _ACK     0
#define _NACK    1

//--------------------------------------
extern void MPU9250_iic_init(void);

extern void iic_delay(void) ;
extern void iic_start(void) ;
extern void iic_stop(void) ;
extern void iic_send_ack(unsigned char ack) ;
extern unsigned char iic_recv_ack(void) ;
extern void iic_send_byte(unsigned char dat) ;
extern unsigned char iic_recv_byte(void) ;

extern unsigned char iic_read(unsigned char dev,
               unsigned char reg,
               unsigned char length,
               unsigned char * data);

extern unsigned char iic_write(unsigned char dev,
                        unsigned char reg,
                        unsigned char len,
                        unsigned char * dat);
												
//
												


void IIC_SCL_OUT(void);
void IIC_SDA_IN(void);
float xbm_atan(float x);
float xbm_atan2(float x, float y);
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                SPI Interface                               //
//      SCL --> PB21                                                          //
//      SDA --> PB22                                                          //
////////////////////////////////////////////////////////////////////////////////




#endif





