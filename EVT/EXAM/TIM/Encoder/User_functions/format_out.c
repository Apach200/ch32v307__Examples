/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Format_out.c
  * @brief          : User Format output functions
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "format_out.h"
#include "lcd.h"
#include "usbd_cdc_if.h"



/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/




/* Private variables ---------------------------------------------------------*/
extern	DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
extern	DMA_HandleTypeDef hdma_memtomem_dma2_stream1;
extern	DMA_HandleTypeDef hdma_memtomem_dma2_stream3;


extern	RTC_HandleTypeDef hrtc;
extern	RTC_DateTypeDef DateToUpdate;
extern	RTC_TimeTypeDef sTime;

extern	UART_HandleTypeDef huart1;
extern	UART_HandleTypeDef huart2;
extern	DMA_HandleTypeDef hdma_usart1_rx;
extern	DMA_HandleTypeDef hdma_usart1_tx;
extern	DMA_HandleTypeDef hdma_usart2_tx;
extern	DMA_HandleTypeDef hdma_usart2_rx;



uint16_t adc_buffer[ADC_SAMPLES * 2 * 2] = {0};

/* Private functions -----------------------------------------------*/
///////////////////////////////////////////////////////////////////////////

//void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
//{
//process_adc_buffer(&adc_buffer[0]);
//}
//
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
//{
//process_adc_buffer(&adc_buffer[ADC_SAMPLES * 2]);
//}
//

// Process half a buffer full of data
float process_adc_buffer(uint16_t *buffer)
{
/*     uint32_t sum1 = 0;
   // uint32_t tmp = 0;
    //uint32_t sum2 = 0;
    for (int i = 0; i < ADC_SAMPLES; ++i) {
    	sum1 += buffer[i * 2];
    	//sum2 += buffer[1 + i * 2];
    }
    //	tmp =  4096 * ADC_SAMPLES;
    //float	temperature = 25.0 + ((float)sum1)/((float)tmp)*400.;
    float	temperature = (float)(sum1*3.3)/40960.*400;
   // vref = (float)sum2 / 1000 / ADC_SAMPLES;
    return (temperature); */
}



void Message_2_UART(char *pMessage)
{
static uint8_t	Array_2_UART_a[128];
static uint8_t	Array_2_UART_b[128];
static uint8_t	Array_2_UART_c[128];
static uint8_t Select_Array =0;
uint16_t Size_to_Send;

switch (Select_Array)
{
case 0:	Size_to_Send = sprintf((char*)Array_2_UART_a,pMessage);
		Array_2_UART_a[Size_to_Send+0]=0x0d;
		Array_2_UART_a[Size_to_Send+1]=0x0A;
		Select_Array=1;
		while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
		HAL_UART_Transmit_IT( &TerminalInterface, (uint8_t*)Array_2_UART_a, Size_to_Send+2);
break;

case 1:	Size_to_Send = sprintf((char*)Array_2_UART_b,pMessage);
		Array_2_UART_b[Size_to_Send+0]=0x0d;
		Array_2_UART_b[Size_to_Send+1]=0x0d;
		Select_Array=2;
		while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
		HAL_UART_Transmit_IT( &TerminalInterface, (uint8_t*)Array_2_UART_b,  Size_to_Send+2);
break;

case 2:	Size_to_Send = sprintf((char*)Array_2_UART_c,pMessage);
		Array_2_UART_c[Size_to_Send+0]=0x0d;
		Array_2_UART_c[Size_to_Send+1]=0x0d;
		Select_Array=0;
		while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
		HAL_UART_Transmit_IT( &TerminalInterface, (uint8_t*)Array_2_UART_c,  Size_to_Send+2);
break;

default:
break;
}//switch (Select_Array)

Select_Array++;
Select_Array = Select_Array %3;
}

///////////////////////////////////////////////////////////////////////////

void Message_2_UART_u16(char *pMessage, uint16_t Argument)
{
static uint8_t	Array_2_UART_a[128];
static uint8_t	Array_2_UART_b[128];
static uint8_t	Array_2_UART_c[128];
uint8_t added_length;
static uint8_t Select_Array =0;
uint16_t Size_to_Send=128;

	switch (Select_Array)
	{
	case 0:	Size_to_Send = sprintf((char*)Array_2_UART_a,pMessage);
			added_length = sprintf((char*)&Array_2_UART_a[Size_to_Send]  ," = 0x%x \n\r\n\r",Argument);
			Select_Array=1;
			while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
			HAL_UART_Transmit_IT( &TerminalInterface, (uint8_t*)Array_2_UART_a, Size_to_Send + added_length);
	break;
	case 1:	Size_to_Send = sprintf((char*)Array_2_UART_b,pMessage);
			added_length = sprintf((char*)&Array_2_UART_b[Size_to_Send]  ," = 0x%x \n\r\n\r",Argument);
			Select_Array=2;
			while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
			HAL_UART_Transmit_IT( &TerminalInterface, (uint8_t*)Array_2_UART_b,  Size_to_Send + added_length);
	break;
	case 2:	Size_to_Send = sprintf((char*)Array_2_UART_c,pMessage);
			added_length = sprintf((char*)&Array_2_UART_c[Size_to_Send]  ," = 0x%x \n\r\n\r",Argument);
			Select_Array=0;
			while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
			HAL_UART_Transmit_IT( &TerminalInterface, (uint8_t*)Array_2_UART_c,  Size_to_Send + added_length);
	break;
	default: break;
	}//switch (Select_Array)

Select_Array++;
Select_Array = Select_Array %3;
}
///////////////////////////////////////////////////////////////////////////

void Message_2_UART_u32(char *pMessage, uint32_t Argument)
{
static uint8_t	Array_2_UART_a[128];
static uint8_t	Array_2_UART_b[128];
static uint8_t	Array_2_UART_c[128];
uint8_t added_length;
static uint8_t Select_Array =0;
uint16_t Size_to_Send;

	switch (Select_Array)
	{
	case 0:	Size_to_Send = sprintf((char*)Array_2_UART_a,pMessage);
			added_length = sprintf((char*)&Array_2_UART_a[Size_to_Send]  ," = 0x%04X%04X \n\r\n\r\n\r\n\r\n\r\n\r\n\r",(uint16_t)(Argument>>16),(uint16_t)(Argument & 0x0FFFF));
			Select_Array=1;
			while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
			HAL_UART_Transmit_IT( &TerminalInterface, (uint8_t*)Array_2_UART_a, Size_to_Send + added_length);
	break;

	case 1:	Size_to_Send = sprintf((char*)Array_2_UART_b,pMessage);
			added_length = sprintf((char*)&Array_2_UART_a[Size_to_Send]  ," = 0x%04X%04X \n\r\n\r\n\r\n\r\n\r\n\r\n\r",(uint16_t)(Argument>>16),(uint16_t)(Argument & 0x0FFFF));
			Select_Array=2;
			while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
			HAL_UART_Transmit_IT( &TerminalInterface, (uint8_t*)Array_2_UART_b,  Size_to_Send + added_length);
	break;

	case 2:	Size_to_Send = sprintf((char*)Array_2_UART_c,pMessage);
			added_length = sprintf((char*)&Array_2_UART_a[Size_to_Send]  ," = 0x%04X%04X \n\r\n\r\n\r\n\r\n\r\n\r\n\r",(uint16_t)(Argument>>16),(uint16_t)(Argument & 0x0FFFF));
			Select_Array=0;
			while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
			HAL_UART_Transmit_IT( &TerminalInterface, (uint8_t*)Array_2_UART_c,  Size_to_Send + added_length);
	break;

	default: break;
	}//switch (Select_Array)

Select_Array++;
Select_Array = Select_Array %3;
}
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////

uint16_t Process_Rx_Array_UART_DMA(uint8_t *Array,uint16_t Size_of_Array)
{
uint16_t N_shift=0;
if((uint16_t)Array[N_shift]==0x0d
	&&
	Array[(N_shift+1)%Size_of_Array]==0x0A
){return (N_shift); }	//end of message detected

if(Array[N_shift]==0x0A
	&&
	Array[(N_shift+1)%Size_of_Array]==0x0d
){return (N_shift); }	//end of message detected
return (Size_of_Array+1);
}

///////////////////////////////////////////////

uint32_t RTC_update_and_Terminal(uint32_t Period_update_ms)
{
	static uint8_t Sekunden=0;
	static uint8_t Minuten=0;
	static uint8_t Uhr=0;
	static	uint32_t Tick_old=0;
    //uint16_t cnt=0;
    extern RTC_DateTypeDef DateToUpdate;
    extern RTC_TimeTypeDef sTime;
    //					    0,//uint8_t Hours; Max_Data=12 if the RTC_HourFormat_12; Max_Data=23 if the RTC_HourFormat_24
    //						0,//uint8_t Minutes; Max_Data = 59
    //						0,//uint8_t Seconds; Max_Data = 59 */
    //						0,//uint8_t TimeFormat;Specifies the RTC AM/PM Time.
    //						0,//uint32_t SecondFraction;parameter corresponds to a time unit range between [0-1] Second with [1 Sec / SecondFraction +1] granularity
    //						0,//uint32_t DayLightSaving;  This interface is deprecated.
    //						0//uint32_t StoreOperation;

  if(
	  (HAL_GetTick() - Tick_old) > 1999
	){
	  Tick_old=HAL_GetTick();
	  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	  HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
	 }

	if(Sekunden != sTime.Seconds){
		Sekunden = sTime.Seconds;
		Minuten  = sTime.Minutes;
		Uhr      = sTime.Hours;
		Get_Time_output(&Uhr, &Minuten, &Sekunden);
		//cnt++;
		}
	//return (cnt);
	return (Tick_old);
}

////////////////////////////////////////////////////////////////////////////

void GPIO_Blink_Test(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t Count_of_Blink, uint16_t Period_of_blink_ms)
{
	for(uint8_t cnt=0;cnt<Count_of_Blink;cnt++)
	{
	HAL_GPIO_TogglePin(GPIOx, GPIO_Pin );
	HAL_Delay(Period_of_blink_ms);
	}
	  //HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/////////////////////////////////////////////////////////////////////////

void Get_Time(void)
{
///Time & Date output
	char Array_char_x_64[64]={};
	uint16_t Length_Msg;
	extern RTC_TimeTypeDef sTime;
	//					    0,//uint8_t Hours; Max_Data=12 if the RTC_HourFormat_12; Max_Data=23 if the RTC_HourFormat_24
	//						0,//uint8_t Minutes; Max_Data = 59
	//						0,//uint8_t Seconds; Max_Data = 59 */
	//						0,//uint8_t TimeFormat;Specifies the RTC AM/PM Time.
	//						0,//uint32_t SecondFraction;parameter corresponds to a time unit range between [0-1] Second with [1 Sec / SecondFraction +1] granularity
	//						0,//uint32_t DayLightSaving;  This interface is deprecated.
	//						0//uint32_t StoreOperation;
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
			Length_Msg=sprintf(Array_char_x_64,
								"   *  System Time %d.%d.%02d \n\r", //  System
								sTime.Hours, sTime.Minutes, sTime.Seconds);
			HAL_UART_Transmit( &TerminalInterface, (uint8_t*)(Array_char_x_64), Length_Msg,5);
			while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
}


//////////////////////////////////////////////////////////////////////////////////

void Get_Time_output(uint8_t *Uhren,uint8_t *Minutn,uint8_t *Sekundn)
{
//Time output
	char Array_char_x_32[32]={0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08};
	uint16_t Length_Msg;
	extern RTC_TimeTypeDef sTime;
	//		0,//uint8_t Hours;   Max_Data=12 if the RTC_HourFormat_12; Max_Data=23 if the RTC_HourFormat_24
	//		0,//uint8_t Minutes; Max_Data = 59
	//		0,//uint8_t Seconds; Max_Data = 59 */
	//		0,//uint8_t TimeFormat;Specifies the RTC AM/PM Time.
	//		0,//uint32_t SecondFraction;parameter corresponds to a time unit range between [0-1] Second with [1 Sec / SecondFraction +1] granularity
	//		0,//uint32_t DayLightSaving;  This interface is deprecated.
	//		0 //uint32_t StoreOperation;

		while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
		Length_Msg=sprintf( 8+Array_char_x_32 ,
							"%02d.%02d.%02d", //  System
							*Uhren, *Minutn, *Sekundn);//	sTime.Hours, sTime.Minutes, sTime.Seconds

		HAL_UART_Transmit_IT( &TerminalInterface, (uint8_t*)(Array_char_x_32), Length_Msg+8);
		//CDC_Transmit_FS  (                   (uint8_t*)(Array_char_x_32), Length_Msg+8  );
		LCD_SetPos(0, 1);	            HAL_Delay(1);
		LCD_String(8+Array_char_x_32);  HAL_Delay(1);
		//while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
}

////////////////////////////////////////////////////////////////////////

void Get_Date(void)
{
///Date output
	char Array_char_x_32[32]={};
	uint16_t Length_Msg;
	extern RTC_DateTypeDef DateToUpdate;

HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
Length_Msg=sprintf(Array_char_x_32,
					"   *  Date %02d.%02d.20%02d\n\r",
					DateToUpdate.Date, DateToUpdate.Month, DateToUpdate.Year);
//CDC_Transmit_FS((uint8_t*)Array_for_Messages, strlen(Array_for_Messages));//to_usb
HAL_UART_Transmit( &TerminalInterface, (uint8_t*)(Array_char_x_32), Length_Msg,2);
//while(TerminalInterface.gState != HAL_UART_STATE_READY){;}
}


//////////////////////////////////////////////////////////////////////////////////////////


uint16_t CAN_GetState(CAN_HandleTypeDef *hcan, char* String)
{
	  uint8_t state = (uint8_t)(hcan->State);
	  uint16_t Length_String;
switch(state)
{
case HAL_CAN_STATE_RESET:
	Length_String =sprintf(String,"CAN_not_yet_initialized_or_disabled\n\r\n\r");
	return (Length_String);
	break;

case HAL_CAN_STATE_READY:
	Length_String =sprintf(String,"CAN_STATE_READY\n\r\n\r");
	return (Length_String);
	break;

case HAL_CAN_STATE_LISTENING:
	Length_String =sprintf(String,"CAN_STATE_LISTENING\n\r\n\r");
	return (Length_String);
	break;

case HAL_CAN_STATE_SLEEP_PENDING:
	Length_String =sprintf(String,"CAN_STATE_SLEEP_PENDING\n\r\n\r");
	return (Length_String);
	break;

case HAL_CAN_STATE_SLEEP_ACTIVE:
	Length_String =sprintf(String,"CAN_STATE_SLEEP_ACTIVE\n\r\n\r");
	return (Length_String);
	break;

case HAL_CAN_STATE_ERROR:
	Length_String =sprintf(String,"CAN_error_state\n\r\n\r");
	return (Length_String);
	break;

default:	Length_String =sprintf(String,"CAN_UNknown_state\n\r\n\r");
	return (Length_String);
	break;
}

//if ((state == HAL_CAN_STATE_READY) ||
//  (state == HAL_CAN_STATE_LISTENING))
//{
///* Check sleep mode acknowledge flag */
//if ((hcan->Instance->MSR & CAN_MSR_SLAK) != 0U)
//{
//  /* Sleep mode is active */
//  state = HAL_CAN_STATE_SLEEP_ACTIVE;
//  Length_String =sprintf(String,"CAN_STATE_SLEEP_ACTIVE___READY_or_LISTENING \n\r\n\r");
//}
///* Check sleep mode request flag */
//else if ((hcan->Instance->MCR & CAN_MCR_SLEEP) != 0U)
//{
//  /* Sleep mode request is pending */
//  state = HAL_CAN_STATE_SLEEP_PENDING;
//  Length_String =sprintf(String,"CAN_STATE_SLEEP_PENDING___READY_or_LISTENING \n\r\n\r");
//}else{/* Neither sleep mode request nor sleep mode acknowledge */}
//
//
//
//
//}
return (Length_String);
}

////////////////////////////////////

//void Datum_Time_from_PC(void)
void Datum_Time_from_PC(
						RTC_DateTypeDef Date_Upd,
						RTC_TimeTypeDef sTime_Set
						)
 {
//	  RTC_DateTypeDef Date_Upd;
//	  RTC_TimeTypeDef sTime_Set;

	  char Macro_Time[] = {__TIME__};
	  char Macro_Date[] = {__DATE__};
	  uint16_t Lng =  sizeof(__DATE__);
	  Date_Upd.Year = ( (uint8_t)(*(Macro_Date + Lng -2) ) - 0x30 )*1 +  ((uint8_t)(*(Macro_Date + Lng -3)) - 0x30 )*10;

	  if(
			  *(Macro_Date + Lng -8)==0x31
			  ||
			  *(Macro_Date + Lng -8)==0x31
		){
		   Date_Upd.Date = (*(Macro_Date + Lng -8)-0x30)*10 + (*(Macro_Date + Lng -7)-0x30)*1;
	  	 } else {
	  		 	 Date_Upd.Date = *(Macro_Date + Lng -7)-0x30;
	  	 	    }


	  if(*(Macro_Date + 0)=='J')
	  {
	  	if(*(Macro_Date + 1)=='a'){Date_Upd.Month = 1;}
	  	if(*(Macro_Date + 2)=='n'){Date_Upd.Month = 6;}
	  	if(*(Macro_Date + 2)=='l'){Date_Upd.Month = 7;}
	  }

	  if(*(Macro_Date + 0)=='F'){Date_Upd.Month = 2;}

	  if(*(Macro_Date + 0)=='M')
	  	{
	  	if(*(Macro_Date + 2)=='r'){Date_Upd.Month = 3;}
	  	if(*(Macro_Date + 2)=='y'){Date_Upd.Month = 5;}
	  	}

	  if(*(Macro_Date + 0)=='A')
	  {
	  	if(*(Macro_Date + 1)=='p'){Date_Upd.Month = 4;}
	  	if(*(Macro_Date + 1)=='u'){Date_Upd.Month = 8;}
	  }

	  if(*(Macro_Date + 0)=='S'){Date_Upd.Month = 9;}
	  if(*(Macro_Date + 0)=='O'){Date_Upd.Month = 10;}
	  if(*(Macro_Date + 0)=='N'){Date_Upd.Month = 11;}
	  if(*(Macro_Date + 0)=='D'){Date_Upd.Month = 12;}
	  HAL_RTC_SetDate(&hrtc, &Date_Upd, RTC_FORMAT_BIN);

	  	sTime_Set.Hours   = (*(Macro_Time+0)-0x30)*10 + (*(Macro_Time +1)-0x30)*1;
	  	sTime_Set.Minutes = (*(Macro_Time+3)-0x30)*10 + (*(Macro_Time +4)-0x30)*1;
	  	sTime_Set.Seconds = (*(Macro_Time+6)-0x30)*10 + (*(Macro_Time +7)-0x30)*1;
	  	HAL_RTC_SetTime(&hrtc, &sTime_Set,        RTC_FORMAT_BIN);

	    HAL_RTC_GetDate(&hrtc, &Date_Upd, RTC_FORMAT_BIN);
	    HAL_RTC_GetTime(&hrtc, &sTime_Set,        RTC_FORMAT_BIN);

 }

////////////////////////////////////
