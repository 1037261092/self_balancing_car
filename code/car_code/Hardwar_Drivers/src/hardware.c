/*
 * hardware.c
 *
 *  Created on: 2020/8/12
 *      Author: simonlu
 */

#include "hardware.h"

float BattVolt;
extern float roll,pitch,yaw;

void Hardware_Init(void)
{
	/******************LED Init*************************/
	
	LED_Init();
	
/******************Buzzer Init**********************/	
	
	Buzzer_Init();

/******************USART Init***********************/	
	
	//USART1_Init();
	//printf("USART1 bandrate 115200bps \r\n");
	
/******************ADC Init*************************/	
	
	Volt_ADC_Init();
	Delay_ms(5);
	BattVolt=Get_BattVolt();
	if(BattVolt<=VOLT_STOP_VALUE)
	{
		Play_Beep();
		LED_OFF();
		while(1);
	}
	//printf("Battery voltage: %fV\r\n", BattVolt);
	
/*****************Motor Init************************/	
	
	DRV8833C_Init();

/******************MPU6050 Init*********************/	

  while(mpu_dmp_init())
	{
		LED_OFF();
	}
	/*
	 *The following code is to waiting until mpu output reliable data,
	 *because the get_data function always output roll=0 at the firstime,
	 *i want to set the car don't move when power up it if roll bigger than a degree,
	 *i don't know the exactly time i need to wait,
	*/
	for(int i=0;i<600;i++)
	{
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		Delay_ms(1);
	}
	LED_ON();
  //printf("MPU6050 DMP init successful!\r\n");	
	
/******************Si24r1 Init**********************/
	SI24R1_Init();
	Delay_ms(100);  //According to daatasheet pages 22, power on reset 100ms.
	while(!SI24R1_Check())
	{
		LED_OFF();
	}
	LED_ON();
	//printf("Si24r1 init successful!\r\n");
}
