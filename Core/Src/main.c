/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Define pour le debug
#define Debug


#define VREF 3300 // Voltage Reference in mV
#define VREF_FACTOR 2650

// For the stockage purpose
#define STOCK_VOLTAGE 3700 // Sotckage voltage in MV
#define TRESHOLD_LIMIT 50 // Variable used for hysteresis

// Used for full charge
#define FULL_CHARGE_VOLTAGE 4100

// Used for Coherence Detection in voltage
#define HIGH_LVL_VOLTAGE 4200
#define LOW_LVL_VOLTAGE 4200
#define ADC_MARGIN 100

// Used for coherence detection in temperature
#define HIGH_LVL_TEMP 2550
#define LOW_LVL_TEMP 999


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define DisSwitch !HAL_GPIO_ReadPin(Full_Charge_Switch_GPIO_Port, Full_Charge_Switch_Pin)

#define HIGH_VOTLAGE !HAL_GPIO_ReadPin(Comp_High_Level_GPIO_Port, Comp_High_Level_Pin)
#define LOW_VOLTAGE !HAL_GPIO_ReadPin(Comp_Low_Level_GPIO_Port, Comp_Low_Level_Pin)
#define HIGH_TEMP !HAL_GPIO_ReadPin(Comp_High_Temp_GPIO_Port, Comp_High_Temp_Pin)
#define LOW_TEMP !HAL_GPIO_ReadPin(Comp_Low_Temp_GPIO_Port, Comp_Low_Temp_Pin)

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t vBat; // Battery Voltage
convertedCH5;

typedef enum {
	IDLE,
	DISCHARGE,
	CHRG_ERROR,
	CHARGE
}ChargeCtrlStates;

typedef enum {
	BLINK,
	ON,
	OFF
}LED_States;

LED_States ST_LD1 = OFF;
LED_States ST_LD2 = OFF;
LED_States ST_LD3 = OFF;


ChargeCtrlStates ChargeCtrlState = IDLE;
ChargeCtrlStates PrevChargeCtrlState = DISCHARGE;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(ST_LD1 == BLINK) HAL_GPIO_TogglePin(UI_LD1_GPIO_Port, UI_LD1_Pin);
	if(ST_LD2 == BLINK) HAL_GPIO_TogglePin(UI_LD2_GPIO_Port, UI_LD2_Pin);
	if(ST_LD3 == BLINK) HAL_GPIO_TogglePin(UI_LD3_GPIO_Port, UI_LD3_Pin);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  ST_LD3 = OFF;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  	uint32_t adc_value_CH5;
	  	uint32_t adc_value_CH6;

	  	ADC_ChannelConfTypeDef sConfig = {0};
	  	sConfig.Channel = ADC_CHANNEL_5; //First we will use channel 5
	  	sConfig.Rank = ADC_REGULAR_RANK_1; // Always rank 1 for single conversions
	  	sConfig.SamplingTime = ADC_SAMPLETIME_24CYCLES_5; // Example sampling time
	  	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	  	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  	sConfig.Offset = 0;

	  	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  	{
	  	//Error_Handler();
	  	}


	  	HAL_ADC_Start(&hadc1); // Start ADC
	  	HAL_ADC_PollForConversion(&hadc1, 100); // Wait for conversion
	  	adc_value_CH5 = HAL_ADC_GetValue(&hadc1); // Read the result
	  	HAL_ADC_Stop(&hadc1); // Stop ADC

	  	sConfig.Channel = ADC_CHANNEL_6;	//Then we will use channel 5
	  	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  	{
	  	//Error_Handler();
	  	}

	  	HAL_ADC_Start(&hadc1); // Start ADC
	  	HAL_ADC_PollForConversion(&hadc1, 100); // Wait for conversion
	  	adc_value_CH6 = HAL_ADC_GetValue(&hadc1); // Read the result
	  	HAL_ADC_Stop(&hadc1); // Stop ADC

	  	convertedCH5 = (adc_value_CH5*VREF)/4096;
	  	vBat = (adc_value_CH6*VREF)/VREF_FACTOR;

	  	char msg[100];

		#ifdef DebugV

			sprintf(msg, "Temperature Voltage %ld mV \r\n", convertedCH5);
			HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

			sprintf(msg, "VBAT value is %ld mV \r\n", vBat);
			HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
		#endif


	  	// ------------ Charge Control ------------
	  	switch(ChargeCtrlState){
	  		case IDLE :
	  			if(vBat > STOCK_VOLTAGE+TRESHOLD_LIMIT && DisSwitch) ChargeCtrlState = DISCHARGE;
	  			else if (vBat < FULL_CHARGE_VOLTAGE-TRESHOLD_LIMIT && !DisSwitch) ChargeCtrlState = CHARGE;
	  			else if (vBat < STOCK_VOLTAGE-TRESHOLD_LIMIT && DisSwitch) ChargeCtrlState = CHARGE;
	  		break;

	  		case DISCHARGE :
	  			if(vBat < STOCK_VOLTAGE) ChargeCtrlState = IDLE;
	  		break;

	  		case CHRG_ERROR:

			break;

	  		case CHARGE:
	  			if(vBat > FULL_CHARGE_VOLTAGE) ChargeCtrlState = IDLE;
	  			else if (vBat > STOCK_VOLTAGE && DisSwitch) ChargeCtrlState = IDLE;
	  		break;
	  	}
	  	if(ChargeCtrlState != PrevChargeCtrlState){
	  		PrevChargeCtrlState = ChargeCtrlState;
	  		switch(ChargeCtrlState){
				case IDLE :
					HAL_GPIO_WritePin(Enable_Charge_GPIO_Port, Enable_Charge_Pin, 0);
					HAL_GPIO_WritePin(Enable_Discharge_GPIO_Port, Enable_Discharge_Pin, 0);

					ST_LD1 = OFF;
					ST_LD2 = ON;

					#ifdef Debug
						sprintf(msg, "ChargeCtrlState : IDLE\n");
					#endif
				break;

				case DISCHARGE :
					HAL_GPIO_WritePin(Enable_Charge_GPIO_Port, Enable_Charge_Pin, 0);
					HAL_GPIO_WritePin(Enable_Discharge_GPIO_Port, Enable_Discharge_Pin, 1);

					ST_LD2 = OFF;
					ST_LD1 = BLINK;

					#ifdef Debug
						sprintf(msg, "ChargeCtrlState : DISCHARGE\n");
					#endif
				break;

				case CHRG_ERROR:
					HAL_GPIO_WritePin(Enable_Charge_GPIO_Port, Enable_Charge_Pin, 0);
					HAL_GPIO_WritePin(Enable_Discharge_GPIO_Port, Enable_Discharge_Pin, 0);

					ST_LD2 = OFF;
					ST_LD1 = OFF;

					#ifdef Debug
						sprintf(msg, "ChargeCtrlState : ERROR\n");
					#endif
				break;

				case CHARGE:
					HAL_GPIO_WritePin(Enable_Charge_GPIO_Port, Enable_Charge_Pin, 1);
					HAL_GPIO_WritePin(Enable_Discharge_GPIO_Port, Enable_Discharge_Pin, 0);

					ST_LD2 = OFF;
					ST_LD1 = ON;

					#ifdef Debug
						sprintf(msg, "ChargeCtrlState : CHARGE\n");
					#endif
				break;
			}
	  	}

	  	//HAL_GPIO_WritePin(Enable_Charge_GPIO_Port, Enable_Charge_Pin, 1);

	// -------------------- LED 1 & 3 CTRL ---------------
	  	if(ST_LD1 == OFF) HAL_GPIO_WritePin(UI_LD1_GPIO_Port, UI_LD1_Pin, 1);
	  	else if (ST_LD1 == ON) HAL_GPIO_WritePin(UI_LD1_GPIO_Port, UI_LD1_Pin, 0);

	  	if(ST_LD2 == OFF) HAL_GPIO_WritePin(UI_LD2_GPIO_Port, UI_LD2_Pin, 1);
		else if (ST_LD2 == ON) HAL_GPIO_WritePin(UI_LD2_GPIO_Port, UI_LD2_Pin, 0);

	  	if(ST_LD3 == OFF) HAL_GPIO_WritePin(UI_LD3_GPIO_Port, UI_LD3_Pin, 1);
		else if (ST_LD3 == ON) HAL_GPIO_WritePin(UI_LD3_GPIO_Port, UI_LD3_Pin, 0);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 32000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 250;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Enable_Discharge_Pin|Enable_Charge_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(UI_LD3_GPIO_Port, UI_LD3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, UI_LD1_Pin|UI_LD2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : Full_Charge_Switch_Pin Comp_High_Level_Pin Comp_Low_Temp_Pin Comp_High_Temp_Pin */
  GPIO_InitStruct.Pin = Full_Charge_Switch_Pin|Comp_High_Level_Pin|Comp_Low_Temp_Pin|Comp_High_Temp_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Enable_Discharge_Pin Enable_Charge_Pin */
  GPIO_InitStruct.Pin = Enable_Discharge_Pin|Enable_Charge_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Comp_Low_Level_Pin */
  GPIO_InitStruct.Pin = Comp_Low_Level_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Comp_Low_Level_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : UI_LD3_Pin */
  GPIO_InitStruct.Pin = UI_LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(UI_LD3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : UI_LD1_Pin UI_LD2_Pin */
  GPIO_InitStruct.Pin = UI_LD1_Pin|UI_LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
