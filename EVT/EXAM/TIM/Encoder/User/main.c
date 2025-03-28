/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2024/02/21
* Description        : Main program body.
*******************************************************************************/

/**
 * @note
 * Encoder rourine
 * The main function initializes and tests an encoder, and continuously prints the encoder position and
 * speed.
 */
#include "debug.h"
#include "lcd.h"
#include "RTC_utils.h"
#include "ADC_utils.h"

/* Global define */

/* Global Variable */
/* I2C Mode Definition */
#define HOST_MODE     0
#define SLAVE_MODE    1

#define Size   6
#define RXAdderss   0x4e
#define TxAdderss   0x4e

/* Global Variable */
u8 TxData[Size] = { 0x61, 0x32, 0x43, 0x54, 0x75, 0x86 };
u8 RxData[5][Size];
char str[32];

volatile int circle = 0, precircle = 0;
volatile uint16_t precnt = 0;
volatile uint32_t time = 0;

extern _calendar_obj calendar;
extern s16 Calibrattion_Val;
	u16 ADC_val;
	s32 val_mv;


/*********************************************************************
 * @fn      IIC_Init
 *
 * @brief   Initializes the IIC peripheral.
 *
 * @return  none
 */
void IIC_Init(u32 bound, u16 address)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef  I2C_InitTSturcture = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    //GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTSturcture.I2C_ClockSpeed = bound;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitTSturcture);

    I2C_Cmd(I2C1, ENABLE);

#if(I2C_MODE == HOST_MODE)
    I2C_AcknowledgeConfig(I2C1, ENABLE);

#endif
}



void Before_while()
{

#if(I2C_MODE == HOST_MODE)
    printf("\r\n I2C Host mode\r\n\n");
    IIC_Init(100000, TxAdderss);//Delay_Ms(100);

    for(uint16_t j =0; j < 5; j++)
     {
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );

    I2C_GenerateSTART(I2C1, ENABLE);

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, 0x4E, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

   for(uint16_t i=0; i< 6;i++ )
    {
        if(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) != RESET)
        {
            I2C_SendData( I2C1, TxData[i] );
        }
    }

    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
    I2C_GenerateSTOP( I2C1, ENABLE );
     }

    LCD_ini();
    Delay_Ms(1);

    sprintf(str,"LCD2004_I2C1");
    LCD_SetPos(0, 0);
    LCD_String(str);//// LCD on I2C1  SDA PB7, SCL PB6

    sprintf(str,"SDA_PB7___SCL__PB6");
    LCD_SetPos(0, 1);
    LCD_String(str);

    sprintf(str,"Encoder");
    LCD_SetPos(0, 2);
    LCD_String(str);

#endif
}


















void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
/*********************************************************************
 * @fn TIM3_IRQHandler
 * 
 * @brief TIM3_IRQHandler function handles the interrupt for TIM3 and updates the "circle" variable based
 * on the current count and auto-reload values of TIM3.
 */
void TIM3_IRQHandler()
{

    volatile uint16_t tempcnt = TIM3->CNT, temparr = TIM3->ATRLR;
    if (TIM_GetITStatus(TIM3, TIM_IT_Update))
    {

        if (tempcnt < temparr / 2)
        {
            circle += 1;
        }
        else
        {
            circle -= 1;
        }
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

/*********************************************************************
 * @fn TIM3_Encoder_Init
 * 
 * @brief function initializes the TIM3 timer as an encoder with specific settings.
 * 
 * @return none
 */
void TIM3_Init()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 0;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}



void Encoder_Init(void) 
{
  //TIM3_Init(); 
 TIM_ClearFlag(TIM3, TIM_FLAG_Update);
 TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
 TIM_SetCounter(TIM3, 0x8000);
 TIM_Cmd(TIM3, ENABLE);
}



int16_t Encoder_Get_Abs_Value() 
{
static  int16_t Encoder_Abs_Value=0;
static uint16_t newCount=0;
static uint16_t prevCount=0;

newCount = TIM_GetCounter(TIM3)>>2;
if (newCount != prevCount)
    {
    if (newCount > prevCount)
        {
        prevCount = newCount;
        return (Encoder_Abs_Value++);
        } else {
                prevCount = newCount;
                return (Encoder_Abs_Value--);
                }
    }
return (Encoder_Abs_Value);
}





/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200*8);
    printf("\r\nSystemClk:%d\r\n", SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID());
    printf("Encoder_PA7_PA6__Timer3___with_I2C_LCD2004\r\n");

    TIM3_Init();
    Encoder_Init() ;
    Before_while();
    RTC_Init();
    
	ADC_Function_Init();
    printf("CalibrattionValue:%d\n\n", Calibrattion_Val);

 
    printf("year/month/day/week/hour/min/sec:\r\n\n");
    printf("%d-%d-%d  %d  %d:%d:%d\r\n", calendar.w_year, calendar.w_month, calendar.w_date,
            calendar.week, calendar.hour, calendar.min, calendar.sec);

    LCD_SetPos(0, 3);
    sprintf(str,"%d.%d.%d",calendar.w_date, calendar.w_month, calendar.w_year);
    LCD_String(str);

    RTC_Get();   
    uint8_t Curr_Sec =calendar.sec;
    int16_t prev_Value=0;
    while (1)
    {
    int16_t curr_Enc;
    curr_Enc = Encoder_Get_Abs_Value();
    if (prev_Value!= curr_Enc)
        {
        prev_Value = curr_Enc;
        if(curr_Enc<0)
        {
            printf("Abs_Value=-%03d \r\n", -curr_Enc);
            LCD_SetPos(8, 2);
            sprintf(str,"     ");
            LCD_String(str);

            LCD_SetPos(8, 2);
            sprintf(str,"-%d",-curr_Enc);  
            LCD_String(str); 
        }else{
               printf("Abs_Value=+%03d \r\n", curr_Enc);
               LCD_SetPos(8, 2);
               sprintf(str,"     ");
               LCD_String(str);

               LCD_SetPos(8, 2);
               sprintf(str,"+%d",curr_Enc);
               LCD_String(str);  
             }

        }

    RTC_Get();  
    if(calendar.sec == (Curr_Sec+2)%60)
    {
    Curr_Sec =calendar.sec;
    LCD_SetPos(12, 3);
    sprintf(str,"%02d.%02d.%02d",calendar.hour, calendar.min, calendar.sec);
    LCD_String(str);
    }
         
    
    }//while (1)

}//main(void)
