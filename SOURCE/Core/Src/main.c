/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Simple 4-Way Traffic Light Controller (4 States)
  *                   - YELLOW: 2 seconds
  *                   - GREEN: 3 seconds
  *                   - No delay between transitions
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* Traffic Light States (1 to 4) */
typedef enum {
    STATE_1 = 1,    // North-South GREEN, East-West RED
    STATE_2,        // North-South YELLOW, East-West RED
    STATE_3,        // North-South RED, East-West GREEN
    STATE_4         // North-South RED, East-West YELLOW
} TrafficState_t;

/* Timing definitions (in milliseconds) */
#define GREEN_TIME    3000   // 3 seconds
#define YELLOW_TIME   2000   // 2 seconds

/* Global variables */
TrafficState_t current_state = STATE_1;
uint32_t state_timer = 0;

/* Function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void traffic_control(void);
void set_lights(uint8_t ns_red, uint8_t ns_yellow, uint8_t ns_green,
                uint8_t ew_red, uint8_t ew_yellow, uint8_t ew_green);

/**
 * @brief Set traffic light states
 * @param ns_red, ns_yellow, ns_green: North-South lights (1=ON, 0=OFF)
 * @param ew_red, ew_yellow, ew_green: East-West lights (1=ON, 0=OFF)
 */
void set_lights(uint8_t ns_red, uint8_t ns_yellow, uint8_t ns_green,
                uint8_t ew_red, uint8_t ew_yellow, uint8_t ew_green)
{
    // North-South lights (RED1, YELLOW1, GREEN1)
    HAL_GPIO_WritePin(GPIOA, RED1_Pin, ns_red ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin, ns_yellow ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, ns_green ? GPIO_PIN_RESET : GPIO_PIN_SET);

    // East-West lights (RED2, YELLOW2, GREEN2)
    HAL_GPIO_WritePin(GPIOA, RED2_Pin, ew_red ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, YELLOW2_Pin, ew_yellow ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, ew_green ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

/**
 * @brief Main traffic light control function
 */
void traffic_control(void)
{
    uint32_t current_time = HAL_GetTick();
    uint32_t elapsed_time = current_time - state_timer;

    switch(current_state)
    {
        case STATE_1:
            // North-South: GREEN, East-West: RED
            set_lights(0, 0, 1,   1, 0, 0);

            if(elapsed_time >= GREEN_TIME) {
                current_state = STATE_2;
                state_timer = current_time;
            }
            break;

        case STATE_2:
            // North-South: YELLOW, East-West: RED
            set_lights(0, 1, 0,   1, 0, 0);

            if(elapsed_time >= YELLOW_TIME) {
                current_state = STATE_3;
                state_timer = current_time;
            }
            break;

        case STATE_3:
            // North-South: RED, East-West: GREEN
            set_lights(1, 0, 0,   0, 0, 1);

            if(elapsed_time >= GREEN_TIME) {
                current_state = STATE_4;
                state_timer = current_time;
            }
            break;

        case STATE_4:
            // North-South: RED, East-West: YELLOW
            set_lights(1, 0, 0,   0, 1, 0);

            if(elapsed_time >= YELLOW_TIME) {
                current_state = STATE_1;  // Go directly to North-South GREEN
                state_timer = current_time;
            }
            break;

        default:
            current_state = STATE_1;
            state_timer = current_time;
            break;
    }
}

/**
 * @brief Main function
 */
int main(void)
{
    /* Initialize HAL Library */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize GPIO */
    MX_GPIO_Init();

    /* Initialize traffic light system */
    set_lights(0, 0, 0, 0, 0, 0);  // Turn off all lights
    state_timer = HAL_GetTick();
    current_state = STATE_1;

    /* Main loop */
    while (1)
    {
        traffic_control();
        HAL_Delay(50);  // Small delay for stability
    }
}

/**
 * @brief System Clock Configuration
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief GPIO Initialization Function
 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO Port A Clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure GPIO pins as outputs */
    HAL_GPIO_WritePin(GPIOA, RED1_Pin|YELLOW1_Pin|GREEN1_Pin|RED2_Pin
                          |YELLOW2_Pin|GREEN2_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = RED1_Pin|YELLOW1_Pin|GREEN1_Pin|RED2_Pin
                          |YELLOW2_Pin|GREEN2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief Error Handler
 */
void Error_Handler(void)
{
    __disable_irq();
    while (1) {
        // Error occurred - stay here
    }
}
