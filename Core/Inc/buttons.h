/*
 * buttons.h
 *
 *  Created on: Dec 28, 2020
 *      Author: Andrei
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include "main.h"
#include "settings.h"
#include "animations.h"
#include "cmsis_os2.h"

#define NUMBER_OF_BUTTONS 	3
#define DEBOUNCE_TIME_MS	300

#define MODE_BUTTON_PORT 	GPIOA
#define MODE_BUTTON_PIN		GPIO_PIN_12
#define AUTO_BUTTON_PORT	GPIOA
#define AUTO_BUTTON_PIN		GPIO_PIN_11
#define POWER_BUTTON_PORT	GPIOA
#define POWER_BUTTON_PIN	GPIO_PIN_10

typedef enum
{
	BUTTON_PRESSED,
	BUTTON_RELEASED
}ButtonState;

typedef enum
{
	BUTTON_MODE,
	BUTTON_AUTO,
	BUTTON_POWER
}ButtonID;

typedef struct
{
	ButtonState currentState;
	ButtonState lastState;
	GPIO_TypeDef* port;
	uint16_t pin;
	uint32_t debounceTime;
}Button;

Button button[NUMBER_OF_BUTTONS];

void initButtons();
void handleButton(ButtonID id);
State buttonBounce(ButtonID id);

ButtonID getButtonID(uint16_t pin);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);


#endif /* INC_BUTTONS_H_ */
