#include "string.h"
#include "stdlib.h"
#include "communicate.h"
#include "mpu6050.h"
#include "upstandingcar.h"
#include "usart.h"
#include "dataflash.h"
#include "common.h"

float sppData1,sppData2;
char StatusFlag=0;


/*
	�ϱ���̬����
*/
void ResponseIMU()
{
	char temp[32];	
	sprintf(temp, "V,%d,%d,%.0f\r\n", 
		(-accel[1])/285+180,
		gyro[0]/30+180,
		Roll+180);
	BluetoothOut(temp);
}

/*
	�ϱ�PIDֵ
*/
void ResponsePID(PIDType type)
{
	char temp[32];
	if(type==AnglePID)
		sprintf(temp, "AP,%.2f,%.2f,%.2f\r\n", 
		g_tCarAnglePID.P,
		g_tCarAnglePID.I,
		g_tCarAnglePID.D);
	else if(type == SpeedPID)
		sprintf(temp, "SP,%.2f,%.2f,%.2f\r\n", 
		g_tCarSpeedPID.P,
		g_tCarSpeedPID.I,
		g_tCarSpeedPID.D);
	BluetoothOut(temp);
}

/*
	�ϱ��汾��Ϣ
*/
const char FirmwareVer[] = "2.10";
const char EEPROMVer[]="2.00";
const char MCUVer[] = "STM32F103C8T6";
void ResponseInfo()
{
	char temp[32];
	sprintf(temp, "I,%s,%s,%s\r\n", 
		FirmwareVer,
		EEPROMVer,
		MCUVer);
	BluetoothOut(temp);
}


/*
	�ϱ�״̬��Ϣ
*/
extern unsigned short RunTime;
extern unsigned short BatVol;
void ResponseStatus()
{
	char temp[32];
	sprintf(temp, "R,%d,%d\r\n", 
		BatVol*10,
		RunTime);
	BluetoothOut(temp);
}

/*
	APP���ݽ�������
	Э��淶:֡ͷ+����+����+У��
	��:#17,CM,-27.314,-5.716*56;  ֡ͷ# ����17  ����CM,-27.314,-5.716 У��56������Ϊ�����ֽ���
*/
void Parse(char *dataInput)
{
	unsigned char len,check,ret;
	char *p = dataInput;

	if(Uart3Index){
		Uart1SendBuf(Uart3Buffer, Uart3Index);
		Uart1SendStr("\r\n");
	}

	while(((p-dataInput)<Uart3Index))
	{
		if(*p == '#')// 
		{
			len = (unsigned char)atof(strtok(p+1, ","));//length of data
			if((len>0)&&(len<100))//���ȴ���100�����ݺ���
			{
				if(len<10){
					check = (unsigned char)atof(strtok(p+len+4, ";"));
					ret = XOR_Check(p+3, len, check);
					p += 3;
				}
				else{
					check = (unsigned char)atof(strtok(p+len+5, ";"));
					ret = XOR_Check(p+4, len, check);
					p += 4;
				}
				if(ret){//  У����ȷ����ʼ��������
					if(strstr(p , "CS")){steer(stop);}//ֹͣ
					else if(strstr(p, "CJ")){// ҡ�˿���
						strtok(p, ",");
						sppData1 = atof(strtok(NULL,","));//X������
						sppData2 = atof(strtok(NULL,"*"));//Y������
						steer(joystick);
					}
					else if(strstr(p, "CM")){//������Ӧ����
						strtok(p, ",");
						sppData1 = atof(strtok(NULL, ",")); // Pitch
      						sppData2 = atof(strtok(NULL, "*")); // Roll
      						steer(imu);
					}
					else if(strstr(p,"SAP")){// ���ýǶȻ�PID
						strtok(p, ",");
						g_tCarAnglePID.P = atof(strtok(NULL,","));
						g_tCarAnglePID.I = atof(strtok(NULL,","));
						g_tCarAnglePID.D = atof(strtok(NULL,"*"));
						PIDWrite(AnglePID);
						ResponsePID(AnglePID);
					}
					else if(strstr(p,"SSP")){// �����ٶȻ�PID
						strtok(p, ",");
						g_tCarSpeedPID.P = atof(strtok(NULL,","));
						g_tCarSpeedPID.I = atof(strtok(NULL,","));
						g_tCarSpeedPID.D = atof(strtok(NULL,"*"));
						PIDWrite(SpeedPID);
						ResponsePID(SpeedPID);
					}
					else if(strstr(p, "GAP")){//��ѯ�ǶȻ�PID
						ResponsePID(AnglePID);
					}
					else if(strstr(p, "GSP")){//��ѯ�ٶȻ�PID
						ResponsePID(SpeedPID);
					}
					else if(strstr(p,"RA")){// �ǶȻ�pid�ָ�Ĭ��ֵ
						PIDReset(AnglePID);
						ResponsePID(AnglePID);
					}
					else if(strstr(p,"RV")){//�ٶȻ�pis�ָ�Ĭ��ֵ
						PIDReset(SpeedPID);
						ResponsePID(SpeedPID);
					}
					else if(strstr(p,"RB")){//��ѯ�汾������״̬�ϱ���Ϣ
						StatusFlag = 1;
						ResponseInfo();
					}
					else if(strstr(p,"RS")){//�ر�״̬�ϱ�
						StatusFlag = 0;
					}

				}
			}
			
		}
		p++;
	}
	Uart3Index=0;
	memset(Uart3Buffer, 0, Uart3BufferSize);
}


#if 0
void monitorTest()
{
	short temp[3];
	unsigned char cTemp[32],Index=0;
	char i=0;
	
	temp[0] = g_fGravityAngle;
	temp[1] = g_fGyroAngleSpeed;
	temp[2] = g_fCarAngle;

	cTemp[Index++]=0xaa;
	cTemp[Index++]=0x55;
	cTemp[Index++]=0x01;
	cTemp[Index++]=0x06;
	
	for(i=0; i<3;  i++){
		cTemp[Index++] = (temp[i]>>8);
		cTemp[Index++] = (temp[i]);
	}

	cTemp[Index++]= Sum(cTemp,Index);
	
	for(i=0; i<Index; i++)UART1SendByte(cTemp[i]);

}
#endif


