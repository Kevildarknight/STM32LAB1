#include "main.h"
#include "port.h"

typedef enum {STATE1, STATE2, STATE3, STATE4} Light_State;
static Light_State current_state = STATE1;

#define STATE_DURATION(state) ((state == STATE1) ? 10 : \
		(state == STATE2) ? 2 : \
				(state == STATE3) ? 15 : \
						3)
static unsigned int time_in_state = 0;

void TRAFFIC_LIGHTS_Update(void){
	const Light_State next_state = ((current_state + 1) % 4);
	switch(current_state){
	case STATE1:
		LED_PORT->ODR = (RED1 | GREEN2) &~(YELLOW1 | GREEN1 | RED2 | YELLOW2);
		break;
	case STATE2:
		LED_PORT->ODR = (RED1 | YELLOW2) &~(YELLOW1 | GREEN1 | RED2 | GREEN2);
		break;
	case STATE3:
		LED_PORT->ODR = (GREEN1 | RED2) &~(YELLOW1 | RED1 | YELLOW2 | GREEN2);
		break;
	case STATE4:
		LED_PORT->ODR = (YELLOW1 | RED2) &~(RED1 | GREEN1 | GREEN2 | YELLOW2);
		break;
	default:
		break;
	}
	if(++time_in_state == STATE_DURATION(current_state)){
		current_state = next_state;
		time_in_state = 0;
	}
}
