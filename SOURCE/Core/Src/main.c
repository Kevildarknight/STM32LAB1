/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : 4-Way Traffic Light Controller with Dual 7-Segment Display
  *                   East-West: PB0-PB6 (SEG_A to SEG_G)
  *                   North-South: PB7-PB13 (SEG_A1 to SEG_G1)
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* Traffic light timing constants */
#define RED_TIME    5    // 5 seconds
#define YELLOW_TIME 2    // 2 seconds
#define GREEN_TIME  3    // 3 seconds

/* Traffic light states */
typedef enum {
    EW_GREEN_NS_RED,     // East-West Green, North-South Red
    EW_YELLOW_NS_RED,    // East-West Yellow, North-South Red
    EW_RED_NS_GREEN,     // East-West Red, North-South Green
    EW_RED_NS_YELLOW     // East-West Red, North-South Yellow
} TrafficState_t;

/* Global variables */
TrafficState_t currentState = EW_GREEN_NS_RED;
uint32_t stateTimer = 0;
uint32_t lastTick = 0;
uint8_t ewCountdown = GREEN_TIME;
uint8_t nsCountdown = RED_TIME;

/* Function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void display7SEG_EW(int num);
void display7SEG_NS(int num);
void updateTrafficLights(void);
void updateCountdowns(void);

/**
 * @brief Display digit on East-West 7-segment display (PB0-PB6)
 * @param num: Number to display (0-9)
 * @note: Common Anode - Logic 0 turns ON segment, Logic 1 turns OFF segment
 */
void display7SEG_EW(int num)
{
    // Segment patterns for digits 0-9 (Common Anode - inverted)
    // Format: {a, b, c, d, e, f, g}
    const uint8_t patterns[10][7] = {
        {0, 0, 0, 0, 0, 0, 1}, // 0: abcdef
        {1, 0, 0, 1, 1, 1, 1}, // 1: bc
        {0, 0, 1, 0, 0, 1, 0}, // 2: abged
        {0, 0, 0, 0, 1, 1, 0}, // 3: abgcd
        {1, 0, 0, 1, 1, 0, 0}, // 4: fgbc
        {0, 1, 0, 0, 1, 0, 0}, // 5: afgcd
        {0, 1, 0, 0, 0, 0, 0}, // 6: afgcde
        {0, 0, 0, 1, 1, 1, 1}, // 7: abc
        {0, 0, 0, 0, 0, 0, 0}, // 8: abcdefg
        {0, 0, 0, 0, 1, 0, 0}  // 9: abcdfg
    };

    if (num >= 0 && num <= 9) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, patterns[num][0]); // a
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, patterns[num][1]); // b
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, patterns[num][2]); // c
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, patterns[num][3]); // d
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, patterns[num][4]); // e
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, patterns[num][5]); // f
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, patterns[num][6]); // g
    } else {
        // Turn off all segments for invalid input
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
                         GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_SET);
    }
}

/**
 * @brief Display digit on North-South 7-segment display (PB7-PB13)
 * @param num: Number to display (0-9)
 * @note: Common Anode - Logic 0 turns ON segment, Logic 1 turns OFF segment
 */
void display7SEG_NS(int num)
{
    // Segment patterns for digits 0-9 (Common Anode - inverted)
    const uint8_t patterns[10][7] = {
        {0, 0, 0, 0, 0, 0, 1}, // 0: abcdef
        {1, 0, 0, 1, 1, 1, 1}, // 1: bc
        {0, 0, 1, 0, 0, 1, 0}, // 2: abged
        {0, 0, 0, 0, 1, 1, 0}, // 3: abgcd
        {1, 0, 0, 1, 1, 0, 0}, // 4: fgbc
        {0, 1, 0, 0, 1, 0, 0}, // 5: afgcd
        {0, 1, 0, 0, 0, 0, 0}, // 6: afgcde
        {0, 0, 0, 1, 1, 1, 1}, // 7: abc
        {0, 0, 0, 0, 0, 0, 0}, // 8: abcdefg
        {0, 0, 0, 0, 1, 0, 0}  // 9: abcdfg
    };

    if (num >= 0 && num <= 9) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, patterns[num][0]);  // a1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, patterns[num][1]);  // b1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, patterns[num][2]);  // c1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, patterns[num][3]); // d1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, patterns[num][4]); // e1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, patterns[num][5]); // f1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, patterns[num][6]); // g1
    } else {
        // Turn off all segments for invalid input
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|
                         GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_SET);
    }
}

/**
 * @brief Update traffic light LEDs based on current state
 */
void updateTrafficLights(void)
{
    // Turn off all lights first
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
                     GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_SET);

    switch (currentState) {
        case EW_GREEN_NS_RED:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET); // EW Green
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // NS Red
            break;

        case EW_YELLOW_NS_RED:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET); // EW Yellow
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // NS Red
            break;

        case EW_RED_NS_GREEN:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); // EW Red
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // NS Green
            break;

        case EW_RED_NS_YELLOW:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); // EW Red
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // NS Yellow
            break;
    }
}

/**
 * @brief Update countdown values and 7-segment displays
 */
void updateCountdowns(void)
{
    uint32_t currentTick = HAL_GetTick();

    // Update every second
    if (currentTick - lastTick >= 1000) {
        lastTick = currentTick;
        stateTimer++;

        // Update countdown values based on current state
        switch (currentState) {
            case EW_GREEN_NS_RED:
                ewCountdown = (GREEN_TIME > stateTimer) ? (GREEN_TIME - stateTimer) : 0;
                nsCountdown = (RED_TIME > stateTimer) ? (RED_TIME - stateTimer) : 0;

                if (stateTimer >= GREEN_TIME) {
                    currentState = EW_YELLOW_NS_RED;
                    stateTimer = 0;
                    ewCountdown = YELLOW_TIME;
                    nsCountdown = YELLOW_TIME;
                }
                break;

            case EW_YELLOW_NS_RED:
                ewCountdown = (YELLOW_TIME > stateTimer) ? (YELLOW_TIME - stateTimer) : 0;
                nsCountdown = (YELLOW_TIME > stateTimer) ? (YELLOW_TIME - stateTimer) : 0;

                if (stateTimer >= YELLOW_TIME) {
                    currentState = EW_RED_NS_GREEN;
                    stateTimer = 0;
                    ewCountdown = RED_TIME;
                    nsCountdown = GREEN_TIME;
                }
                break;

            case EW_RED_NS_GREEN:
                ewCountdown = (RED_TIME > stateTimer) ? (RED_TIME - stateTimer) : 0;
                nsCountdown = (GREEN_TIME > stateTimer) ? (GREEN_TIME - stateTimer) : 0;

                if (stateTimer >= GREEN_TIME) {
                    currentState = EW_RED_NS_YELLOW;
                    stateTimer = 0;
                    ewCountdown = YELLOW_TIME;
                    nsCountdown = YELLOW_TIME;
                }
                break;

            case EW_RED_NS_YELLOW:
                ewCountdown = (YELLOW_TIME > stateTimer) ? (YELLOW_TIME - stateTimer) : 0;
                nsCountdown = (YELLOW_TIME > stateTimer) ? (YELLOW_TIME - stateTimer) : 0;

                if (stateTimer >= YELLOW_TIME) {
                    currentState = EW_GREEN_NS_RED;
                    stateTimer = 0;
                    ewCountdown = GREEN_TIME;
                    nsCountdown = RED_TIME;
                }
                break;
        }

        // Update 7-segment displays
        display7SEG_EW(ewCountdown);
        display7SEG_NS(nsCountdown);

        // Update traffic lights
        updateTrafficLights();
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

    /* Initialize displays and lights */
    display7SEG_EW(ewCountdown);
    display7SEG_NS(nsCountdown);
    updateTrafficLights();

    lastTick = HAL_GetTick();

    /* Main loop */
    while (1)
    {
        updateCountdowns();
        HAL_Delay(10); // Small delay to prevent excessive CPU usage
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

    /* Enable GPIO Port A and B Clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure GPIO pins for Traffic Lights (PA1-PA6) */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
                     GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
                         GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure GPIO pins for East-West 7-segment (PB0-PB6) */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
                     GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
                         GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Configure GPIO pins for North-South 7-segment (PB7-PB13) */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|
                     GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|
                         GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
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
