/*
 * settings.h
 *
 *  Created on: Dec 26, 2020
 *      Author: Andrei
 */

#ifndef INC_SETTINGS_H_
#define INC_SETTINGS_H_

#define SETTINGS_ADDRESS 0x0000

#include "globalVariables.h"
#include "spi_flash.h"

typedef struct
{
	uint32_t storedSize; 	/* Stored structure size in flash, used for re-initialization of the settings in case of an update */
	uint32_t fadeOutTime;
	uint8_t maxBrightness;
	uint8_t animationNumber;
	uint8_t modeAuto;
}Settings;

Settings settings;

void initSettings();
void loadSettings();
void saveSettings();


#endif /* INC_SETTINGS_H_ */
