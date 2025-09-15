/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
/* Traffic Light LEDs */
#define RED1_Pin GPIO_PIN_1
#define RED1_GPIO_Port GPIOA
#define YELLOW1_Pin GPIO_PIN_2
#define YELLOW1_GPIO_Port GPIOA
#define GREEN1_Pin GPIO_PIN_3
#define GREEN1_GPIO_Port GPIOA
#define RED2_Pin GPIO_PIN_4
#define RED2_GPIO_Port GPIOA
#define YELLOW2_Pin GPIO_PIN_5
#define YELLOW2_GPIO_Port GPIOA
#define GREEN2_Pin GPIO_PIN_6
#define GREEN2_GPIO_Port GPIOA

/* East-West 7-Segment Display */
#define SEG_A_Pin GPIO_PIN_0
#define SEG_A_GPIO_Port GPIOB
#define SEG_B_Pin GPIO_PIN_1
#define SEG_B_GPIO_Port GPIOB
#define SEG_C_Pin GPIO_PIN_2
#define SEG_C_GPIO_Port GPIOB
#define SEG_D_Pin GPIO_PIN_3
#define SEG_D_GPIO_Port GPIOB
#define SEG_E_Pin GPIO_PIN_4
#define SEG_E_GPIO_Port GPIOB
#define SEG_F_Pin GPIO_PIN_5
#define SEG_F_GPIO_Port GPIOB
#define SEG_G_Pin GPIO_PIN_6
#define SEG_G_GPIO_Port GPIOB

/* North-South 7-Segment Display */
#define SEG_A1_Pin GPIO_PIN_7
#define SEG_A1_GPIO_Port GPIOB
#define SEG_B1_Pin GPIO_PIN_8
#define SEG_B1_GPIO_Port GPIOB
#define SEG_C1_Pin GPIO_PIN_9
#define SEG_C1_GPIO_Port GPIOB
#define SEG_D1_Pin GPIO_PIN_10
#define SEG_D1_GPIO_Port GPIOB
#define SEG_E1_Pin GPIO_PIN_11
#define SEG_E1_GPIO_Port GPIOB
#define SEG_F1_Pin GPIO_PIN_12
#define SEG_F1_GPIO_Port GPIOB
#define SEG_G1_Pin GPIO_PIN_13
#define SEG_G1_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
