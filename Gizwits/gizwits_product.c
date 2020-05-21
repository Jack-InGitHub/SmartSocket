/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related       hardware initialization
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/

#include <stdio.h>
#include <string.h>
#include "hal_key.h"
#include "gizwits_product.h"
#include "common.h"
#include "main.h"

static uint32_t timerMsCount;
uint8_t aRxBuffer;

/** User area the current device state structure*/
dataPoint_t currentDataPoint;
extern keysTypedef_t keys;

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
    uint8_t i = 0;
    dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
    moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
    protocolTime_t *ptime = (protocolTime_t *)gizdata;

#if MODULE_TYPE
    gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
    moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

    if((NULL == info) || (NULL == gizdata))
    {
        return -1;
    }
    
    GIZWITS_LOG("gizwitsEventProcess----------\n");

    for(i=0; i<info->num; i++)
    {
        switch(info->event[i])
        {
        case EVENT_Socket1:
            currentDataPoint.valueSocket1 = dataPointPtr->valueSocket1;
            GIZWITS_LOG("Evt: EVENT_Socket1 %d \n", currentDataPoint.valueSocket1);
            if(0x01 == currentDataPoint.valueSocket1)
            {
                //user handle
                HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_SET);
            }
            else
            {
                //user handle
                HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_RESET);
            }
            break;
        case EVENT_Socket2:
            currentDataPoint.valueSocket2 = dataPointPtr->valueSocket2;
            GIZWITS_LOG("Evt: EVENT_Socket2 %d \n", currentDataPoint.valueSocket2);
            if(0x01 == currentDataPoint.valueSocket2)
            {
                //user handle
                HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_SET);
            }
            else
            {
                //user handle
                HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_RESET);
            }
            break;


        case EVENT_OnDay_1:
            currentDataPoint.valueOnDay_1 = dataPointPtr->valueOnDay_1;
            GIZWITS_LOG("Evt:EVENT_OnDay_1 %d\n",currentDataPoint.valueOnDay_1);
            //user handle
            break;
        case EVENT_OnHour_1:
            currentDataPoint.valueOnHour_1 = dataPointPtr->valueOnHour_1;
            GIZWITS_LOG("Evt:EVENT_OnHour_1 %d\n",currentDataPoint.valueOnHour_1);
            //user handle
            break;
        case EVENT_OnMinute_1:
            currentDataPoint.valueOnMinute_1 = dataPointPtr->valueOnMinute_1;
            GIZWITS_LOG("Evt:EVENT_OnMinute_1 %d\n",currentDataPoint.valueOnMinute_1);
            //user handle
            break;
        case EVENT_OffDay_1:
            currentDataPoint.valueOffDay_1 = dataPointPtr->valueOffDay_1;
            GIZWITS_LOG("Evt:EVENT_OffDay_1 %d\n",currentDataPoint.valueOffDay_1);
            //user handle
            break;
        case EVENT_OffHour_1:
            currentDataPoint.valueOffHour_1 = dataPointPtr->valueOffHour_1;
            GIZWITS_LOG("Evt:EVENT_OffHour_1 %d\n",currentDataPoint.valueOffHour_1);
            //user handle
            break;
        case EVENT_OffMinute_1:
            currentDataPoint.valueOffMinute_1 = dataPointPtr->valueOffMinute_1;
            GIZWITS_LOG("Evt:EVENT_OffMinute_1 %d\n",currentDataPoint.valueOffMinute_1);
            //user handle
            break;


        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER:
            GIZWITS_LOG("WIFI_CON_ROUTER ...\n");
            break;
        case WIFI_DISCON_ROUTER:
            GIZWITS_LOG("WIFI_DISCON_ROUTER ...\n");
            break;
        case WIFI_CON_M2M:
            GIZWITS_LOG("WIFI_CON_M2M ...\n");
            break;
        case WIFI_DISCON_M2M:
            GIZWITS_LOG("WIFI_DISCON_M2M ...\n");
            break;
        case WIFI_CON_APP:
            GIZWITS_LOG("WIFI_CON_APP ...\n");
            {
                uint8_t temp=0;
                uint8_t humi=0;
                
                if( DHT11_Read_Data(&temp, &humi) != 0 )
                {
                    GIZWITS_LOG("DHT11 : Err\n");
                }
                GIZWITS_LOG("DHT11 : %dC %d%% \n",temp,humi);
                
                currentDataPoint.valueRH = humi;
                currentDataPoint.valueTemperature = temp;
            }
            break;
        case WIFI_DISCON_APP:
            GIZWITS_LOG("WIFI_DISCON_APP ...\n");
            break;
        case WIFI_RSSI:
            GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
            break;
        case TRANSPARENT_DATA:
            GIZWITS_LOG("TRANSPARENT_DATA \n");
            //user handle , Fetch data from [data] , size is [len]
            break;
        case WIFI_NTP:
            GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
            {
                extern RTC_HandleTypeDef hrtc;
                RTC_TimeTypeDef Time;
                RTC_DateTypeDef Data;
                
                Data.Year = ptime->year%100;
                Data.Month = ptime->month;
                Data.Date = ptime->day;
                HAL_RTC_SetDate(&hrtc, &Data, RTC_FORMAT_BIN);
                
                Time.Hours = ptime->hour;
                Time.Minutes = ptime->minute;
                Time.Seconds = ptime->second;
                HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);
            }
            break;
        case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
#if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
#else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
#endif
            break;
        default:
            break;
        }
    }

    return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
    /*
       currentDataPoint.valueCurrent = ;//Add Sensor Data Collection
       currentDataPoint.valueVoltage = ;//Add Sensor Data Collection
       currentDataPoint.valueTemperature = ;//Add Sensor Data Collection
       currentDataPoint.valueRH = ;//Add Sensor Data Collection

       */

}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));

    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/
    /*
      currentDataPoint.valueSocket1 = ;
      currentDataPoint.valueSocket2 = ;
      currentDataPoint.valueOnDay_1 = ;
      currentDataPoint.valueOnHour_1 = ;
      currentDataPoint.valueOnMinute_1 = ;
      currentDataPoint.valueOffDay_1 = ;
      currentDataPoint.valueOffHour_1 = ;
      currentDataPoint.valueOffMinute_1 = ;
      currentDataPoint.valueCurrent = ;
      currentDataPoint.valueVoltage = ;
      currentDataPoint.valueTemperature = ;
      currentDataPoint.valueRH = ;
    */

}


/**
* @brief Millisecond timing maintenance function, milliseconds increment, overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief Read millisecond count

* @param none
* @return millisecond count
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief MCU reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
}

/**@} */

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART1 and Loop until the end of transmission */
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==&htim2)
    {
        keyHandle((keysTypedef_t *)&keys);
        gizTimerMs();
    }
}

/**
* @brief Timer TIM3 init function

* @param none
* @return none
*/
void timerInit(void)
{
    HAL_TIM_Base_Start_IT(&htim2);
}

/**
  * @brief  This function handles USART IDLE interrupt.
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef*UartHandle)
{
    if(UartHandle->Instance == USART2)
    {
        gizPutData((uint8_t *)&aRxBuffer, 1);

        HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断
    }
}

/**
* @brief USART init function

* Serial communication between WiFi modules and device MCU
* @param none
* @return none
*/
void uartInit(void)
{
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断
}

/**
* @brief Serial port write operation, send data to WiFi module
*
* @param buf      : buf address
* @param len      : buf length
*
* @return : Return effective data length;-1，return failure
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint8_t crc[1] = {0x55};
    uint32_t i = 0;

    if(NULL == buf)
    {
        return -1;
    }

    for(i=0; i<len; i++)
    {
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)&buf[i], 1);
        while (huart2.gState != HAL_UART_STATE_READY);//Loop until the end of transmission

        if(i >=2 && buf[i] == 0xFF)
        {
            HAL_UART_Transmit_IT(&huart2, (uint8_t *)&crc, 1);
            while (huart2.gState != HAL_UART_STATE_READY);//Loop until the end of transmission
        }
    }

#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);

        if(i >=2 && buf[i] == 0xFF)
        {
            GIZWITS_LOG("%02x ", 0x55);
        }
    }
    GIZWITS_LOG("\n");
#endif

    return len;
}
