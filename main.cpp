/*
 * main.cpp
 *
 *  Created on: Jul 6, 2018
 *      Author: Kevin
 */

#include <LcdDriver/Crystalfontz128x128_ST7735.hpp>
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.hpp"
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>
#include "Ball.hpp"
#include "Functions.h"
#include "math.h"


void SetupAccelerometer(void);

/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;               // Stop watchdog timer
    SetupScreen();
    SetupAccelerometer();
    Graphics_clearDisplay(&g_sBall);
    CreateMaze();
    Ball ball1(120, 40, &g_sBall);                            // Create an object ball1 from the class Ball
    uint8_t l_iFinish = 0;                                    // Used to stay on play loop or get out if finished
    while(l_iFinish == 0){
        uint16_t l_ui16Delayer = 0;
        while (l_ui16Delayer <= 10000){l_ui16Delayer++;}
        ball1.PrintBall();
        l_iFinish = ball1.BallReachingTarget();
    }
    ball1.GameEnded();
}


void SetupAccelerometer(){


    /* Configures Pin 4.0, 4.2, and 6.1 as ADC input */
         MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2, GPIO_TERTIARY_MODULE_FUNCTION);
         MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN1, GPIO_TERTIARY_MODULE_FUNCTION);

         /* Initializing ADC (ADCOSC/64/8) */
         MAP_ADC14_enableModule();
         ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);
         ADC14_setResultFormat(ADC_SIGNED_BINARY);
         ADC14_setResolution(ADC_14BIT);

         /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM2 (A11, A13, A14)  with no repeat)
              * with internal 2.5v reference */
         MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM2, true);
         MAP_ADC14_configureConversionMemory(ADC_MEM0,
                 ADC_VREFPOS_AVCC_VREFNEG_VSS,
                 ADC_INPUT_A14, ADC_NONDIFFERENTIAL_INPUTS);

         MAP_ADC14_configureConversionMemory(ADC_MEM1,
                 ADC_VREFPOS_AVCC_VREFNEG_VSS,
                 ADC_INPUT_A13, ADC_NONDIFFERENTIAL_INPUTS);

         MAP_ADC14_configureConversionMemory(ADC_MEM2,
                 ADC_VREFPOS_AVCC_VREFNEG_VSS,
                 ADC_INPUT_A11, ADC_NONDIFFERENTIAL_INPUTS);


         /* Setting up the sample timer to automatically step through the sequence
          * convert.
          */
         MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

         /* Triggering the start of the sample */
         MAP_ADC14_enableConversion();
         MAP_ADC14_toggleConversionTrigger();
         NVIC_SetPriority(ADC14_IRQn,1);
         NVIC_EnableIRQ(ADC14_IRQn);
         __enable_irq();

}

extern "C"{
void ADC14_IRQHandler(void){
    __disable_irq();

    printf("Entre a la interrupción del ADC\n");

    ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0;
    __enable_irq();
    return;
    }
}




