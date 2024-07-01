#include "stm32f10x.h"                  // Device header

/**
  * ��    ������������ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void Beep_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//����GPIOB��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//��PB12���ų�ʼ��Ϊ�������
	
	/*����GPIO��ʼ�����Ĭ�ϵ�ƽ*/
	GPIO_SetBits(GPIOB, GPIO_Pin_12);							//����PB12����Ϊ�ߵ�ƽ
}

/**
  * ��    ��������������
  * ��    ������
  * �� �� ֵ����
  */
void Beep_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);		//����PB12����Ϊ�͵�ƽ
}

/**
  * ��    �����������ر�
  * ��    ������
  * �� �� ֵ����
  */
void Beep_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);		//����PB12����Ϊ�ߵ�ƽ
}

/**
  * ��    ����������״̬��ת
  * ��    ������
  * �� �� ֵ����
  */
void Beep_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12) == 0)		//��ȡ����Ĵ�����״̬�������ǰ��������͵�ƽ
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);						//������PB12����Ϊ�ߵ�ƽ
	}
	else														//���򣬼���ǰ��������ߵ�ƽ
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);						//������PB12����Ϊ�͵�ƽ
	}
}
