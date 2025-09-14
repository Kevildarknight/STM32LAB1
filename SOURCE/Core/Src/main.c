/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : 4-Way Traffic Light Controller
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    NORTH_SOUTH_GREEN,
    NORTH_SOUTH_YELLOW,
    NORTH_SOUTH_RED,
    EAST_WEST_GREEN,
    EAST_WEST_YELLOW,
    EAST_WEST_RED
} TrafficState_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define GREEN_TIME_MS    3000   // 3 seconds
#define YELLOW_TIME_MS   2000   // 2 seconds
#define RED_TIME_MS      5000   // 5 seconds
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TrafficState_t current_state = NORTH_SOUTH_GREEN;
uint32_t state_start_time = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void traffic_light_control(void);
void set_north_south_lights(uint8_t red, uint8_t yellow, uint8_t green);
void set_east_west_lights(uint8_t red, uint8_t yellow, uint8_t green);
void turn_off_all_lights(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief Control North-South traffic lights
 * @param red: 1 to turn on, 0 to turn off
 * @param yellow: 1 to turn on, 0 to turn off
 * @param green: 1 to turn on, 0 to turn off
 */
void set_north_south_lights(uint8_t red, uint8_t yellow, uint8_t green)
{
    HAL_GPIO_WritePin(GPIOA, RED1_Pin, red ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin, yellow ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, green ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief Control East-West traffic lights
 * @param red: 1 to turn on, 0 to turn off
 * @param yellow: 1 to turn on, 0 to turn off
 * @param green: 1 to turn on, 0 to turn off
 */
void set_east_west_lights(uint8_t red, uint8_t yellow, uint8_t green)
{
    HAL_GPIO_WritePin(GPIOA, RED2_Pin, red ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, YELLOW2_Pin, yellow ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, green ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief Turn off all traffic lights
 */
void turn_off_all_lights(void)
{
    set_north_south_lights(0, 0, 0);
    set_east_west_lights(0, 0, 0);
}

/**
 * @brief Main traffic light control logic
 */
void traffic_light_control(void)
{
    uint32_t current_time = HAL_GetTick();
    uint32_t elapsed_time = current_time - state_start_time;

    switch(current_state)
    {
        case NORTH_SOUTH_GREEN:
            // North-South: GREEN, East-West: RED
            set_north_south_lights(0, 0, 1);  // Green ON
            set_east_west_lights(1, 0, 0);    // Red ON

            if(elapsed_time >= GREEN_TIME_MS)
            {
                current_state = NORTH_SOUTH_YELLOW;
                state_start_time = current_time;
            }
            break;

        case NORTH_SOUTH_YELLOW:
            // North-South: YELLOW, East-West: RED
            set_north_south_lights(0, 1, 0);  // Yellow ON
            set_east_west_lights(1, 0, 0);    // Red ON

            if(elapsed_time >= YELLOW_TIME_MS)
            {
                current_state = NORTH_SOUTH_RED;
                state_start_time = current_time;
            }
            break;

        case NORTH_SOUTH_RED:
            // North-South: RED, East-West: RED (All Red phase)
            set_north_south_lights(1, 0, 0);  // Red ON
            set_east_west_lights(1, 0, 0);    // Red ON

            if(elapsed_time >= RED_TIME_MS)
            {
                current_state = EAST_WEST_GREEN;
                state_start_time = current_time;
            }
            break;

        case EAST_WEST_GREEN:
            // East-West: GREEN, North-South: RED
            set_east_west_lights(0, 0, 1);    // Green ON
            set_north_south_lights(1, 0, 0);  // Red ON

            if(elapsed_time >= GREEN_TIME_MS)
            {
                current_state = EAST_WEST_YELLOW;
                state_start_time = current_time;
            }
            break;

        case EAST_WEST_YELLOW:
            // East-West: YELLOW, North-South: RED
            set_east_west_lights(0, 1, 0);    // Yellow ON
            set_north_south_lights(1, 0, 0);  // Red ON

            if(elapsed_time >= YELLOW_TIME_MS)
            {
                current_state = EAST_WEST_RED;
                state_start_time = current_time;
            }
            break;

        case EAST_WEST_RED:
            // East-West: RED, North-South: RED (All Red phase)
            set_east_west_lights(1, 0, 0);    // Red ON
            set_north_south_lights(1, 0, 0);  // Red ON

            if(elapsed_time >= RED_TIME_MS)
            {
                current_state = NORTH_SOUTH_GREEN;
                state_start_time = current_time;
            }
            break;

        default:
            current_state = NORTH_SOUTH_GREEN;
            state_start_time = current_time;
            break;
    }
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

  // Initialize traffic light system
  turn_off_all_lights();
  state_start_time = HAL_GetTick();
  current_state = NORTH_SOUTH_GREEN;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    traffic_light_control();
    HAL_Delay(100);  // Small delay to prevent excessive CPU usage
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
  * in the RCC_OscInitTypeDef structure.
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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RED1_Pin|YELLOW1_Pin|GREEN1_Pin|RED2_Pin
                          |YELLOW2_Pin|GREEN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RED1_Pin YELLOW1_Pin GREEN1_Pin RED2_Pin
                           YELLOW2_Pin GREEN2_Pin */
  GPIO_InitStruct.Pin = RED1_Pin|YELLOW1_Pin|GREEN1_Pin|RED2_Pin
                          |YELLOW2_Pin|GREEN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
