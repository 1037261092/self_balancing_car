#include "mpuiic.h"
#include "systick.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32F103������
//MPU6050 IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2018/7/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2018-2028
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
  //MPU IIC ��ʱ����
void MPU_IIC_Delay(void)
{
	Delay_us(2);
}

//��ʼ��IIC
void MPU_IIC_Init(void)
{					     	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);          //����GPIOBʱ��
	 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;    	//����
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);//PB11,PB12 �����	
}

//����IIC��ʼ�ź�
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();     //sda�����
	SET_SDA;	  	  
	SET_SCL;
	MPU_IIC_Delay();
 	CLR_SDA;//START:when CLK is high,DATA change form high to low 
	MPU_IIC_Delay();
	CLR_SCL;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();//sda�����
	CLR_SCL;
	CLR_SDA;//STOP:when CLK is high DATA change form low to high
 	MPU_IIC_Delay();
	SET_SCL;  
	SET_SDA;//����I2C���߽����ź�
	MPU_IIC_Delay();							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t MPU_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;  
	MPU_SDA_IN();      //SDA����Ϊ����  
	SET_SDA;MPU_IIC_Delay();	   
	SET_SCL;MPU_IIC_Delay();	 
	while(MPU_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	CLR_SCL;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void MPU_IIC_Ack(void)
{
	CLR_SCL;
	MPU_SDA_OUT();
	CLR_SDA;
	MPU_IIC_Delay();
	SET_SCL;
	MPU_IIC_Delay();
	CLR_SCL;
}
//������ACKӦ��		    
void MPU_IIC_NAck(void)
{
	CLR_SCL;
	MPU_SDA_OUT();
	SET_SDA;
	MPU_IIC_Delay();
	SET_SCL;
	MPU_IIC_Delay();
	CLR_SCL;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MPU_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;  
		MPU_SDA_OUT(); 		
    CLR_SCL;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {   
			if (txd & 0x80) SET_SDA;else CLR_SDA;
      //MPU_IIC_SDA=(txd&0x80)>>7;
      txd<<=1; 	  
			SET_SCL;
			MPU_IIC_Delay(); 
			CLR_SCL;	
			MPU_IIC_Delay();
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MPU_SDA_IN();//SDA����Ϊ����
  for(i=0;i<8;i++ )
	{
        CLR_SCL; 
        MPU_IIC_Delay();
		SET_SCL;
        receive<<=1;
        if(MPU_READ_SDA)receive++;   
		MPU_IIC_Delay(); 
    }					 
    if (!ack)
        MPU_IIC_NAck();//����nACK
    else
        MPU_IIC_Ack(); //����ACK   
    return receive;
}


















