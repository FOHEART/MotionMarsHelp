/************************************************************
 * Copyright (C), 2015-2019, BEIJING FOHEART Tech. Co., Ltd.
 * FileName:MotionMars_Q1_Arduino_MEGA2560.ino
 * Author:MaoxiaoHu
 * Date:01/09/2019
 * Description:MotionMars Q1
 Q1使用出厂默认配置：
  波特率：115200bps
  发送数据频率：200Hz
  发送数据类型：四元数+欧拉角+标定后的加速度值+标定后的角速度值+标定后的磁力计值。

 使用Arduino MEGA 2560接收并解析来自MotionMars Q1的姿态数据包。
 （1）连接Q1的TTL TX线（2脚）到MEGA2560的RX1 19脚。
 （2）连接Q1的TTL RX线（3脚）到MEGA2560的TX1 18脚。
 （3）连接Q1的5V电源（1脚）与GND（4脚）到MEGA2560的5V电源与GND。
 （4）打开Arduino IDE的工具->串口监视器查看解析完成的数据包。
 * Version:
 * Function List:
 *     1. -------
 * History:
 *     <author>  <time>   <version >   <desc>
 *     MaoxiaoHu 19/01/10     1.0   build this moudle
 ***********************************************************/
#define CIRCLEBUFF_LEN ( 42 )

#pragma pack(1)
typedef struct {
	uint8_t addr;
	uint8_t code;

	/*[0, 4294967295]‬之间增加，达到最大值后从0开始重新计数，
	   注意若以200Hz上传数据，此值约248天溢出一次*/
	uint32_t frameNum;

	/*用位表示是否包含数据项*/
	union {
		struct {
			uint16_t	quatFlag : 1;   /*Bit:0*/
			uint16_t	eulerFlag : 1;  /*Bit:1*/
			uint16_t	accelFlag : 1;  /*Bit:2*/
			uint16_t	gyroFlag : 1;   /*Bit:3*/
			uint16_t	magFlag : 1;    /*Bit:4*/
			uint16_t	resvFlag : 11;  /*Bit:5~11*/
		};
		uint16_t dataSendFlag;
	};
}UDIMU_UART_HDR_ND2PC;

/*根据Flag的不同可至多发送16种不同数据*/
typedef struct
{
	uint8_t addr;
	uint8_t code;


	/*[0, 4294967295]‬之间增加，达到最大值后从0开始重新计数，
	   注意若以200Hz上传数据，此值约248天溢出一次*/
	uint32_t frameNum;

	/*用位表示是否包含数据项*/
	union {
		struct {
			uint16_t	quatFlag : 1;   /*Bit:0*/
			uint16_t	eulerFlag : 1;  /*Bit:1*/
			uint16_t	accelFlag : 1;  /*Bit:2*/
			uint16_t	gyroFlag : 1;   /*Bit:3*/
			uint16_t	magFlag : 1;    /*Bit:4*/
			uint16_t	resvFlag : 11;  /*Bit:5~11*/
		};
		uint16_t dataSendFlag;
	};

	int16_t		quat[4];
	int16_t		euler[3];
	int16_t		accel[3];
	int16_t		gyro[3];
	int16_t		mag[3];
	uint16_t	crc16;
}UDIMU_UART_ND2PC;

#pragma pack()

UDIMU_UART_ND2PC	nd2pc;
uint8_t				globalBuff[CIRCLEBUFF_LEN];


/***********************************************************
* Function:        Function Name
* Description:
* Input:
* Input:
* Output:
* Return:
* Others:          Other Description.
***********************************************************/
void setup( )
{
	// put your setup code here, to run once:
	Serial.begin( 115200 ); 
	Serial1.begin( 115200 );
}

/***********************************************************
* Function:        loop
* Description: Arduino 程序主循环
* Input:
* Input:
* Output:
* Return:
* Others:
   （1）监听串口1，将收到的数据放入循环缓冲区。
   （2）寻找姿态数据包的头部 也就是连续的两个字节0x01 0x24。
***********************************************************/
void loop( )
{
	if( Serial1.available( ) > 0 )
	{
		int numdata = Serial1.available( );
		if( numdata > 1 )
		{
			byte b[64];
			Serial1.readBytes( b, numdata );
			//Serial.println( numdata );
		}else
		{
			byte b[64];
			Serial1.readBytes( b, numdata );

			for( int i = 1; i < CIRCLEBUFF_LEN; i++ )
			{
				globalBuff[i - 1] = globalBuff[i];
			}
			globalBuff[CIRCLEBUFF_LEN - 1] = b[0];

			if( globalBuff[0] == 0x01 &&
			    globalBuff[1] == 0x24
			    )
			{
				ParsePckage( globalBuff, CIRCLEBUFF_LEN );
			}
		}
	}
}

/***********************************************************
* Function:        Function Name
* Description:
* Input:
* Input:
* Output:
* Return:
* Others:这里的打印函数输出非常占用Arduino时间，打印的数据
         要比实际收到的数据少，实际收到的数据速率为200Hz!
         可适当减少打印内容。
***********************************************************/
void ParsePckage( uint8_t* buff, uint16_t size )
{
	uint16_t	crc16		   = buff[size - 2] | ( buff[size - 1] << 8 );
	uint16_t	crc16Expect	   = CRC16( buff, size - 2 );

	if( crc16 != crc16Expect )
	{
		/*CRC16校验错误，不执行后续代码*/
		return;
	}

	int						frontptr = 0;
	UDIMU_UART_HDR_ND2PC	hdr;
	for( int i = 0; i < sizeof( hdr ); i++ )
	{
		*( (uint8_t *)( &hdr ) + i ) = buff[i];
	}
	/*打印帧序号，如不需要可去掉*/
	Serial.print( hdr.frameNum ); Serial.print( " " );
	frontptr += sizeof( hdr );


	/*！最终数据！
	   定义浮点数数据，由整型格式转换出浮点数表示的数据*/
	float	quat_f[4]  = { 0.0f, 0.0f, 0.0f, 0.0f }; /*w,x,y,z*/
	float	euler_f[3] = { 0.0f, 0.0f, 0.0f };
	float	accel_f[3] = { 0.0f, 0.0f, 0.0f };
	float	gyro_f[3]  = { 0.0f, 0.0f, 0.0f };
	float	mag_f[3]   = { 0.0f, 0.0f, 0.0f };

	if( hdr.quatFlag )
	{
		int16_t quat[4] = { 0, 0, 0, 0 };
		memcpy( quat, &buff[frontptr], sizeof( quat ) );
		frontptr  += sizeof( quat );
		quat_f[0]  = (float)quat[0] / ( 1 << 14 );
		quat_f[1]  = (float)quat[1] / ( 1 << 14 );
		quat_f[2]  = (float)quat[2] / ( 1 << 14 );
		quat_f[3]  = (float)quat[3] / ( 1 << 14 );
		/*打印四元数，如不需要可去掉*/
		Serial.print( "Quat:" ); Serial.print( quat_f[0], 6 ); Serial.print( " " ); Serial.print( quat_f[1], 6 ); Serial.print( " " );
		Serial.print( quat_f[2], 6 ); Serial.print( " " ); Serial.print( quat_f[3], 6 ); Serial.print( " " );
	}

	if( hdr.eulerFlag )
	{
		int16_t euler[3] = { 0, 0, 0 };
		memcpy( euler, &buff[frontptr], sizeof( euler ) );
		frontptr  += sizeof( euler );
		euler_f[0] = (float)euler[0] / ( 1 << 7 );
		euler_f[1] = (float)euler[1] / ( 1 << 7 );
		euler_f[2] = (float)euler[2] / ( 1 << 7 );

		/*打印欧拉角，如不需要可去掉*/
		Serial.print( "Euler:" ); Serial.print( euler_f[0], 6 ); Serial.print( " " );
		Serial.print( euler_f[1], 6 ); Serial.print( " " ); Serial.print( euler_f[2], 6 ); Serial.print( " " );
	}

	if( hdr.accelFlag )
	{
		int16_t accel[3] = { 0, 0, 0 };
		memcpy( accel, &buff[frontptr], sizeof( accel ) );
		frontptr  += sizeof( accel );
		accel_f[0] = (float)accel[0] / 2048.0f;
		accel_f[1] = (float)accel[1] / 2048.0f;
		accel_f[2] = (float)accel[2] / 2048.0f;
		/*打印加速度，如不需要可去掉*/
		Serial.print( "Accel:" ); Serial.print( accel_f[0], 6 ); Serial.print( " " );
		Serial.print( accel_f[1], 6 ); Serial.print( " " ); Serial.print( accel_f[2], 6 ); Serial.print( " " );
	}

	if( hdr.gyroFlag )
	{
		int16_t gyro[3] = { 0, 0, 0 };
		memcpy( gyro, &buff[frontptr], sizeof( gyro ) );
		frontptr  += sizeof( gyro );
		gyro_f[0]  = (float)gyro[0] / 16.4f;
		gyro_f[1]  = (float)gyro[1] / 16.4f;
		gyro_f[2]  = (float)gyro[2] / 16.4f;
		/*打印角速度，如不需要可去掉*/
		Serial.print( "Gyro:" ); Serial.print( gyro_f[0], 6 ); Serial.print( " " );
		Serial.print( gyro_f[1], 6 ); Serial.print( " " ); Serial.print( gyro_f[2], 6 ); Serial.print( " " );
	}

	if( hdr.magFlag )
	{
		int16_t mag[3] = { 0, 0, 0 };
		memcpy( mag, &buff[frontptr], sizeof( mag ) );
		frontptr  += sizeof( mag );
		mag_f[0]   = (float)mag[0] / 120.0f;
		mag_f[1]   = (float)mag[1] / 120.0f;
		mag_f[2]   = (float)mag[2] / 120.0f;
		/*打印磁力值，如不需要可去掉*/
		Serial.print( "Mag:" ); Serial.print( mag_f[0], 6 ); Serial.print( " " );
		Serial.print( mag_f[1], 6 ); Serial.print( " " ); Serial.print( mag_f[2], 6 ); Serial.print( " " );
	}
	Serial.println( );
	/*在此可插入回调函数，将解析完成的单项或多项数据分发出去。*/
}

static const uint8_t	CRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40
};

static const uint8_t	CRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
	0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
	0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
	0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
	0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
	0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
	0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
	0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
	0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
	0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
	0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
	0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
	0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
	0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
	0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
	0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
	0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
	0x41, 0x81, 0x80, 0x40
};


/***********************************************************
* Function:        CRC16
* Description:    Return the CRC16 result of data stream
* Input:            pucFrame:Data stream repre by hex16
* Input:            usLen: Total length of data stream
* Output:
* Return:          CRC16 Results
* Others:          Other Description.
***********************************************************/
uint16_t CRC16( uint8_t * pucFrame, uint16_t usLen )
{
	uint8_t ucCRCHi	   = 0xFF;
	uint8_t ucCRCLo	   = 0xFF;
	int		iIndex;

	while( usLen-- )
	{
		iIndex	   = ucCRCLo ^ *( pucFrame++ );
		ucCRCLo	   = ( uint8_t )( ucCRCHi ^ CRCHi[iIndex] );
		ucCRCHi	   = CRCLo[iIndex];
	}
	return ( uint16_t )( ucCRCHi << 8 | ucCRCLo );
}

/************************************** The End Of File **************************************/
