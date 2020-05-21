#ifndef __DHT11_H
#define __DHT11_H 

#include "common.h"
#include "main.h"

//IO方向设置
#define DHT11_IO_IN()   {DHT11_GPIO_Port->CRH&=0XFFF0FFFF;DHT11_GPIO_Port->CRH|=8<<16;}
#define DHT11_IO_OUT()  {DHT11_GPIO_Port->CRH&=0XFFF0FFFF;DHT11_GPIO_Port->CRH|=3<<16;}

////IO操作函数											   
#define	DHT11_DQ_OUT    PAout(12)           //数据端口 
#define	DHT11_DQ_IN     PAin(12)            //数据端口 


uint8_t DHT11_Init(void);                                   //初始化DHT11
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);       //读取温湿度
uint8_t DHT11_Read_Byte(void);                              //读出一个字节
uint8_t DHT11_Read_Bit(void);                               //读出一个位
uint8_t DHT11_Check(void);                                  //检测是否存在DHT11
void DHT11_Rst(void);                                       //复位DHT11    
#endif















