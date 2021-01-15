/*
 * settings.c
 *
 *  Created on: Dec 26, 2020
 *      Author: Andrei
 */

#include "settings.h"

Settings settings;

void initSettings()
{
	settings.maxBrightness = 100;
	settings.animationNumber = 0;
	settings.fadeOutTime = 1000;
	settings.storedSize = sizeof(settings);
	settings.modeAuto = FALSE;
	saveSettings();
}

void loadSettings()
{
	readBytesFlash(SETTINGS_ADDRESS, (void*)&settings, sizeof(settings));
	if(settings.storedSize != sizeof(settings))
		initSettings();
}

void saveSettings()
{
	sectorErase4KFlash(SETTINGS_ADDRESS);
	writeBytesFlash(SETTINGS_ADDRESS, (void*)&settings, sizeof(settings));
}
