#include "Encoder.h"
#include "main.h"
#define Tim_Encoder Tim_1_Encoder

extern
TIM_HandleTypeDef Tim_Encoder;

static uint16_t newCount;
static uint16_t prevCount;

void Encoder_Init(void) {
  HAL_TIM_Encoder_Start(&Tim_Encoder, TIM_CHANNEL_ALL);
  HAL_Delay(1);
  __HAL_TIM_SET_COUNTER(&Tim_Encoder, 0x8000);
  newCount=__HAL_TIM_GET_COUNTER(&Tim_Encoder);
  newCount=newCount>>1;
  prevCount=newCount;
}

uint16_t Encoder_Read() {
  uint16_t val = __HAL_TIM_GET_COUNTER(&Tim_Encoder);
  return (val >> 1);
}

Encoder_Status Encoder_Get_Status() {
	  newCount = Encoder_Read();
	  if (newCount != prevCount)
	  {
	    if (newCount > prevCount)
	    	{
	    	prevCount = newCount;
	    	return (Incremented);
	    	} else {
	    			prevCount = newCount;
	    			return (Decremented);
	    			}
	  }
	  return (Neutral);
}

/**
  * @brief TIM3 Initialization Function (Encoder Mode)
  * @param None
  * @retval None
  */
void Encoder_Config(void)
{
  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  HAL_TIM_Base_MspDeInit(&Tim_Encoder);

  Tim_Encoder.Instance = TIM3;
  Tim_Encoder.Instance = TIM1;
  Tim_Encoder.Init.Prescaler = 0;
  Tim_Encoder.Init.CounterMode = TIM_COUNTERMODE_UP;
  Tim_Encoder.Init.Period = 65535;
  Tim_Encoder.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  Tim_Encoder.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;     // Capture performed each time an edge is detected on the capture input
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;     // Capture performed each time an edge is detected on the capture input
  sConfig.IC2Filter = 0;
  
  HAL_TIM_Encoder_Init(&Tim_Encoder, &sConfig);
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  
  HAL_TIMEx_MasterConfigSynchronization(&Tim_Encoder, &sMasterConfig);
}




