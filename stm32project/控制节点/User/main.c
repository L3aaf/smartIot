#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "usart.h"
#include "LED.h"
#include "Key.h"
#include "beep.h"

#include <stdio.h>

u8 Led_state = 0;
uint8_t KeyNum;

void Hardware_Init(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	LED_Init();			//LED初始化
	Key_Init();			//按键初始化
	Beep_Init();		//蜂鸣器初始化
	Usart1_Init(115200);     //Debug串口初始化
	Usart2_Init(115200);	 //8266串口初始化
	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");	
}



int main(void)
{
	
	Hardware_Init();		//外设初始化

	while (1)
	{	
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			LED1_Turn();
		}
	}
}
