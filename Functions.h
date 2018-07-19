/*
 * Functions.h
 *
 *  Created on: Jul 9, 2018
 *      Author: Kevin
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <LcdDriver/Crystalfontz128x128_ST7735.hpp>
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.hpp"
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>

/*General functions*/
void SetupScreen(void);
void GetAccelData(void);
void CreateMaze();

/*Specific functions for PacMan*/
void DrawMapPacMan(void);


/*Global variables*/
uint8_t g_ui8ActualX;
uint8_t g_ui8ActualY;

/* ADC results buffer */
int16_t g_u16resultsBuffer[2];

/* Graphic library context */
Graphics_Context g_sGeneral;              // Graphic context for general purpose
Graphics_Context g_sBall;                 // Graphic context for balls 1 to 3
Graphics_Context g_sEraser;               // Filled ball to delete previous ball
Graphics_Context g_sBallTarget;           // Target of the ball




#endif /* FUNCTIONS_H_ */
