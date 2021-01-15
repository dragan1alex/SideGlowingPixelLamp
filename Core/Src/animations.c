/*
 * animations.c
 *
 *  Created on: Dec 27, 2020
 *      Author: Andrei
 */

#include "animations.h"

const osThreadAttr_t circle_attributes = {
  .name = "sideToSide",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 64 * 4
};

const osThreadAttr_t circleRandColor_attributes = {
  .name = "sideToSideRandColor",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 64 * 4
};

const osThreadAttr_t randomChaseSingleColor_attributes = {
  .name = "randomChaseSingleColor",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 64 * 4
};

const osThreadAttr_t rainbow_attributes = {
  .name = "rainbow",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 64 * 4
};


const osThreadAttr_t changeBrightness_attributes = {
  .name = "changeBrightness",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 64 * 4
};

AnimationSettings animationSettings;

osThreadId_t animationThreads[NUMBER_OF_ANIMATIONS + 1];

void nextAnimation()
{
	settings.animationNumber++;
	settings.animationNumber %= NUMBER_OF_ANIMATIONS;
	saveSettings();
	vars.nextAnimation = FALSE;
	startAnimation();
}

const osThreadAttr_t corners_attributes = {
  .name = "corners",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 64 * 4
};

/*
 * Always-on task, leave it be
 * Pixel fade-out can be controlled using the functions enablePixelsFade()/disablePixelsFade()
 */
void FadeOutLedsTask(void* args)
{
	uint16_t i;
	uint32_t currentTime;
	while(1)
	{
		currentTime = osKernelGetTickCount();
		for(i=0;i<NUMBER_OF_PIXELS;i++)
		{
			decreasePixelBrightness(i, currentTime);
			fadePixelColor(i, currentTime);
		}
		if(vars.nextAnimation == TRUE)
		{
			nextAnimation();
		}
		if(vars.setBrightnessMode == TRUE && animationThreads[NUMBER_OF_ANIMATIONS] == NULL)
		{
			stopAnimation();
			startBrightnessChangeMode();
		}
		if(vars.setBrightnessMode == FALSE && animationThreads[NUMBER_OF_ANIMATIONS] != NULL)
		{
			startAnimation();
		}
		osDelay(1);
	}
}

void MonitorTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	int8_t i;
	State enabledInAuto;
	/* Infinite loop */
	for(;;)
	{
		if(settings.modeAuto == TRUE)
		{
			checkLightSensor();
			if(vars.isDark == FALSE)
			{
				// If an animation other than brightness change is running
				if(isAnimationRunning() != FALSE && isAnimationRunning() != NUMBER_OF_ANIMATIONS + 1)
				{
					/* Stop the animation task */
					stopAnimation();
					/* Fade out the pixels */
					enablePixelsFade();
					enabledInAuto = FALSE;
				}
				/* Fade the white status led */
				for(i=0;i<=LED_MAX_PWM;i++)
				{
					setLed(i);
					osDelay(10);
					checkLightSensor();
				}
				osDelay(1000);
				for(i=LED_MAX_PWM; i>=0 ;i--)
				{
					setLed(i);
					osDelay(10);
					checkLightSensor();
				}
			}
			/* Start the animation if not started */
			else
			{
				setLed(50);
				if(isAnimationRunning() == FALSE)
				{
					startAnimation();
				}
				enabledInAuto = TRUE;
			}
		}
		else
		{
			if(enabledInAuto != TRUE)
			{
				startAnimation();
				enabledInAuto = TRUE;
			}
			setLed(0);
		}
		osDelay(1000);
	}
  /* USER CODE END 5 */
}

void startBrightnessChangeMode()
{
	animationSettings.delayBetweenCycles = 4000;
	animationSettings.delayBetweenPixels = 1500;
	settings.fadeOutTime = 5600;
	animationThreads[NUMBER_OF_ANIMATIONS] = osThreadNew(ChangeBrightness, NULL, &changeBrightness_attributes);
}

void startAnimation()
{
	uint8_t number = settings.animationNumber;
	if(number >= NUMBER_OF_ANIMATIONS)
		return;
	stopAnimation();
	disablePixelsFade();
	switch (number) {
		case 0:
			animationSettings.delayBetweenCycles = 0;
			animationSettings.delayBetweenPixels = 20;
			animationSettings.brightnessIncreaseDuration = 200;
			settings.fadeOutTime = 1000;
			enablePixelsFade();
			animationThreads[number] = osThreadNew(Circle, NULL, &circle_attributes);
			break;
		case 1:
			animationSettings.delayBetweenCycles = 0;
			animationSettings.delayBetweenPixels = 0;
			animationSettings.brightnessIncreaseDuration = 100;
			settings.fadeOutTime = 1800;
			enablePixelsFade();
			animationThreads[number] = osThreadNew(CircleRandColor, NULL, &circleRandColor_attributes);
			break;
		case 2:
			animationSettings.delayBetweenCycles = 1500;
			animationSettings.delayBetweenPixels = 25;
			animationSettings.brightnessIncreaseDuration = 0;
			settings.fadeOutTime = 3500;
			enablePixelsFade();
			animationThreads[number] = osThreadNew(RandomChaseSingleColor, NULL, &randomChaseSingleColor_attributes);
			break;
		case 3:
			animationSettings.delayBetweenCycles = 100;
			animationSettings.delayBetweenPixels = 75;
			animationSettings.brightnessIncreaseDuration = 0;
			settings.fadeOutTime = 1000;
			animationThreads[number] = osThreadNew(Rainbow, NULL, &rainbow_attributes);
			break;
		case 4:
			animationSettings.delayBetweenPixels = 80;
			animationSettings.delayBetweenCycles = 500;
			settings.fadeOutTime = 1000;
			animationSettings.brightnessIncreaseDuration = 3000;
			animationSettings.r = 200;
			animationSettings.g = 200;
			animationSettings.b = 200;
			enablePixelsFade();
			animationThreads[number] = osThreadNew(White, NULL, &corners_attributes);
			break;
		default:
			break;
	}
}

void stopAnimation()
{
	uint16_t i;
	for(i=0;i<=NUMBER_OF_ANIMATIONS;i++)
	{
		if(animationThreads[i] != NULL && osThreadGetState(animationThreads[i]) != osThreadTerminated)
		{
			osThreadTerminate(animationThreads[i]);
			animationThreads[i] = NULL;
			if(i == NUMBER_OF_ANIMATIONS)
			{
				//The last animation is the brightness change mode, if it was running get the brightness and save to flash
				Pixel p = getPixel(0);
				settings.maxBrightness = p.brightness;
				saveSettings();
				uint16_t j;
				disablePixelsFade();
				for(j=0;j<NUMBER_OF_PIXELS;j++)
				{
					setPixelBrightness(j, 0);
				}
			}
			if(i==4)
				enablePixelsFade();
		}
	}
}

uint16_t isAnimationRunning()
{
	uint16_t i;
	for(i=0;i<=NUMBER_OF_ANIMATIONS;i++)
		if(animationThreads[i] != NULL && osThreadGetState(animationThreads[i]) != osThreadTerminated)
			return i+1;
	return 0;
}

void Circle(void* args)
{
	uint8_t r,g,b;
	int32_t i;
	AnimationDirection direction = 1;
	getRandomColor(&r, &g, &b);
	while(1)
	{
		direction = BACKWARD;
		if(direction == FORWARD)
		{
			for(i=0; i <= NUMBER_OF_PIXELS; i++)
			{
				setPixelColor(i, r, g, b);
				increasePixelBrightness(i, animationSettings.brightnessIncreaseDuration);
				osDelay(animationSettings.delayBetweenPixels);
				changeColor(&r, &g, &b, 10);
			}
		}
		else
		{
			for(i=NUMBER_OF_PIXELS; i>=0;i--)
			{
				setPixelColor(i, r, g, b);
				increasePixelBrightness(i, animationSettings.brightnessIncreaseDuration);
				osDelay(animationSettings.delayBetweenPixels);
				changeColor(&r, &g, &b, 10);
			}
		}
		osDelay(animationSettings.delayBetweenCycles);
	}
}

void CircleRandColor(void* args)
{
	int32_t i;
	uint8_t r,g,b;
	AnimationDirection direction = 1;
	while(1)
	{
		direction = BACKWARD;
		if(direction == FORWARD)
		{
			for(i=0; i < NUMBER_OF_PIXELS; i++)
			{
				if(i%2==0)
					getRandomColor(&r, &g, &b);
				increasePixelToColorWithDelay(i, r, g, b, animationSettings.brightnessIncreaseDuration);
				osDelay(animationSettings.delayBetweenPixels);
			}
		}
		else
		{
			for(i=NUMBER_OF_PIXELS; i>=0;i--)
			{
				if(i%2==0)
					getRandomColor(&r, &g, &b);
				increasePixelToColorWithDelay(i, r, g, b, animationSettings.brightnessIncreaseDuration);
				osDelay(animationSettings.delayBetweenPixels);
			}
		}
		osDelay(animationSettings.delayBetweenCycles);
	}
}


void RandomChaseSingleColor(void* args)
{
	int32_t i, pixelDistance;
	AnimationDirection direction = 1;
	uint16_t startPixel;
	uint8_t r, g, b;
	while(1)
	{
		direction = 1 - direction;
		startPixel = rand() % (NUMBER_OF_PIXELS - 10) + 5;
		pixelDistance = rand() % (NUMBER_OF_PIXELS - 15) + 5;
		getRandomColor(&r, &g, &b);
		if(direction == FORWARD)
		{
			for(i=startPixel; i < NUMBER_OF_PIXELS && pixelDistance > 0; i++)
			{
				increasePixelToColorWithDelay(i, r, g, b, animationSettings.brightnessIncreaseDuration);
				osDelay(animationSettings.delayBetweenPixels);
				pixelDistance--;
			}
		}
		else
		{
			for(i=startPixel; i >= 0 && pixelDistance > 0 ;i--)
			{
				increasePixelToColorWithDelay(i, r, g, b, animationSettings.brightnessIncreaseDuration);
				osDelay(animationSettings.delayBetweenPixels);
				pixelDistance--;
			}
		}
		osDelay(animationSettings.delayBetweenCycles);
	}
}

void Rainbow(void* args)
{
	int32_t i;
	uint32_t phase;
	uint8_t r,g,b;
	uint8_t phaseMult = 768 / NUMBER_OF_PIXELS;
	while(1)
	{
		for(i = 0; i <= NUMBER_OF_PIXELS; i++)
		{
			getRainbowColor(phase + i * phaseMult, &r, &g, &b);
			setNextPixelFadeColor(i, r, g, b, animationSettings.delayBetweenCycles);
		}
		phase+=phaseMult;
		osDelay(animationSettings.delayBetweenCycles + 12);
	}
}

void White(void* args)
{
	int32_t i;
	uint8_t r,g,b;
	disablePixelsFade();
	for(i=0;i<NUMBER_OF_PIXELS;i++)
	{
		setPixelColor(i, animationSettings.r, animationSettings.g, animationSettings.b);
		setPixelBrightness(i, settings.maxBrightness);
	}
	while(1)
	{
		i = rand()%NUMBER_OF_PIXELS;
		getRandomColor(&r, &g, &b);
		setPixelColor(i, r, g, b);
		setNextPixelFadeColor(i, animationSettings.r, animationSettings.g, animationSettings.b, animationSettings.brightnessIncreaseDuration);
		osDelay(rand() % animationSettings.delayBetweenCycles + 200);
	}
}


void ChangeBrightness(void* args)
{
	uint16_t i;
	while(1)
	{
		disablePixelsFade();
		for(i=0;i<NUMBER_OF_PIXELS;i++)
			setPixelColorNormalized(i, 255, 255, 255, 100);
		osDelay(animationSettings.delayBetweenPixels);
		enablePixelsFade();
		osDelay(animationSettings.delayBetweenCycles);
		disablePixelsFade();
		for(i=0;i<NUMBER_OF_PIXELS;i++)
			setPixelColor(i, 255, 0, 0);
		osDelay(animationSettings.delayBetweenPixels * 2);

	}
}

/*
 * Helper functions used in the animation tasks
 */


void getRainbowColor(uint32_t phase, uint8_t* r, uint8_t* g, uint8_t* b)
{
	phase %= 768;
	if(phase < 256) {
	    *r = phase;
	    *g = 255 - phase;
	    *b = 0;
	  }
	  else if(phase < 512) {
		  phase -= 256;
		  *r = 255 - phase;
		  *g = 0;
		  *b = phase;
	  }
	  else {
		  phase -= 512;
		  *r = 0;
		  *g = phase;
		  *b = 255 - phase;
	  }
}

void changeColor(uint8_t* r, uint8_t* g, uint8_t* b, uint8_t amount)
{
	static int8_t lr, lg, lb, init;
	uint8_t colorChanged;
	if(init == 0)
	{
		lr = amount;
		//lr = rand()%2 == 0 ? 0 : amount;
		lr = rand()%2 == 0 ? lr : -lr;

		lg = amount;
		//lg = rand()%2 == 0 ? 0 : amount;
		lg = rand()%2 == 0 ? lg : -lg;

		lb = amount;
		//lb = rand()%2 == 0 ? 0 : amount;
		lb = rand()%2 == 0 ? lb : -lb;
		init = 1;
	}
	colorChanged = 0;
	if((lr == -amount && *r > amount) || (lr == amount && *r < 255 - amount))
	{
		*r += lr;
		colorChanged = 1;
	}
	if((lg == -amount && *g > amount) || (lg == amount && *g < 255 - amount))
	{
		*g += lg;
		colorChanged = 1;
	}
	if((lb == -amount && *b > amount) || (lb == amount && *b < 255 - amount))
	{
		*b += lb;
		colorChanged = 1;
	}
	if(!colorChanged)
		init = 0;
}

void getRandomColor(uint8_t* r, uint8_t* g, uint8_t* b)
{
	*r = rand() % 128 + 128;
	*g = rand() % 128;
	*b = rand() % 50;
	shuffleColors(r, g, b);
}

void shuffleColors(uint8_t* r, uint8_t* g, uint8_t* b)
{
	int random;
	uint8_t temp;
	random = rand() % 100;
	while(random > 0)
	{
		temp = *r;
		*r = *g;
		*g = temp;
		random--;
	}
	random = rand() % 100;
	while(random > 0)
	{
		temp = *r;
		*r = *b;
		*b = temp;
		random--;
	}
	random = rand() % 100;
	while(random > 0)
	{
		temp = *b;
		*b = *g;
		*g = temp;
		random--;
	}
}

void increasePixelToColorWithDelay(uint16_t number, uint8_t r, uint8_t g, uint8_t b, uint32_t duration)
{
	uint32_t j;
	uint32_t delay = duration * 2 / settings.maxBrightness;
	for(j=2;j<=settings.maxBrightness;j+=2)
	{
		setPixelColorNormalized(number, r, g, b, j);
		osDelay(delay);
	}
}

void increasePixelBrightness(uint16_t number, uint32_t duration)
{
	uint32_t j;
	uint32_t delay = duration * 2 / settings.maxBrightness;
	for(j=2;j<=settings.maxBrightness;j+=2)
	{
		setPixelBrightness(number, j);
		osDelay(delay);
	}
}
