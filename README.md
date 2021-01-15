# SideGlowingPixelLamp

DISCLAIMER: This is a hobby project, it will be completed as the free time allows it :)

You are free to use any component of this software as you please, as long as the code you chose to use or modify is written by me. 
STM has a license agreement you need to read before using/modifying/redistributing their bits and pieces of software.


Decorative ambient light powered by STM32F103 Arm Cortex-M3 MCU, running CMSIS-RTOS V2.

A wonderful learning experience if you want to know how the DMA works in an ARM MCU, 
and how it can be used together with a timer to drive individually addressable RGB LEDs (TX1812) with minimal performance hit on the MCU.

This board uses 34 pixels, but the code can easily be modified to support as many LEDs as the memory allows you.
The code has the pixels mapped to a structure array to allow each pixel to be individually faded (both color and brightness) by an always-running task.

Each animation is done by a separate task for easier management of animations and modes of operation. 
This way each animation can be easily started/stopped/changed by a monitor task, and the brightness control of the pixels can be (and was) set up as an extra annimation.

Written code can be found in Core/Src and Core/Inc.

Project created in STM32CubeIDE 1.5.0.
