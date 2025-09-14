#include "main.h"
#include "port.h"

typedef enum {STATE1, STATE2, STATE3, STATE4} Light_State;
static Light_State current_state = STATE1;

#define STATE_DURATION(state) ((state == STATE1) ? 10 : \
		(state == STATE2) ? 2 : \
				(state == STATE3) ? 15 : \
						3)
static unsigned int time_in_state = 0;

// Helper function to turn off all LEDs
static void turn_off_all_leds(void) {
    HAL_GPIO_WritePin(LED_PORT, RED1|YELLOW1|GREEN1|RED2|YELLOW2|GREEN2, GPIO_PIN_RESET);
}

// Helper function to turn on specific LEDs
static void turn_on_leds(uint16_t pins) {
    HAL_GPIO_WritePin(LED_PORT, pins, GPIO_PIN_SET);
}

void TRAFFIC_LIGHTS_Update(void){
	const Light_State next_state = ((current_state + 1) % 4);

	// Turn off all LEDs first
	turn_off_all_leds();

	// Turn on appropriate LEDs for current state
	switch(current_state){
	case STATE1:  // RED1 and GREEN2
		turn_on_leds(RED1 | GREEN2);
		break;
	case STATE2:  // RED1 and YELLOW2
		turn_on_leds(RED1 | YELLOW2);
		break;
	case STATE3:  // GREEN1 and RED2
		turn_on_leds(GREEN1 | RED2);
		break;
	case STATE4:  // YELLOW1 and RED2
		turn_on_leds(YELLOW1 | RED2);
		break;
	default:
		break;
	}

	if(++time_in_state >= STATE_DURATION(current_state)){
		current_state = next_state;
		time_in_state = 0;
	}
}
