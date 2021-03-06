/**
  ******************************************************************************
  * @file    main.c
  * @author  Simon Lu
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "hardware.h"

#define RF_SLEEP_TIME 4   //x50ms,if joystick don't move over this time, rf chip will stop tx.
#define RX_TIME       20  //x50ms, send get volt and temp request to car then transfer to rx mode immediately;

void getJoyStickData(void);
void sendData2Car(void);
void getCarTempVolt(void);
void displayGUI(void);

uint16_t tx_data[3]={2040,2040,0};   //joystick y,x,request flag.
float rx_data[2]={0,0};              //mpu temp and battery voltage.
uint8_t time2sleep=0;
uint8_t time2rx=21;

const unsigned char logo [] = {
0x00, 0x00, 0xE0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFE, 0x7E, 0x7E, 0x7E, 0x7E, 0x00, 0x00, 0xFE, 0xFE,
0xFE, 0xFE, 0xFE, 0x00, 0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0x00, 0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
0x7E, 0x7E, 0x7E, 0xFC, 0xFC, 0xFC, 0xF8, 0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x7E, 0x7E, 0x7E,
0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFE, 0x7E, 0x7E, 0x7E, 0x7E,
0x00, 0x00, 0x00, 0xE0, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFC, 0xE0, 0x00, 0x00, 0x00, 0xFE, 0xFE,
0xFE, 0xFE, 0xFE, 0x7E, 0x7E, 0xFE, 0xFC, 0xFC, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x07, 0x1F, 0x3F, 0x3F, 0x7F, 0x78, 0x70, 0x70, 0x70, 0x10, 0x00, 0x00, 0x0F, 0x3F, 0x3F,
0x7F, 0x7F, 0x60, 0x60, 0x7F, 0x7F, 0x7F, 0x3F, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x73,
0x73, 0x73, 0x3F, 0x3F, 0x1D, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x63, 0x63, 0x63, 0x63,
0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1F, 0x3F, 0x3F, 0x7F, 0x78, 0x70, 0x70, 0x70, 0x10, 0x00,
0x60, 0x7E, 0x7F, 0x3F, 0x31, 0x33, 0x3F, 0x7F, 0x7F, 0x7F, 0x7E, 0x60, 0x00, 0x7F, 0x7F, 0x7F,
0x7F, 0x7F, 0x1C, 0x3C, 0x7F, 0xFF, 0xFF, 0x73, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};


/********************************Main******************************************/
int main(void)
{	
	Hardware_Init();
	
	Play_Beep();
	
	SI24R1_TxMode();
	Delay_ms(1);
	
	while(1)
	{
		getJoyStickData();

		sendData2Car();
		
		getCarTempVolt();
		
	  displayGUI();
		
		Delay_ms(50);
	}
}

/*******************************Get Joystick Data******************************/
void getJoyStickData(void)
{
	tx_data[0]=Get_JoystickY();
	tx_data[1]=Get_JoystickX();
}

/*******************************Send Joystick Data*****************************/
void sendData2Car(void)
{
	if((tx_data[0]<2060&&tx_data[0]>2020)&&(tx_data[1]<2060&&tx_data[1]>2020))
	{
		time2sleep++;
	}
	else{
		time2sleep=0;
	}
	
	if(time2sleep>=RF_SLEEP_TIME)
	{
		time2sleep--;
	}
	else
	{		
		SI24R1_TxPacket((uint8_t *)tx_data);
	}
}

/*********************Get Car's MPU6050 Temp & Battery Voltage*****************/
void getCarTempVolt(void)
{
	if(time2rx>=RX_TIME)
	{
		time2rx=0;
		tx_data[2]=1;
		if(SI24R1_TxPacket((uint8_t *)tx_data)==TX_OK)
		{
			SI24R1_RxMode();
			Delay_ms(1);
			for(int i=0;i<10;i++)
			{
				if(!SI24R1_RxPacket((uint8_t *)rx_data))
				{
					break;
				}
				Delay_ms(1);
			}
			SI24R1_TxMode();
		}
		tx_data[2]=0;
	}
	else
	{
		time2rx++;
	}
}

/********************************Display Main GUI******************************/
void displayGUI(void)
{
	OLED_ClearDisplay();
	
	//Remote Part
	SetCursor(3,4);
	OLED_PrintChar("REMOTE");

	OLED_DrawFastVLine(43,0,16,1);
	
	SetCursor(50,0);
	OLED_PrintChar("VOLT: ");
	OLED_PrintFloat(rx_data[1]);
	OLED_PrintChar("V");
	
	SetCursor(50,8);
	OLED_PrintChar("TEMP: ");
	OLED_PrintFloat(rx_data[0]);
	OLED_PrintChar("'C");
	
	OLED_DrawFastHLine(0,16,128,1);
	
	//Rolling Part
	OLED_DrawBitmap(19,25,logo,111,17,1);
	FillRect2(0,41,128,7,0);
	
	//Self
	SetCursor(8,55);
	OLED_PrintChar("SELF");

	OLED_DrawFastVLine(43,48,16,1);
	
	SetCursor(50,51);
	OLED_PrintChar("VOLT: ");
	OLED_PrintFloat(Get_BattVolt());
	OLED_PrintChar("V");
	
	SetCursor(50,59);
	if(GPIO_ReadInputDataBit(CHARGE_DETECT_PORT,CHARGE_DETECT_PIN)==Bit_RESET)
	{
		OLED_PrintChar("CHARGING...");
	}
  else
	{
		OLED_PrintChar("NOT CHARGING");
	}
	
	OLED_DrawFastHLine(0,48,128,1);
	
	OLED_Refresh_Gram();
}
