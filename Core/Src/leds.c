/*
 * leds.c
 *
 *  Created on: Dec 26, 2020
 *      Author: Andrei
 */

#include "leds.h"

Led onboardLed;

void initLed()
{
	onboardLed.timer = &LED_TIMER;
	onboardLed.channel = LED_CHANNEL;
	onboardLed.pwm = 0;
	__HAL_TIM_SET_COMPARE(onboardLed.timer, onboardLed.channel, 0);
	HAL_TIM_PWM_Start(onboardLed.timer, onboardLed.channel);
}

void setLed(uint8_t brightness)
{
	if(brightness > LED_MAX_PWM)
		return;
	__HAL_TIM_SET_COMPARE(onboardLed.timer, onboardLed.channel, brightness);
}

void ledOn()
{
	__HAL_TIM_SET_COMPARE(onboardLed.timer, onboardLed.channel, LED_MAX_PWM);
}

void ledOff()
{
	__HAL_TIM_SET_COMPARE(onboardLed.timer, onboardLed.channel, 0);
}
