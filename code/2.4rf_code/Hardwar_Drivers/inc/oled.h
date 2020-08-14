/*
 * oled.h
 *
 *  Created on: 2020��7��14��
 *      Author: simonlu
 */

#ifndef __OLED_H
#define __OLED_H 

#include "stm32f10x.h"
#include "stdlib.h"
#include "stdio.h"

//-----------------OLED�˿ڶ���---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_3)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOB,GPIO_Pin_3)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_5)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_7)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_7)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_6)//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_6)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_9)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define OLED_WIDTH  128
#define OLED_HEIGHT 64

#define WHITE 1 
#define BLACK 0 

#define _BV(n) (1u<<n)
#define min(a,b) ((a)<=(b)?(a):(b))
#define max(a,b) ((a)>=(b)?(a):(b))
#define ABS(x) (((x) > 0) ? (x) : -(x))
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))	

void OLED_W_CMD(u8 cmd);
void OLED_W_DATA(u8 data);

void OLED_Init(void);//��ʼ��OLED
void OLED_GPIO_Init(void);
void OLED_SPI_Init(void);
void OLED_DisplayTurn(u8 i);
void OLED_Refresh_Gram(void);//����OLED�Դ�
void OLED_ClearDisplay(void);//OLED����
void OLED_FillScreen(u8 color);//OLED�������

void OLED_DrawPixel(u8 x, u8 y, u8 color);//����
void OLED_DrawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color);//��Բ
void FillRect2(int16_t x, int16_t y, uint8_t w, int8_t h, uint8_t color);////��ʵ�ľ��� �������������
void DrawRect(int16_t x, int16_t y, uint8_t w, int8_t h, uint8_t color);//�����ľ���
void FillPatternedRect(int16_t x, int16_t y, uint8_t w, int8_t h, const uint8_t *ptn);//��Բ�Ǿ���
void OLED_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);//����
void OLED_DrawFastVLine(int16_t x, int16_t y, int8_t h, uint8_t color);//����ֱ��
void OLED_DrawFastHLine(int16_t x, int16_t y, uint8_t w, uint8_t color);//��ˮƽ��
void OLED_DrawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);//OLED��ͼ

void DrawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size);//���ַ�
void OLED_ShowString(u8 x,u8 y,char *chr);//OLED��ʾ�ַ���
void SetCursor(int16_t x, int16_t y);//��������
void OLED_PrintChar( char *chr);//��ӡ�ַ���
void OLED_PrintNum(u16 num);//��ӡ����
size_t OLED_PrintCharEx(int16_t x, int16_t y,char *chr);////�������ӡ�ַ���

int fputc(int ch, FILE *f);

#endif /* OLED_H_ */




