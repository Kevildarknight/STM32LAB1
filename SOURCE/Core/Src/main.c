/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body - Simplified Analog Clock Implementation
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

// Function prototypes
void clearAllClock(void);
void setNumberOnClock(int num);
void clearNumberOnClock(int num);

// Exercise 7: Clear all LEDs
void clearAllClock(void)
{
    HAL_GPIO_WritePin(GPIOA, LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                            |LED5_Pin|LED6_Pin|LED7_Pin|LED8_Pin
                            |LED9_Pin|LED10_Pin|LED11_Pin|LED12_Pin, GPIO_PIN_SET);
}

// Exercise 8: Set specific LED based on number (0-11)
void setNumberOnClock(int num)
{
    switch(num)
    {
        case 0:  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);   break;  // 12 o'clock position
        case 1:  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);   break;  // 1 o'clock position
        case 2:  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);   break;  // 2 o'clock position
        case 3:  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);   break;  // 3 o'clock position
        case 4:  HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET);   break;  // 4 o'clock position
        case 5:  HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, GPIO_PIN_RESET);   break;  // 5 o'clock position
        case 6:  HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, GPIO_PIN_RESET);   break;  // 6 o'clock position
        case 7:  HAL_GPIO_WritePin(LED8_GPIO_Port, LED8_Pin, GPIO_PIN_RESET);   break;  // 7 o'clock position
        case 8:  HAL_GPIO_WritePin(LED9_GPIO_Port, LED9_Pin, GPIO_PIN_RESET);   break;  // 8 o'clock position
        case 9:  HAL_GPIO_WritePin(LED10_GPIO_Port, LED10_Pin, GPIO_PIN_RESET); break;  // 9 o'clock position
        case 10: HAL_GPIO_WritePin(LED11_GPIO_Port, LED11_Pin, GPIO_PIN_RESET); break;  // 10 o'clock position
        case 11: HAL_GPIO_WritePin(LED12_GPIO_Port, LED12_Pin, GPIO_PIN_RESET); break;  // 11 o'clock position
        default: break;
    }
}

// Exercise 9: Clear specific LED based on number (0-11)
void clearNumberOnClock(int num)
{
    switch(num)
    {
        case 0:  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);   break;
        case 1:  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);   break;
        case 2:  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);   break;
        case 3:  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);   break;
        case 4:  HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET);   break;
        case 5:  HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, GPIO_PIN_SET);   break;
        case 6:  HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, GPIO_PIN_SET);   break;
        case 7:  HAL_GPIO_WritePin(LED8_GPIO_Port, LED8_Pin, GPIO_PIN_SET);   break;
        case 8:  HAL_GPIO_WritePin(LED9_GPIO_Port, LED9_Pin, GPIO_PIN_SET);   break;
        case 9:  HAL_GPIO_WritePin(LED10_GPIO_Port, LED10_Pin, GPIO_PIN_SET); break;
        case 10: HAL_GPIO_WritePin(LED11_GPIO_Port, LED11_Pin, GPIO_PIN_SET); break;
        case 11: HAL_GPIO_WritePin(LED12_GPIO_Port, LED12_Pin, GPIO_PIN_SET); break;
        default: break;
    }
}

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Simplified clock system variables
typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} ClockTime_t;

// Initialize clock
ClockTime_t clock_time = {12, 00, 00};

/**
 * @brief Calculate hour hand position with smooth movement
 * @param hour: Current hour (1-12)
 * @param minute: Current minute (0-59) for smooth hour hand movement
 * @retval LED position (0-11) where 0=1 o'clock, 11=12 o'clock
 */
uint8_t getHourHandPosition(uint8_t hour, uint8_t minute) {
    // Convert to 12-hour format
    uint8_t h = (hour == 0) ? 12 : hour;
    if (h > 12) h -= 12;

    // Calculate position: 1 o'clock = 0, 2 o'clock = 1, ..., 12 o'clock = 11
    uint8_t position = (h - 1) % 12;  // h=1->0, h=2->1, ..., h=12->11

    return position;
}

/**
 * @brief Calculate minute hand position
 * @param minute: Current minute (0-59)
 * @retval LED position (0-11) where 0=1 o'clock, 11=12 o'clock
 */
uint8_t getMinuteHandPosition(uint8_t minute) {
    // Map 60 minutes to 12 positions
    // 0-4 min -> 12 o'clock (pos 11), 5-9 min -> 1 o'clock (pos 0), etc.
    uint8_t temp_pos = (minute * 12) / 60;  // 0-11

    // Adjust so that 0 minutes points to 12 o'clock (pos 11)
    if (temp_pos == 0) {
        return 11;  // 0-4 minutes -> 12 o'clock position (LED12)
    } else {
        return temp_pos - 1;  // 5-9 min -> pos 0 (1 o'clock), etc.
    }
}

/**
 * @brief Calculate second hand position
 * @param second: Current second (0-59)
 * @retval LED position (0-11) where 0=1 o'clock, 11=12 o'clock
 */
uint8_t getSecondHandPosition(uint8_t second) {
    // Map 60 seconds to 12 positions
    // 0-4 sec -> 12 o'clock (pos 11), 5-9 sec -> 1 o'clock (pos 0), etc.
    uint8_t temp_pos = (second * 12) / 60;  // 0-11

    // Adjust so that 0 seconds points to 12 o'clock (pos 11)
    if (temp_pos == 0) {
        return 11;  // 0-4 seconds -> 12 o'clock position (LED12)
    } else {
        return temp_pos - 1;  // 5-9 sec -> pos 0 (1 o'clock), etc.
    }
}

/**
 * @brief Display the analog clock on LEDs
 * Shows hour, minute, and second hands simultaneously
 */
void displayAnalogClock(void) {
    // Clear all LEDs first
    clearAllClock();
    HAL_Delay(1);
    // Calculate hand positions
    uint8_t hour_pos = getHourHandPosition(clock_time.hours, clock_time.minutes);
    uint8_t minute_pos = getMinuteHandPosition(clock_time.minutes);
    uint8_t second_pos = getSecondHandPosition(clock_time.seconds);

    // Light up the LEDs for each hand
    setNumberOnClock(hour_pos);    // Hour hand (moves slowly)
    setNumberOnClock(minute_pos);  // Minute hand (moves every 5 minutes)
    setNumberOnClock(second_pos);  // Second hand (moves every 5 seconds)
}

/**
 * @brief Update the clock time (increment by 1 second)
 */
void updateClockTime(void) {
    clock_time.seconds++;

    if (clock_time.seconds >= 60) {
        clock_time.seconds = 0;
        clock_time.minutes++;

        if (clock_time.minutes >= 60) {
            clock_time.minutes = 0;
            clock_time.hours++;

            if (clock_time.hours > 12) {
                clock_time.hours = 1;  // 12-hour format
            }
        }
    }
}

/**
 * @brief Set the clock time manually
 * @param hours: Hour (1-12)
 * @param minutes: Minutes (0-59)
 * @param seconds: Seconds (0-59)
 */
void setClockTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    clock_time.hours = (hours > 12) ? (hours - 12) : hours;
    if (clock_time.hours == 0) clock_time.hours = 12;  // Handle 0 -> 12

    clock_time.minutes = (minutes < 60) ? minutes : 0;
    clock_time.seconds = (seconds < 60) ? seconds : 0;
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

  /* USER CODE BEGIN 2 */

  // Exercise 10: Initialize the analog clock system
  setClockTime(8, 30, 59);

  // Display initial clock state
//  displayAnalogClock();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    // Exercise 10: Simple analog clock loop
    // Update time every 1 second
    updateClockTime();        // Increment time by 1 second
    displayAnalogClock();     // Update LED display
    HAL_Delay(100);         // Wait 1 second

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitStructure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level - SET for active low LEDs (turn OFF initially) */
  HAL_GPIO_WritePin(GPIOA, LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                          |LED5_Pin|LED6_Pin|LED7_Pin|LED8_Pin
                          |LED9_Pin|LED10_Pin|LED11_Pin|LED12_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin LED4_Pin
                           LED5_Pin LED6_Pin LED7_Pin LED8_Pin
                           LED9_Pin LED10_Pin LED11_Pin LED12_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                          |LED5_Pin|LED6_Pin|LED7_Pin|LED8_Pin
                          |LED9_Pin|LED10_Pin|LED11_Pin|LED12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void testAllLEDs(void) {
    for (int i = 0; i < 12; i++) {
        clearAllClock();
        setNumberOnClock(i);
        HAL_Delay(500);
    }
    clearAllClock();
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
