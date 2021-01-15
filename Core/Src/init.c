/*
 * init.c
 *
 *  Created on: Dec 26, 2020
 *      Author: Andrei
 */

#define HAS_GLOBAL_VARIABLES

#include "init.h"
#include "pixels.h"
#include "settings.h"
#include "animations.h"
#include "buttons.h"
#include "leds.h"
#include "adc.h"


void initDevice()
{
	loadSettings();
	initButtons();
	initLed();
	initPixels();
	initADC();
	startAnimation();
}
