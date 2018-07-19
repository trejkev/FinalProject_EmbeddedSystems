/*
 * Ball.hpp
 *
 *  Created on: Jul 9, 2018
 *      Author: Kevin
 */

#ifndef BALL_HPP_
#define BALL_HPP_
#include <stdio.h>
#include <LcdDriver/Crystalfontz128x128_ST7735.hpp>
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.hpp"
#include "Functions.h"

class Ball{
public:
    Ball(uint8_t initXposition, uint8_t initYposition, Graphics_Context *g_sBall1);
    void PrintBall(void);                                   // Print ball each time is needed
    void GetBallPosition(void);                             // Reads the accelerometer to get the new (X,Y)
    void CheckBallCollision(void);                          // If there is any collision the ball will no advance in that direction
    uint8_t BallReachingTarget(void);                             // Returns 1 if the game is done
    void GameEnded(void);

private:
    int16_t l_mActualXYposition[2] = {5,45};                            // X in position 0 and Y in position 1
    int16_t l_mOldXYposition[2] = {5,45};                               // X in position 0 and Y in position 1
    Graphics_Context * m_sGraphContextBall;
};

#endif /* BALL_HPP_ */
