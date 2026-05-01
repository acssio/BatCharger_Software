/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define Full_Charge_Switch_Pin GPIO_PIN_3
#define Full_Charge_Switch_GPIO_Port GPIOA
#define Comp_High_Level_Pin GPIO_PIN_4
#define Comp_High_Level_GPIO_Port GPIOA
#define Comp_Low_Temp_Pin GPIO_PIN_5
#define Comp_Low_Temp_GPIO_Port GPIOA
#define Comp_High_Temp_Pin GPIO_PIN_6
#define Comp_High_Temp_GPIO_Port GPIOA
#define Enable_Discharge_Pin GPIO_PIN_7
#define Enable_Discharge_GPIO_Port GPIOA
#define Comp_Low_Level_Pin GPIO_PIN_0
#define Comp_Low_Level_GPIO_Port GPIOB
#define Enable_Charge_Pin GPIO_PIN_8
#define Enable_Charge_GPIO_Port GPIOA
#define UI_LD3_Pin GPIO_PIN_11
#define UI_LD3_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
#define LD3_Pin GPIO_PIN_3
#define LD3_GPIO_Port GPIOB
#define UI_LD1_Pin GPIO_PIN_4
#define UI_LD1_GPIO_Port GPIOB
#define UI_LD2_Pin GPIO_PIN_5
#define UI_LD2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
