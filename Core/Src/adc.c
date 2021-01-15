/*
 * adc.c
 *
 *  Created on: Jan 3, 2021
 *      Author: Andrei
 */

#include "adc.h"

Analog analog;

void initADC()
{
	HAL_ADCEx_Calibration_Start(&ADC_HAL);
	HAL_ADC_Start_DMA(&ADC_HAL, (uint32_t*)analog.adc, NUM_ADC_CHANNELS);
}
uint16_t getLightSensor()
{
	return analog.adc[1];
}

void checkLightSensor()
{
	uint16_t sensor = getLightSensor();
	if(sensor < LIGHT_SENSOR_THRESHOLD - LIGHT_SENSOR_OFFSET)
		vars.isDark = TRUE;
	if(sensor > LIGHT_SENSOR_THRESHOLD + LIGHT_SENSOR_OFFSET)
		vars.isDark = FALSE;
}

float getVCC();
float getTemperature();
