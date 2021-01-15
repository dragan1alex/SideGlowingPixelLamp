/*
 * leds.h
 *
 *  Created on: Dec 26, 2020
 *      Author: Andrei
 */

#ifndef INC_LEDS_H_
#define INC_LEDS_H_

#include "globalVariables.h"

/* A single white LED is mounted on the board for feedback on the "auto start/stop" mode */
#define LED_TIMER 		htim1
#define LED_CHANNEL 	TIM_CHANNEL_1
#define LED_MAX_PWM 	100

typedef struct
{
	TIM_HandleTypeDef* timer;
	uint16_t channel;
	uint8_t pwm;
}Led;

Led onboardLed;

void initLed();
void setLed(uint8_t brightness);
void ledOn();
void ledOff();

#endif /* INC_LEDS_H_ */
