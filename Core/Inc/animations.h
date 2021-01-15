/*
 * animations.h
 *
 *  Created on: Dec 26, 2020
 *      Author: Andrei
 */

#ifndef INC_ANIMATIONS_H_
#define INC_ANIMATIONS_H_

#include "pixels.h"
#include "globalVariables.h"
#include "settings.h"
#include "leds.h"
#include "adc.h"

#define NUMBER_OF_ANIMATIONS	5
#define DEFAULT_ANIMATION		0

typedef enum
{
	FORWARD,
	BACKWARD
}AnimationDirection;

typedef struct
{
	uint32_t delayBetweenPixels;
	uint32_t delayBetweenCycles;
	uint32_t brightnessIncreaseDuration;
	uint8_t r,g,b;
}AnimationSettings;

AnimationSettings animationSettings;

/*The last animation is the brightness change mode, which is treated as a special one*/
osThreadId_t animationThreads[NUMBER_OF_ANIMATIONS + 1];

void nextAnimation();
/* Always running task that tries to fade the pixel brightness or color (or both) if the fade is
 * individually enabled on the pixels */
void FadeOutLedsTask(void* args);
void MonitorTask(void *argument);
void startBrightnessChangeMode();
void startAnimation();
void stopAnimation();
uint16_t isAnimationRunning();

void Circle(void* args);
void CircleRandColor(void* args);
void RandomChaseSingleColor(void* args);
void Rainbow(void* args);
void White(void* args);

void ChangeBrightness(void* args);

void getRainbowColor(uint32_t phase, uint8_t* r, uint8_t* g, uint8_t* b);

void changeColor(uint8_t* r, uint8_t* g, uint8_t* b, uint8_t amount);
void getRandomColor(uint8_t* r, uint8_t* g, uint8_t* b);
void shuffleColors(uint8_t* r, uint8_t* g, uint8_t* b);
void increasePixelToColorWithDelay(uint16_t number, uint8_t r, uint8_t g, uint8_t b, uint32_t delay);
void increasePixelBrightness(uint16_t number, uint32_t duration);

#endif /* INC_ANIMATIONS_H_ */
