#include "mpu9250.h"
//#include "Global.h"
//#include "timer.h"
//#include "sensorprocess.h"

unsigned char MPU_DAT_BUF[14];
MPU_DATA mpu_data_orign;

int flag_cali_mag = 0;
unsigned char AK8975_ADDR = 0x18;

unsigned char ak87 = 0;


#define magFilterNum 8
float mag_raw_data[3];
float magData[3];
float mag_param[3][2];

#define MAG_ADJ_MAX 3000

int magCaliNum = 0;
float magMax[3] = {-1000000,-1000000,-1000000};
float magMin[3] = {1000000,1000000,1000000};


float Angle[3] = {0,0,0};

void sensorCalibrate(void);



unsigned char mpu9250_write_reg(unsigned char reg,
                                unsigned char length,
                                unsigned char * data)
{
    return iic_write(ADDR_MPU9250, reg, length, data);
}

unsigned char mpu9250_read_reg(unsigned char reg,
                                unsigned char length,
                                unsigned char * dat)
{
    return iic_read(ADDR_MPU9250, reg, length, dat);
}


void init_mpu9250(void)
{
	unsigned char dat;
	MPU9250_iic_init();
	Delay_ms(100) ;
	dat = 0x80 ;
	iic_write(ADDR_MPU9250, PWR_MGMT_1,    1,&dat) ;
	Delay_ms(50);
	dat = 0x00 ;
	iic_write(ADDR_MPU9250, PWR_MGMT_1,    1,&dat) ;
	Delay_ms(50);
	iic_write(ADDR_MPU9250, PWR_MGMT_1,    1,&dat) ;
	dat = 0x07 ;
	iic_write(ADDR_MPU9250, SMPLRT_DIV,    1,&dat) ;
	dat = 0x86 ;
	iic_write(ADDR_MPU9250, CONFIG,        1,&dat) ;
	dat = 0x10 ;								// 陀螺仪量程 ±1000dps
	iic_write(ADDR_MPU9250, GYRO_CONFIG,   1,&dat) ;
	dat = 0x10 ;								// 加速度计量程 ±8g
	iic_write(ADDR_MPU9250, ACCEL_CONFIG,  1,&dat) ;
	
	/////////////////////////////////////////////////
	// 
	dat = 0x00;
	iic_write(ADDR_MPU9250, 0x6A,  1, &dat) ;	
	dat = 0x02;
	iic_write(ADDR_MPU9250, 0x37,  1, &dat) ;

	Delay_ms(50);
	dat = 0x00;
	iic_write(AK8975_ADDR, 0x0A,  1, &dat) ;
	Delay_ms(50);
	dat = 0x01;
	iic_write(AK8975_ADDR, 0x0A,  1, &dat) ;
	
	while(dat != 0x48)
	{
		iic_read(AK8975_ADDR, 0x00, 1, &dat);
		Delay_ms(1);
	}
	
	
	flag_cali_mag = 0;
	
	mag_param[0][0] = -6;
	mag_param[0][1] = 0.025641026;
	
	mag_param[1][0] = 35;
	mag_param[1][1] = 0.166666672;
	
	mag_param[2][0] = -53;
	mag_param[2][1] = 0.024390243;
	
	
	
	
}



void get_mpu9250_dat(void)
{
  uint8_t dat = 0;
	mpu9250_read_reg(ACCEL_XOUT_H, 14, MPU_DAT_BUF);
	mpu_data_orign.axc_x  = (short)((MPU_DAT_BUF[0]<<8) + MPU_DAT_BUF[1]);
	mpu_data_orign.axc_y  = (short)((MPU_DAT_BUF[2]<<8) + MPU_DAT_BUF[3]);
	mpu_data_orign.axc_z  = (short)((MPU_DAT_BUF[4]<<8) + MPU_DAT_BUF[5]);
	mpu_data_orign.temp   = (short)((MPU_DAT_BUF[6]<<8) + MPU_DAT_BUF[7]);
	
  mpu_data_orign.gyro_x = (short)((MPU_DAT_BUF[8]  << 8) + MPU_DAT_BUF[9]  );
	mpu_data_orign.gyro_y = (short)((MPU_DAT_BUF[10] << 8) + MPU_DAT_BUF[11] );
	mpu_data_orign.gyro_z = (short)((MPU_DAT_BUF[12] << 8) + MPU_DAT_BUF[13] );
	
	//dat = 0x01;
	//iic_write(AK8975_ADDR, 0x0A,  1, &dat) ;

	
	iic_read(AK8975_ADDR,0x03, 6, MPU_DAT_BUF);
	
	mpu_data_orign.mag_x = (short)((MPU_DAT_BUF[1]<<8) + MPU_DAT_BUF[0]);
	mpu_data_orign.mag_y = (short)((MPU_DAT_BUF[3]<<8) + MPU_DAT_BUF[2]);
	mpu_data_orign.mag_z = (short)((MPU_DAT_BUF[5]<<8) + MPU_DAT_BUF[4]);		
	
	dat = 0x01;
	iic_write(AK8975_ADDR, 0x0A,  1, &dat);
	
	//Ang = -xbm_atan2(mpu_data_orign.mag_x,-mpu_data_orign.mag_y) * XBM_R2D;
	//Ang = atan2(mpu_data_orign.mag_y,mpu_data_orign.mag_x )* XBM_R2D;
}



void mpu_data_prepare(void)
{
	static float magFilter[3][magFilterNum];
	static float magSum[3] ;
	static int magFilterCnt = 0;
	static unsigned char magFilterIsFull = 0;
	unsigned char i;
	
	
	get_mpu9250_dat();
	
	sensorCalibrate();
	
	mag_raw_data[0] = (mpu_data_orign.mag_x - mag_param[0][0]) * mag_param[0][1];
	mag_raw_data[1] = (mpu_data_orign.mag_y - mag_param[1][0]) * mag_param[1][1];
	mag_raw_data[2] = (mpu_data_orign.mag_z - mag_param[2][0]) * mag_param[2][1];
	
	
	
	
	if(magFilterCnt >= magFilterNum)
	{
		magFilterCnt = 0;
		magFilterIsFull = 1;
	}
	if(magFilterIsFull)
	{
		for(i = 0; i < 3; i ++)
		{
			magSum[i] = magSum[i] - magFilter[i][magFilterCnt];
			magFilter[i][magFilterCnt] = mag_raw_data[i];
			magSum[i] += magFilter[i][magFilterCnt];
			magData[i] = magSum[i] / magFilterNum;	
		}
	}
	else
	{
		
		for(i = 0; i < 3; i ++)
		{
			magFilter[i][magFilterCnt] = mag_raw_data[i];
			magSum[i] += magFilter[i][magFilterCnt];
			magData[i] = magSum[i]/(magFilterCnt + 1);
		}
		
	}
	
	magFilterCnt ++;
	
}

void sensor_process(void)
{
	if(flag_cali_mag != 1)
	{
		Angle[2] = xbm_atan2(magData[2],magData[0]) * XBM_R2D;
	}
	//SendData( (Angle[2] + 180 )/2);
	//USART_SendData(UART4, (unsigned char)((Angle[2] + 180)/2));
	DT_SetMagData();
}


void sensorCalibrate(void)
{

	int i ;
	
	if(local_data.flag_mag_cali == 1)
	{

		if(magMax[0] < mpu_data_orign.mag_x)
		{
			magMax[0] = mpu_data_orign.mag_x;
		}
		
		if(magMax[1] < mpu_data_orign.mag_y)
		{
			magMax[1] = mpu_data_orign.mag_y;
		}
		
		if(magMax[2] < mpu_data_orign.mag_z)
		{
			magMax[2] = mpu_data_orign.mag_z;
		}
		
		if(magMin[0] > mpu_data_orign.mag_x)
		{
			magMin[0] = mpu_data_orign.mag_x;
		}
		
		if(magMin[1] > mpu_data_orign.mag_y)
		{
			magMin[1] = mpu_data_orign.mag_y;
		}
		
		if(magMin[2] > mpu_data_orign.mag_z)
		{
			magMin[2] = mpu_data_orign.mag_z;
		}
		magCaliNum ++ ;
		if(magCaliNum >= MAG_ADJ_MAX)
		{
			magCaliNum = 0;
			local_data.flag_mag_cali = 0;
			
			 
			for(i = 0; i < 3; i ++)
			{
				mag_param[i][0] = (magMax[i] + magMin[i])/2;
				mag_param[i][1] = 2/(magMax[i] - magMin[i]);
			}
			
			//data_save();

		}
		
		 
		
	}
	else
	{
		
	}
	
}





////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                IIC Interface                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

unsigned char ack;

void MPU9250_iic_init(void)
{
	// SCL	PB6
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	//PG11 ??,??IIC?SCL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//PG12 ??,??IIC?SDA;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;; 
	GPIO_Init(GPIOB,&GPIO_InitStructure);	 
	
}


void iic_delay(void)
{
	//delay_us(2);
	int i;
	for (i = 0; i < 40; i++)
	{
		;
	}

}
/**************************************
起始信号
**************************************/
void iic_start(void)
{
    SDA_HIGH ;
    SCL_HIGH ;      //?????
    iic_delay() ;    //??
    SDA_LOW ;       //?????
    iic_delay() ;    //??
    SCL_LOW ;       //?????
}
/**************************************
停止信号
**************************************/
void iic_stop(void)
{
    SDA_LOW ;
    SCL_HIGH ;
    iic_delay() ;
    SDA_HIGH ; 
    iic_delay() ; 
}
/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void iic_send_ack(unsigned char ack)
{
    if(ack)
    {
        SDA_HIGH ;
    }
    else
    {
        SDA_LOW ;
    }           
    SCL_HIGH ;       
    iic_delay() ;   
    SCL_LOW ;          
    iic_delay() ;   
}
/**************************************
接收应答信号
0为应答 1为应答失败
**************************************/
unsigned char iic_recv_ack(void)
{
    SDA_IN ;
    SCL_HIGH ;                     
    iic_delay() ;                 
    if(READ_SDA)  
    {
       ack = 1;
    }
    else
    {
        ack = 0;
    }
    SCL_LOW ;                       //?????
    SDA_OUT ;                       //??SDA???
    iic_delay() ;                    //??
    return ack ;

}
/**************************************
向IIC总线发送一个字节数据
**************************************/
void iic_send_byte(unsigned char dat)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
        if((dat << i) & 0x80)   //????
        {    
			SDA_HIGH ;          //??SDA??
        }
		else
		{
		    SDA_LOW ;
        }
		iic_delay() ;            //??
        SCL_HIGH ;              //?????
        iic_delay() ;            //??
        SCL_LOW ;               //?????
    }
    iic_recv_ack();
}
/**************************************
从IIC总线接收一个字节数据
**************************************/
unsigned char iic_recv_byte(void)
{
    unsigned char i,dat;
    SDA_HIGH ;                  //??????,??????
    iic_delay() ;                //??
    SDA_IN ;                    //??SDA???
    iic_delay() ;                //??
    for(i = 0; i < 8; i++)
    {
        dat <<= 1;              //????
        SCL_HIGH ;              //?????
        iic_delay() ;            //??
        if(READ_SDA)   			//??SDA????
            dat ++ ;
        SCL_LOW ;               //?????
        iic_delay() ;            //??
    }
    SDA_OUT ;                   //??SDA???
    return dat ;
}
/**************************************
I2C 接收数据
参数:	dev 器件地址, 
		reg 寄存器地址,
		length 接收字节长度, 
		data 数据地址
返回值: 0 if sucess
**************************************/
unsigned char iic_read(unsigned char dev,
               unsigned char reg,
               unsigned char length,
               unsigned char * data)
{
    unsigned char i ;
    iic_start() ;
    iic_send_byte(dev) ;
    iic_send_byte(reg) ;
    iic_start() ;
    iic_send_byte(dev + 1) ;
    for(i = 0; i < length; i ++)
    {
        data[i] = iic_recv_byte() ;
        if(i == length-1)
            iic_send_ack(_NACK) ;
        else
            iic_send_ack(_ACK) ;
    }
    iic_stop() ;
    return 0 ;
}

/**************************************
I2C 发送数据
参数:	dev 器件地址, 
		reg 寄存器地址,
		len 数据长度
		dat数据指针
返回值: 0 if sucess
**************************************/
unsigned char iic_write(unsigned char dev,
                        unsigned char reg,
                        unsigned char len,
                        unsigned char * dat)
{
    unsigned char i ;
    iic_start() ;
    iic_send_byte(dev) ;
    iic_send_byte(reg) ;
    for(i = 0; i < len; i ++)
    {
        iic_send_byte(dat[i]) ;
    }
    iic_stop() ;
    return 0 ;
}

void IIC_SCL_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	//PG11 输出，作为IIC的SCL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void IIC_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}


float xbm_atan(float x)
{
	float y;
	int invFlag = 0;
	if(x < 0.0f)
	{
		x = -x;
		invFlag = 1;
	}
	if(x < 1.0f)
	{
		y = (((-0.03825f*x*x + 0.14498f)*x*x - 0.32053f)*x*x + 0.99913f)*x;
	}else if(x < 3.0f)
	{
		y= ((0.03214001f*x - 0.28140609f)*x + 0.93934992f)*x + 0.09782516f;
	}else if(x < 20.0f)
	{
		y = ((((( -0.000000121964262f*x + 0.000009637007236f)*x - 0.000312307892459f)*x + 0.005353125313825f)*x
			-0.052153209590924f)*x + 0.287835099268528f)*x + 0.735982162169589f;
	}else if(x < 100.0f)
	{
		y = ((((( -0.000000000000992f*x + 0.000000000409265f)*x - 0.000000069742821f)*x + 0.000006342069404f)*x
			-0.000331071339679f)*x + 0.009885048057013f)*x + 1.414935610187774f;
	}else
	{
		y = 1.4915;
	}
	if(invFlag == 1)
	{
		return -y;
	}else
	{
		return y;
	}
}



float xbm_atan2(float x, float y)
{
  float temp = y/x;
  //??
  temp = xbm_atan(temp);
  //????
  if( (x < 0) && (y > 0) )
  {
    temp += PI;
  }else if( (x<0) && (y<0) )
  {
    temp += -PI;
  }
  return temp;  
}

