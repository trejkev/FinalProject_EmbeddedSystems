/*
 * Functions.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: Kevin
 */

#include "Functions.h"

void SetupScreen(){

    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */

    Graphics_initContext(&g_sGeneral, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sGeneral, GRAPHICS_COLOR_BLUE);
    Graphics_setBackgroundColor(&g_sGeneral, GRAPHICS_COLOR_BLACK);
    GrContextFontSet(&g_sGeneral, &g_sFontFixed6x8);

    Graphics_initContext(&g_sBall, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sBall, GRAPHICS_COLOR_YELLOW);
    Graphics_setBackgroundColor(&g_sBall, GRAPHICS_COLOR_BLACK);

    Graphics_initContext(&g_sEraser, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sEraser, GRAPHICS_COLOR_BLACK);
    Graphics_setBackgroundColor(&g_sEraser, GRAPHICS_COLOR_BLACK);

    Graphics_initContext(&g_sBallTarget, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sBallTarget, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(&g_sBallTarget, GRAPHICS_COLOR_BLACK);

}


void GetAccelData(void){
    /* These values were obtained experimentally
     * both warantee that 0 deg is almost equal to 0
     * measurement on accelerometer*/
    g_u16resultsBuffer[0] = (int16_t) ADC14_getResult(ADC_MEM0)+6320;         // Read the 2 channels of the ADC
    g_u16resultsBuffer[1] = (int16_t) ADC14_getResult(ADC_MEM1)+6300;
}

void CreateMaze(){
    uint8_t l_u8Coordinates[9] = {0, 15, 30, 45, 60, 75, 90, 105, 127};

    /*Defines the area where the ball will be*/
    Graphics_Rectangle MazeArea= {0, 127, 0, 127};
    Graphics_drawRectangle(&g_sGeneral, &MazeArea);

    /*Defines the maze walls*/
    uint8_t l_u8ItFillScreen = 15;           // Makes an offset each 15 pixels on the screen to draw the walls
    while(l_u8ItFillScreen <= 105){          // The draw is repeated 3 times along the screen
        /* Walls of the maze:
         * The pattern is repeated two times, first from 0 to 45 and then from 45 to 90
         * last, at 105 we use just the first line of the Y pattern.*/
        Graphics_fillCircle(&g_sGeneral, 7, 117, 6);
        Graphics_drawCircle(&g_sBallTarget, 7, 117, 5);
        Graphics_drawCircle(&g_sBallTarget, 7, 117, 3);
        Graphics_fillCircle(&g_sBallTarget, 7, 117, 1);

        Graphics_drawLine(&g_sGeneral, l_u8ItFillScreen, l_u8Coordinates[0], l_u8ItFillScreen, l_u8Coordinates[1]);
        Graphics_drawLine(&g_sGeneral, l_u8ItFillScreen, l_u8Coordinates[2], l_u8ItFillScreen, l_u8Coordinates[3]);
        Graphics_drawLine(&g_sGeneral, l_u8ItFillScreen, l_u8Coordinates[4], l_u8ItFillScreen, l_u8Coordinates[6]);
        Graphics_drawLine(&g_sGeneral, l_u8ItFillScreen, l_u8Coordinates[7], l_u8ItFillScreen, l_u8Coordinates[8]);

        Graphics_drawLine(&g_sGeneral, l_u8Coordinates[1], l_u8ItFillScreen, l_u8Coordinates[2], l_u8ItFillScreen);
        Graphics_drawLine(&g_sGeneral, l_u8Coordinates[4], l_u8ItFillScreen, l_u8Coordinates[5], l_u8ItFillScreen);
        Graphics_drawLine(&g_sGeneral, l_u8Coordinates[7], l_u8ItFillScreen, l_u8Coordinates[8], l_u8ItFillScreen);

        if(l_u8ItFillScreen < 105){
            Graphics_drawLine(&g_sGeneral, 15+l_u8ItFillScreen, l_u8Coordinates[0], 15+l_u8ItFillScreen, l_u8Coordinates[1]);
            Graphics_drawLine(&g_sGeneral, 15+l_u8ItFillScreen, l_u8Coordinates[2], 15+l_u8ItFillScreen, l_u8Coordinates[4]);
            Graphics_drawLine(&g_sGeneral, 15+l_u8ItFillScreen, l_u8Coordinates[5], 15+l_u8ItFillScreen, l_u8Coordinates[7]);
            Graphics_drawLine(&g_sGeneral, 30+l_u8ItFillScreen, l_u8Coordinates[1], 30+l_u8ItFillScreen, l_u8Coordinates[7]);

            Graphics_drawLine(&g_sGeneral, l_u8Coordinates[4], 15+l_u8ItFillScreen, l_u8Coordinates[5], 15+l_u8ItFillScreen);
            Graphics_drawLine(&g_sGeneral, l_u8Coordinates[0], 30+l_u8ItFillScreen, l_u8Coordinates[1], 30+l_u8ItFillScreen);
            Graphics_drawLine(&g_sGeneral, l_u8Coordinates[5], 30+l_u8ItFillScreen, l_u8Coordinates[6], 30+l_u8ItFillScreen);
        }
        l_u8ItFillScreen = l_u8ItFillScreen + 45;
    }
}



