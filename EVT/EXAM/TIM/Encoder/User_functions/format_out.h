/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Format_out.h
  * @brief          : Header for format_out.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FORMAT_OUT
#define __FORMAT_OUT

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/* Private includes ----------------------------------------------------------*/
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/

extern UART_HandleTypeDef huart2;

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
#define	TerminalInterface	huart2

/* Exported functions prototypes ---------------------------------------------*/

void GPIO_Blink_Test(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t Count_of_Blink, uint16_t Period_of_blink_ms);
void Get_Time(void);
void Get_Date(void);
void Get_Time_output(uint8_t *Uhren,uint8_t *Minutn,uint8_t *Sekundn);
uint32_t RTC_update_and_Terminal(uint32_t Period_update_ms);
uint16_t Process_Rx_Array_UART_DMA(uint8_t *Array,uint16_t Size_of_Array);
void Message_2_UART(char *pMessage);
void Message_2_UART_u16(char *pMessage, uint16_t Argument);
void Message_2_UART_u32(char *pMessage, uint32_t Argument);
uint16_t CAN_GetState(CAN_HandleTypeDef *hcan, char* String);

float process_adc_buffer(uint16_t *buffer);
//void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) ;
void Datum_Time_from_PC(
						RTC_DateTypeDef Date_Upd,
						RTC_TimeTypeDef sTime_Set
						);

/* Private defines -----------------------------------------------------------*/
#define CO_Aliex_Disco407green	0x3A
#define CO_Disco407_Blue		0x3b
#define CO_Lower__f407xx		0x3c
#define CO_Upper_F407XX			0x3d
#define CO_Disco407_Green_1		0x3e
#define Node_Unconfigured		0xFF

#define Make_Read_SDO			1
#define TerminalInterface		huart2
#define ADC_SAMPLES 10

#ifdef __cplusplus
}
#endif

#endif /* __FORMAT_OUT */
