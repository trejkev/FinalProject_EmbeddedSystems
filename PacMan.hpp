/*
 * PacMan.hpp
 *
 *  Created on: Jul 18, 2018
 *      Author: Kevin
 */

#ifndef PACMAN_HPP_
#define PACMAN_HPP_
#include <stdio.h>
#include <LcdDriver/Crystalfontz128x128_ST7735.hpp>
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.hpp"
#include "Functions.h"

class PacMan{
    public:
        PacMan(uint8_t initXposition, uint8_t initYposition, Graphics_Context *g_sBall1);
        uint8_t l_pmPacActualXYposition[2];                   // X in position 0 and Y in position 1
        void PrintPM(void);                                   // Print PacMan each time is needed
        void PrintGhosts(uint8_t l_u8MovAllowGhost);          // Print Ghosts each time is needed
        void GetPacPosition(void);                            // Reads the accelerometer to get the new (X,Y) for PacMan
        void CheckPacCollision(void);                         // If there is any collision the ball will no advance in that direction
        uint8_t BallReachingTarget(void);                             // Returns 1 if the game is done
        void GameEnded(void);
    private:

};




#endif /* PACMAN_HPP_ */
