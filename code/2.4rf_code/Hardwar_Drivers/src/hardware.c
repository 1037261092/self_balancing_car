/*
 * hardware.c
 *
 *  Created on: 2020/7/26
 *      Author: simonlu
 */

#include "hardware.h"

uint8_t temp[5];   //use to translate number to ascii
float BattVolt;
uint16_t JoystickX;
uint16_t JoystickY;

void Hardware_Init(void)
{	
	//OLED Init
	OLED_Init();
	//��Ļ��ת180��
	OLED_DisplayTurn(1);
	OLED_FillScreen(0);
	OLED_Refresh_Gram();
	
	//LED Init
	LED_Init();
	printf("LED ON.\n");
	
	//Buzzer Init
	Buzzer_Init();
	printf("BUZZER TEST.\n");
	Play_NokiaTune();

	//MPU6050 Init
	while(mpu_dmp_init())
	{
		LED_OFF();
	}
	LED_ON();
	printf("MPU6050 INIT.\n");
	
	//SI24R1 Init
	SI24R1_Init();
	Delay_ms(100);  //According to daatasheet pages 22, power on reset 100ms.
	while(SI24R1_Check())
	{
		LED_OFF();
	}
	LED_ON();
	printf("SI24R1 INIT.\n");
	
	//ADC Init
	myADC_Init();
	Delay_ms(5);
	BattVolt=Get_BattVolt();
	printf("BATT VOLT: %.2f\n",BattVolt);
	
	JoystickX=Get_JoystickX();
	JoystickY=Get_JoystickY();
	printf("JOYSTICK X: %d\n",JoystickX);
	printf("JOYSTICK Y: %d\n",JoystickY);

	//Key & Charge Detect IO Init
	Key_Charge_Init();
	Delay_ms(300);
	
	/*
	OLED_ShowString(0,33,(uint8_t *)("W25Q128 Init."),8,1);
	OLED_Refresh();
	W25Q128_Init();
	while(FLASH_ID_Check())
	{
		LED_OFF();
		Delay_ms(1);
	}
	LED_ON();
	*/
}
