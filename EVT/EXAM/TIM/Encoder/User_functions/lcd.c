#include "lcd.h"
#include "main.h"
#include "rtc.h"
#include <stdio.h>
#include "format_out.h"

//------------------------------------------------
uint8_t buf[1]={0};
extern I2C_HandleTypeDef hi2c1;
char str1[100];
uint8_t portlcd; //������ ��� �������� ������ ����� ���������� ����������
//------------------------------------------------
__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
	micros *=(SystemCoreClock / 1000000) / 5;
	while (micros--);
//	DWT->CYCCNT = 0; // reset the counter
//	while( (DWT->CYCCNT / (HAL_RCC_GetSysClockFreq()/1000000) ) < (micros-1) );
}
//------------------------------------------------
void LCD_WriteByteI2CLCD(uint8_t bt)
{
	buf[0]=bt;
	HAL_I2C_Master_Transmit(&hi2c1,(uint16_t) 0x4E,buf,1,1000);
}
//------------------------------------------------
void sendhalfbyte(uint8_t c)
{
	c<<=4;
	e_set();//�������� ����� E
	DelayMicro(50);
	LCD_WriteByteI2CLCD(portlcd|c);
	e_reset();//��������� ����� E
	DelayMicro(50);
}
//------------------------------------------------
void sendbyte(uint8_t c, uint8_t mode)
{
	if(mode==0) rs_reset();
	else rs_set();
	uint8_t hc=0;
	hc=c>>4;
	sendhalfbyte(hc);sendhalfbyte(c);
}
//------------------------------------------------
void LCD_Clear(void)
{
	sendbyte(0x01,0);
	HAL_Delay(2);
}
//------------------------------------------------
void LCD_SendChar(char ch)
{
	sendbyte(ch,1);
}
//------------------------------------------------
void LCD_String(char* st)
{
	uint8_t i=0;
	while(st[i]!=0)
	{
		sendbyte(st[i],1);
		i++;
	}
}
//------------------------------------------------
void LCD_SetPos(uint8_t x, uint8_t y)
{
	switch(y)
	{
		case 0:
			sendbyte(x|0x80,0);
			HAL_Delay(1);
			break;
		case 1:
			sendbyte((0x40+x)|0x80,0);
			HAL_Delay(1);
			break;
		case 2:
			sendbyte((0x14+x)|0x80,0);
			HAL_Delay(1);
			break;
		case 3:
			sendbyte((0x54+x)|0x80,0);
			HAL_Delay(1);
			break;
	}
}
//------------------------------------------------
void LCD_ini(void)
{
	HAL_Delay(15);
	sendhalfbyte(0x03);
	HAL_Delay(4);
	sendhalfbyte(0x03);
	DelayMicro(100);
	sendhalfbyte(0x03);
	HAL_Delay(1);
	sendhalfbyte(0x02);
	HAL_Delay(1);
	sendbyte(0x28,0);//����� 4 ���, 2 ����� (��� ������ �������� ������� ��� 4 �����, ����� 5�8	
	HAL_Delay(1);
	sendbyte(0x0C,0);//������� �������� (D=1), ������� ������� �� �����
	HAL_Delay(1);
	sendbyte(0x01,0);//������ �����
	HAL_Delay(2);
	sendbyte(0x06,0);//����� �����
	HAL_Delay(1);
	setled();//���������
	setwrite();//������
}


//////////////////////////////////////////////////
void Get_Date_to_LCD(uint8_t X,uint8_t Y)
{
///Date output
	static	char String_to_LCD[32]={};
	//uint16_t Length_Msg;
	static	RTC_DateTypeDef DateUpdated = {0};
	static	RTC_TimeTypeDef TimeUpdated = {0};
	HAL_RTC_GetDate(&hrtc, &DateUpdated, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &TimeUpdated, RTC_FORMAT_BIN);//Length_Msg=
		sprintf(String_to_LCD,
					"Datum %d.%d.20%02d",
					DateUpdated.Date, DateUpdated.Month, DateUpdated.Year);
 LCD_SetPos(X, Y);	HAL_Delay(2); LCD_String(String_to_LCD);HAL_Delay(5);

}


//////////////////////////////////////////////////
void Get_Time_to_LCD(uint8_t X,uint8_t Y)
{
static	char String_to_LCD[32]={};
	//uint16_t Length_Msg;
static	RTC_DateTypeDef DateUpdated = {0};
static	RTC_TimeTypeDef TimeUpdated = {0};
HAL_RTC_GetTime(&hrtc, &TimeUpdated, RTC_FORMAT_BIN);
HAL_RTC_GetDate(&hrtc, &DateUpdated, RTC_FORMAT_BIN);
//Length_Msg=
		sprintf(String_to_LCD,
					"Time %02d.%02d.%02d",
					TimeUpdated.Hours, TimeUpdated.Minutes, TimeUpdated.Seconds);
 LCD_SetPos(X, Y);	HAL_Delay(2);
 LCD_String(String_to_LCD);HAL_Delay(5);
}
//////////////////////////////////////////////////

uint16_t RTC_update_and_send2LCD(uint32_t Period_update_ms,uint8_t X, uint8_t Y)
{
	static uint8_t Sekunden=0;
	static uint8_t Minuten=0;
	static uint8_t Uhr=0;
	static	uint32_t Tick_old=0;
    uint16_t cnt=0;
    static	char String_to_LCD[32]={};
extern    RTC_DateTypeDef DateToUpdate ;
extern       RTC_TimeTypeDef sTime;
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
	  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	  HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
	 }

	if(Sekunden != sTime.Seconds){
		Sekunden = sTime.Seconds;
		Minuten  = sTime.Minutes;
		Uhr      = sTime.Hours;

		sprintf(String_to_LCD,
				"Time %02d.%02d.%02d",
				Uhr, Minuten, Sekunden);//	sTime.Hours, sTime.Minutes, sTime.Seconds
		LCD_SetPos(X, Y);	HAL_Delay(2); LCD_String(String_to_LCD);HAL_Delay(5);
		cnt++;
		}
	return (cnt);
}

//////////////////////////////////////////////////
void Logo_to_1602LCD(void){
	const	char XPOMOC[] = "XPOMOC_CANOpen";
	const	char WC[] = "LSS_Master";
extern	char String_LCD[32];

  sprintf(String_LCD,XPOMOC); LCD_SetPos(0, 0);	HAL_Delay(1); LCD_String(String_LCD);//// LCD on I2C1  SDA PB7, SCL PB6
  sprintf(String_LCD,WC);     LCD_SetPos(0, 1);	HAL_Delay(1); LCD_String(String_LCD);
  HAL_Delay(2500);
  sprintf(String_LCD,"NodeID = 0x%2x ",CO_Disco407_Blue);
  LCD_SetPos(0, 0);	HAL_Delay(1); LCD_String(String_LCD);
  HAL_Delay(2000);
  sprintf(String_LCD,"Encoder =  0000 "); LCD_SetPos(0, 0);	HAL_Delay(1); LCD_String(String_LCD);
  HAL_Delay(2500);
  sprintf(String_LCD,"           0000 "); LCD_SetPos(0, 0);	HAL_Delay(5); LCD_String(String_LCD);
  sprintf(String_LCD,"                "); LCD_SetPos(0, 1);	HAL_Delay(5); LCD_String(String_LCD);
}

//////////////////////////////////////////////////

void Datum_to_1602LCD(void){

extern	  char String_LCD[32];
extern    RTC_DateTypeDef DateToUpdate;
HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
sprintf( String_LCD,
		 "%02d.%02d.20%02d",
		 DateToUpdate.Date, DateToUpdate.Month, DateToUpdate.Year);

LCD_SetPos(0, 0);	HAL_Delay(1); LCD_String(String_LCD);//// LCD on I2C1  SDA PB7, SCL PB6
}
//////////////////////////////////////////////////
