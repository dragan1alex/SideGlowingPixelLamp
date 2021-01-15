/*
 * buttons.c
 *
 *  Created on: Dec 30, 2020
 *      Author: Andrei
 */

#include "buttons.h"

Button button[NUMBER_OF_BUTTONS];


void initButtons()
{
	uint8_t i;
	button[BUTTON_MODE].port = 	MODE_BUTTON_PORT;
	button[BUTTON_MODE].pin = 	MODE_BUTTON_PIN;

	button[BUTTON_POWER].port = POWER_BUTTON_PORT;
	button[BUTTON_POWER].pin = 	POWER_BUTTON_PIN;

	button[BUTTON_AUTO].port = 	AUTO_BUTTON_PORT;
	button[BUTTON_AUTO].pin = 	AUTO_BUTTON_PIN;

	for(i=0;i<NUMBER_OF_BUTTONS;i++)
	{
		button[i].currentState 	= 	BUTTON_RELEASED;
		button[i].lastState 	= 	BUTTON_RELEASED;
	}

}

/*
 * Button press/release handler
 */
void handleButton(ButtonID id)
{
	if(id == BUTTON_MODE && button[id].currentState == BUTTON_PRESSED)
	{
		vars.nextAnimation = TRUE;
	}
	if(id == BUTTON_POWER)
	{
		vars.setBrightnessMode = button[id].currentState == BUTTON_PRESSED;
		if(button[id].currentState == BUTTON_RELEASED) //brightness setting done, save it to flash
			saveSettings();
	}
	if(id == BUTTON_AUTO && button[id].currentState == BUTTON_PRESSED)
	{
		settings.modeAuto = 1 - settings.modeAuto;
		saveSettings();
	}
	button[id].lastState = button[id].currentState;
	if(button[id].currentState == BUTTON_RELEASED)
	{
		button[id].debounceTime = osKernelGetTickCount();
	}
}


ButtonID getButtonID(uint16_t pin)
{
	uint8_t i;
	for(i=0;i<NUMBER_OF_BUTTONS;i++)
		if(button[i].pin == pin)
			return i;
	return 0;
}

State buttonBounce(ButtonID id)
{
	if(button[id].currentState == BUTTON_PRESSED && button[id].lastState == BUTTON_RELEASED)
	{
		uint32_t currentTime = osKernelGetTickCount();
		if(button[id].debounceTime + DEBOUNCE_TIME_MS > currentTime)
			return TRUE;
	}
	return FALSE;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	ButtonID id = getButtonID(GPIO_Pin);
	button[id].currentState = HAL_GPIO_ReadPin(button[id].port, button[id].pin);
	if(buttonBounce(id) == FALSE)
		handleButton(id);
}
