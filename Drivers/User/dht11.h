#ifndef __DHT11_H
#define __DHT11_H 

#include "common.h"
#include "main.h"

//IO��������
#define DHT11_IO_IN()   {DHT11_GPIO_Port->CRH&=0XFFF0FFFF;DHT11_GPIO_Port->CRH|=8<<16;}
#define DHT11_IO_OUT()  {DHT11_GPIO_Port->CRH&=0XFFF0FFFF;DHT11_GPIO_Port->CRH|=3<<16;}

////IO��������											   
#define	DHT11_DQ_OUT    PAout(12)           //���ݶ˿� 
#define	DHT11_DQ_IN     PAin(12)            //���ݶ˿� 


uint8_t DHT11_Init(void);                                   //��ʼ��DHT11
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);       //��ȡ��ʪ��
uint8_t DHT11_Read_Byte(void);                              //����һ���ֽ�
uint8_t DHT11_Read_Bit(void);                               //����һ��λ
uint8_t DHT11_Check(void);                                  //����Ƿ����DHT11
void DHT11_Rst(void);                                       //��λDHT11    
#endif















