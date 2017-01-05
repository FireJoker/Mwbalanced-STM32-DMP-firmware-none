/******************** (C) COPYRIGHT 2014 MiaoW Labs ***************************
** �ļ����ƣ�led.c
** ��������: led Ӧ�ú�����            
** ʵ��ƽ̨��������ƽ��С��BasicBalance���ذ�
** Ӳ�����ӣ�----------------- 
**	   		|   PB3 - LED1     |
**			|   PB4 - LED2     |
**			 ----------------- 
** ��汾  ��ST3.5.0 
** ������:   ����ʵ����
** ��  ����  Http://miaowlabs.taobao.com
** �ա���:   2014��08��01��
**********************************************************************************/
#include "led.h"

/*
 * ��������LED_GPIO_Config
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����
 */
void LED_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*�ȿ���GPIOA �� AFIO ������ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO,ENABLE);

	/*�ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_JTAGDisable ��JTAG-DP ���� + SW-DP ʹ��*/
    	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); 

	/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*���ÿ⺯������ʼ��GPIOB*/
  	GPIO_Init(GPIOA, &GPIO_InitStructure);			  

	/* ������led��	*/
	GPIO_SetBits(GPIOA, GPIO_Pin_11 | GPIO_Pin_12); 	 
}



/******************* (C) COPYRIGHT 2014 MiaoW Labs *****END OF FILE************/
