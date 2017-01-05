#include "stdio.h"
#include "common.h"
#include "usart.h"

/*
	��ȡ��Ƭ��12�ֽ� ID
*/
#define UniqueID_Addr 0x1FFFF7E8U
void GetUniqueID(char* ID)
{
	char i;
	char *p = (char*)UniqueID_Addr; 
	for(i=0; i<12; i++){
		*ID++ = *p++;
	}
}

/*
	������������
*/
void SetBlueToothName(void)
{
	char temp[32];
	char check;
	GetUniqueID(temp);
	check = XOR_Get(temp, 12);
	sprintf(temp, "AT+NAMEMWBalanced-%X\r\n",(int)check);
	Uart3SendStr("AT\r\n");
	Uart3SendStr(temp);
}


/*
	���������
*/
unsigned char XOR_Get(char * str, unsigned char  len)
{
	unsigned char	i=0;
	unsigned char 	sum=0;
	for(i=0; i<len; i++)
	{
		sum ^= *str++;
	}
	return sum;
}

/*
	����������Ƿ���ȷ
*/
unsigned char XOR_Check(char * str, unsigned char  len,unsigned char checksum)
{
	unsigned char 	sum=0;
	sum = XOR_Get(str, len);
	if(sum == checksum) return 1;
	else	return 0; 
}

/*
	�����ۼӺ�
*/
unsigned char Sum_Get(char *dat,char len)
{
	char i,sum=0;
	for(i=0; i<len;i++)
	{
		sum += *dat++;
	}
	return sum;
}




