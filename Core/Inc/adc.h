/*
 * adc.h
 *
 *  Created on: Jan 3, 2021
 *      Author: Andrei
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "globalVariables.h"

#define ADC_HAL 				hadc1
#define NUM_ADC_CHANNELS		4

#define LIGHT_SENSOR_THRESHOLD	80
#define LIGHT_SENSOR_OFFSET 	20

typedef struct
{
	uint16_t adc[NUM_ADC_CHANNELS];
	float temperature;
}Analog;

Analog analog;

void initADC();
uint16_t getLightSensor();
void checkLightSensor();
float getVCC();
float getTemperature();


#endif /* INC_ADC_H_ */
