/*
 * globalVariables.h
 *
 *  Created on: Dec 26, 2020
 *      Author: Andrei
 */

#ifndef INC_GLOBALVARIABLES_H_
#define INC_GLOBALVARIABLES_H_

#include "cmsis_os2.h"
#include "main.h"

typedef enum
{
	FALSE,
	TRUE
}State;

typedef struct
{
	uint8_t nextAnimation;		//set to true to trigger changing to the next animation
	uint8_t isDark;				//if settings.modeAuto is true, this tells the monitor task if the lights should be on or off
	uint8_t setBrightnessMode;	//while true, the brightness adjustment mode will be active
}RuntimeVars;

RuntimeVars vars;

ADC_HandleTypeDef hadc1;
SPI_HandleTypeDef hspi2;
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim3_ch1_trig;

#endif /* INC_GLOBALVARIABLES_H_ */
