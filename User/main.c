/******************** (C) COPYRIGHT 2016 MiaowLabs **************************
 * �ļ���  ��main.c
 * ����    ������ʵ����ƽ��С��Mwbalanced STM32       
 * ʵ��ƽ̨��Mwbalanced STM32
 * ��汾  ��ST3.5.0
 *
 * ����    ��MiaowLabs 
 * ��̳    ��https://bbs.miaowlabs.com/
 * �Ա�    ��https://miaowlabs.taobao.com/
**********************************************************************************/
#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
#include "mpu6050.h"
#include "i2c_mpu6050.h"
#include "i2c.h"
#include "motor.h"
#include "SysTick.h"
#include "upstandingcar.h"
#include "outputdata.h"
#include "ADC.h"
#include "communicate.h"
#include "dataflash.h"
#include "common.h"

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */


unsigned short encoder_num1,encoder_num2;

uint16_t RunTime=0;
uint16_t BatVol;

//�뼶����
void SecTask()
{
	static char LED=0;
	if(SoftTimer[0])return;
	else{
		SoftTimer[0] = 1000;
	}
	RunTime++; // ��¼����ʱ��
	BatVol = GetBatVol(); // ��ȡ��ص�ѹ

	if(StatusFlag)ResponseStatus();
	
	LED = ~LED;
	if(LED)LED1(ON);
	else LED1(OFF);
}

int main(void)
{	
	LED_GPIO_Config();	//LED����
	USART1_Config();	//UART1����-->MicroUSB��
	USART3_Config(0);	//UART3����-->������ģ��ͨ��
	delay_nms(20);
	Uart3SendStr("AT\r\n");
	Uart3SendStr("AT+BAUD8\r\n"); //��������ģ��ָ��
	USART3_Config(1);	//����UART3������Ϊ115200
	delay_nms(20);
	SetBlueToothName();	//��������ģ������
	
	NVIC_Configuration();//�ж����ȼ�����
	TIM2_PWM_Init(); //PWM��ʼ��
	MOTOR_GPIO_Config();//����������GPIO��ʼ��
	ADC_Config();//ADC��ʼ��
	TIM3_External_Clock_CountingMode();	 //������ٳ�ʼ��
	TIM4_External_Clock_CountingMode();	 //������ٳ�ʼ��
	i2cInit();	 //��ʼ��I2C
	delay_nms(10);
	MPU6050_Init();//��ʼ��MPU6050
	PIDInit(); //��ʼ��PID
	SysTick_Init();	//��ʼ����ʱ��
	
	CarUpstandInit(); //��ʼ��ϵͳ����

	GPIO_ResetBits(GPIOB, GPIO_Pin_4);

	// ʹ�ܵδ�ʱ��  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	
	while (1)
	{
		SecTask();

		if(SoftTimer[1]==0){// ÿ��50ms�ϴ�һ����̬����
			SoftTimer[1] = 50;
			ResponseIMU();
			Parse(Uart3Buffer);
		}
	
#if 0 /*������ Ԥ��������*/

	  
   OutData[0] = Roll;
   OutData[1] = gyro[0];
   OutData[2] = accel[1] ;
   //OutData[3] = g_iAccelInputVoltage_X_Axis;
   
   OutPut_Data();
#endif	  

#if 0
		encoder_num1=TIM_GetCounter(TIM3);
		TIM3->CNT = 0;
		encoder_num2=TIM_GetCounter(TIM4);
		TIM4->CNT = 0;
		//printf("\r\n this is a printf demo \r\n");
	    //printf("\r\n ��ӭʹ��������ƽ��С��BasicBalance���ذ�:) \r\n");		
		printf("\r\n---------������1---------%d \r\n",encoder_num1);
		printf("\r\n---------������2---------%d \r\n",encoder_num2);
#endif	  
		/*
		printf("\r\n---------���ٶ�X��ԭʼ����---------%d \r\n",GetData(ACCEL_XOUT_H));
		printf("\r\n---------���ٶ�Y��ԭʼ����---------%d \r\n",GetData(ACCEL_YOUT_H));	
		printf("\r\n---------���ٶ�Z��ԭʼ����---------%d \r\n",GetData(ACCEL_ZOUT_H));	
		printf("\r\n---------������X��ԭʼ����---------%d \r\n",GetData(GYRO_XOUT_H));	
		printf("\r\n---------������Y��ԭʼ����---------%d \r\n",GetData(GYRO_YOUT_H));	
		printf("\r\n---------������Z��ԭʼ����---------%d \r\n",GetData(GYRO_ZOUT_H));
		delay_ms(10);     									   */
	}
}


/******************* (C) COPYRIGHT 2016 MiaowLabs Team *****END OF FILE************/


