#include "mpu6050.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "math.h"
#include "KalmanFilter.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32F103������ 
//MPU6050 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/4/18
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
 
//��ʼ��MPU6050
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Init(void)
{ 
	u8 res; 
	MPU_IIC_Init();//��ʼ��IIC����
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
    delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
	MPU_Set_Rate(50);						//���ò�����50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG); 
	if(res==MPU_ADDR)//����ID��ȷ
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
		MPU_Set_Rate(50);						//���ò�����Ϊ50Hz
 	}else return 1;
	return 0;
}
//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
}
//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}
//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}
//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}
//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((addr<<1)|0);//����������ַ+д����	
	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);	//��������
		if(MPU_IIC_Wait_Ack())		//�ȴ�ACK
		{
			MPU_IIC_Stop();	 
			return 1;		 
		}		
	}    
    MPU_IIC_Stop();	 
	return 0;	
} 
//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((addr<<1)|0);//����������ַ+д����	
	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|1);//����������ַ+������	
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	while(len)
	{
		if(len==1)*buf=MPU_IIC_Read_Byte(0);//������,����nACK 
		else *buf=MPU_IIC_Read_Byte(1);		//������,����ACK  
		len--;
		buf++; 
	}    
    MPU_IIC_Stop();	//����һ��ֹͣ���� 
	return 0;	
}
//IICдһ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
    MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����	
	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	MPU_IIC_Send_Byte(data);//��������
	if(MPU_IIC_Wait_Ack())	//�ȴ�ACK
	{
		MPU_IIC_Stop();	 
		return 1;		 
	}		 
    MPU_IIC_Stop();	 
	return 0;
}
//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
    MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����	
	MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//����������ַ+������	
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	res=MPU_IIC_Read_Byte(0);//��ȡ����,����nACK 
    MPU_IIC_Stop();			//����һ��ֹͣ���� 
	return res;		
}

int dt;
float f_dt;
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temp;					//�¶�	 
float a_z;
float angle_init[3];
float a[3];
short acc[4], gyr[4];
float q[4];
short usq[4];
short usq1[4]={0};
short usq2[10]={0};
short usq3[10]={0};
float g1[3] = {0};
float x_value[3] = {0};
float v_value[3] = {0};
float az_value = 0;
void mpu6050_deal()
{
		if(mpu_dmp_get_data(acc, gyr, &pitch,&roll,&yaw, q)==0)
		{ 
			/*
			Az  -45<PIT<45 -45<ROL<45
			-Az  135<PIT<180 || -180<PIT<-135  -45<ROL<45

			Ax   -90<ROL<-45
			-Ax  135<PIT<180 || -180<PIT<-135  45<ROL<90

			Ay  45<PIT<135 -45<ROL<45
			-Ay  -135<PIT<-45  -45<ROL<45
			*/
			int position = 0;
			static int position_old = 0;
			if((roll > -45 && roll < 45) && (pitch > -45 && pitch < 45))
			{
				position = 1;
			}else if(((roll > 135 && roll < 180) || (roll > -180 && roll < -135))&&(pitch > -45 && pitch < 45))
			{
				position = 2;
			}else if(pitch > -90 && pitch < -45)
			{
				position = 3;
			}else if(((roll > 135 && roll < 180) || (roll > -180 && roll < -135))&&(pitch > 45 && pitch < 90))
			{
				position = 4;
			}else if((roll > 45 && roll < 135) && (pitch > -45 && pitch < 45))
			{
				position = 5;
			}else if((roll > -135 && roll < -45) && (pitch > -45 && pitch < 45))
			{
				position = 6;
			}
			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			// printf("temp: %d", temp);
			//  printf("acc: %d,%d,%d\n",aacx,aacy,aacz);
			// printf("gro: %d,%d,%d\r\n",gyrox,gyroy,gyroz);
			
			a[0] = aacx/16384.*9.8;
			a[1] = aacy/16384.*9.8;
			a[2] = aacz/16384.*9.8;
			float T[3] = {0};
			float sum_tmp = sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);
			for(int i = 0; i < 4; i++)
			{
				q[i] /= sum_tmp;
			}
			usq[0] = q[0] * 10000;
			usq[1] = q[1] * 10000;
			usq[2] = q[2] * 10000;
			usq[3] = q[3] * 10000;
			
			usq1[0] = roll * 100;
			usq1[1] = pitch * 100;
			usq1[2] = yaw * 100;




			g1[0] = 2 *9.8* (q[1]*q[3]-q[0]*q[2]);
			g1[1] = 2 *9.8* (q[0]*q[1]+q[2]*q[3]);
			g1[2] = 9.8* (q[0]*q[0]-q[1]*q[1]-q[2]*q[2]+q[3]*q[3]);
			float a_tmp[3] = {0},af_tmp[4] = {0};

			
			for(int i = 0; i<3; i++)
			{
				usq2[i] = (a[i] - g1[i]) *100;
				a_tmp[i] = (float)usq2[i];
			}
			extern float a_zero[3] ;
			filter_FIR(a_tmp, af_tmp);
			//af_tmp[3] = sqrt(af_tmp[0] * af_tmp[0] + af_tmp[1] * af_tmp[1] + af_tmp[2] * af_tmp[2]);
			if(position_old != position)
			{
				position_old = position;
				v_value[0] = 0;
			}
			switch(position)
			{
				case 1:
					af_tmp[3] = af_tmp[2];
					break;
				case 2:
					af_tmp[3] = -af_tmp[2];
					break;
				case 3:
					af_tmp[3] = af_tmp[0];
					break;
				case 4:
					af_tmp[3] = -af_tmp[0];
					break;
				case 5:
					af_tmp[3] = af_tmp[1];
					break;
				case 6:
					af_tmp[3] = -af_tmp[1];
					break;
				default:
					break;
			}
			x_deal(&af_tmp[3], x_value, v_value);
			for(int i = 0; i<3; i++)
			{
				usq2[i+3] = a_tmp[i];
			}
			for(int i = 0; i<3; i++)
			{
				usq2[i+6] = af_tmp[i];
			}			
			usq2[9] = af_tmp[3];
			usq3[0] = x_value[0]*100;
			usq3[1] = v_value[0]*100;
			
			f_dt = x_value[0];
			dt = f_dt;
			//usq2[4] = KalmanFilter(&kfp,(float)usq2[0]);


			//usq2[5] = KalmanFilter(&kfp,(float)usq2[3]);
			//KalmanFilter(usq2[0], &usq2[3]);
			// T[0] = 2 * (q[1]*q[3]+q[0]*q[2]);
			// T[1] = 2 * (q[2]*q[3]-q[0]*q[1]);
			// T[2] = q[0]*q[0]+q[3]*q[3]-q[1]*q[1]-q[2]*q[2];
			//a_z = T[0]*(float)a[0] + T[1] * (float)a[1] + T[2] * (float)a[2];
			//usq2[0] = a_z;

			//printf("z: %f\r\n", a_z);
			//  printf("acc: %f,%f,%f\n",T[0],T[1],T[2]);
			//printf("ac2: %d,%d,%d %f\r\n\r\n",acc[0],acc[1],acc[2], a_z);
		}
}

int vt = 0;
int az_tmp = 0;
static float old_a[3];
float a_cal = 0, v_cal = 0;
u8 dt_cal_flag = 0;
u32 dt_cal_cnt = 0;
void dt_deal()
{

//	float angle_tmp[3] = {0};
//	float error_angle[3] = {0};
//	angle_tmp[0] = pitch;
//	angle_tmp[1] = roll;
//	angle_tmp[2] = yaw;
//	float a_tmp[3] = {0};
//	float error_a[3] = {0};
//	a_tmp[0] = a[0];
//	a_tmp[1] = a[1];
//	a_tmp[2] = a[2];
//	
//	
//	

//	

//	for(int i = 0; i< 3; i++){
//		error_angle[i] = angle_tmp[i] > angle_init[i] ?angle_tmp[i] - angle_init[i]: angle_init[i] - angle_tmp[i];
//		error_a[i] = a_tmp[i] > old_a[i] ?a_tmp[i] - old_a[i]: old_a[i] - a_tmp[i];
//		error_angle[2] = 0;
//		if(error_angle[i] > 58 || error_a[i] > 15)
//		{
//				old_a[0] = a_tmp[0];
//				old_a[1]=a_tmp[1];
//				old_a[2]=a_tmp[2];
//			return;
//		}		
//	}
//	// 1............1..........1..............1..........
//	for(int i = 0; i< 3; i++){
//		error_a[i] = a_tmp[i] > old_a[i] ?a_tmp[i] - old_a[i]: old_a[i] - a_tmp[i];
//		if(error_a[i] > 1.5 && error_a[i] < 9)
//		{
//	
//			if(v_cal >= 0.09 && v_cal <= 0.11)
//			{

//					v_cal = 0;
//					dt_cal_flag = 0;
//			}
//			else
//			{
//				v_cal = 0.1;
//				dt_cal_flag = 1;
//			}
//			break;
//		}	
//	}
//	
//	f_dt += v_cal;
//	dt = f_dt;
//	old_a[0] = a_tmp[0];
//	old_a[1]=a_tmp[1];
//	old_a[2]=a_tmp[2];

}
void set_mpu6050_init()
{
	dt = 0;
	f_dt = 0;
	x_value[0] = 0;
//	v_cal = 0;
//	angle_init[0] = pitch;
//	angle_init[1] = roll;
//	angle_init[2] = yaw;
//	
//	old_a[0] = a[0];
//	old_a[1]=a[1];
//	old_a[2]=a[2];
}
