/*
 * Ball.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: Kevin
 */
#include "Ball.hpp"
#include <math.h>

Ball::Ball(uint8_t initXposition, uint8_t initYposition, Graphics_Context *g_sBallContext){
    l_mActualXYposition[0] = initXposition;
    l_mActualXYposition[1] = initYposition;
    m_sGraphContextBall = g_sBallContext;
}

void Ball::PrintBall(void){
    CheckBallCollision();

    /*Cricles to surround the main circle
     * these cricles are white and clean up
     * the last marks of the main circle*/
    Graphics_drawCircle(&g_sEraser, l_mActualXYposition[0], l_mActualXYposition[1], 3);
    Graphics_drawCircle(&g_sEraser, l_mActualXYposition[0], l_mActualXYposition[1], 4);
    Graphics_drawCircle(&g_sEraser, l_mActualXYposition[0], l_mActualXYposition[1], 5);
    Graphics_fillCircle(m_sGraphContextBall, l_mActualXYposition[0], l_mActualXYposition[1], 2);        //Main Circle
}

void Ball::GetBallPosition(void){
    GetAccelData();
    l_mOldXYposition[0] = l_mActualXYposition[0];
    l_mOldXYposition[1] = l_mActualXYposition[1];

    /*there is a nested int16_t conversion to guarantee that just changes
     *on accelerometer above 5000 will change the ball position*/
    l_mActualXYposition[0] = (int16_t) ((int16_t(g_u16resultsBuffer[0]*0.0004) + l_mActualXYposition[0]));
    l_mActualXYposition[1] = (int16_t) ((-int16_t(g_u16resultsBuffer[1]*0.0004) + l_mActualXYposition[1]));

    /* This code limits the maximum distance
     * between the last print of the ball
     * and the actual print of the ball
     * to avoid rests of the ball along the
     * screen.*/


    if(abs(l_mActualXYposition[0]-l_mOldXYposition[0]) >= 2){
        if(l_mActualXYposition[0] > l_mOldXYposition[0]){
            l_mActualXYposition[0] = l_mOldXYposition[0] + 1;
        }
        if(l_mActualXYposition[0] < l_mOldXYposition[0]){
            l_mActualXYposition[0] = l_mOldXYposition[0] - 1;
        }
    }
    if(abs(l_mActualXYposition[1]-l_mOldXYposition[1]) >= 2){
        if(l_mActualXYposition[1] > l_mOldXYposition[1]){
            l_mActualXYposition[1] = l_mOldXYposition[1] + 1;
        }
        if(l_mActualXYposition[1] < l_mOldXYposition[1]){
            l_mActualXYposition[1] = l_mOldXYposition[1] - 1;
        }
    }


    /* If the read of the position of the ball
     * exceeds the screen boundaries these ifs
     * will limit its position to the max allowed*/
    if(l_mActualXYposition[0]>=122){
        l_mActualXYposition[0]=122;
    }
    if(l_mActualXYposition[1]>=122){
        l_mActualXYposition[1]=122;
    }
    if(l_mActualXYposition[0]<=5){
        l_mActualXYposition[0]=5;
    }
    if(l_mActualXYposition[1]<=5){
        l_mActualXYposition[1]=5;
    }
}

void Ball::CheckBallCollision(void){
    GetBallPosition();

    /* The following comment apply to all the other code blocks:
     *
     * the values for l_mOldXYposition[0] and l_mActualXYposition[0]
     * are related with the ball radius, which is 2 and has
     * 3 white circles surrounding it, so the final radius
     * of the ball is 5, making us to hold a distance
     * between the line and the ball of at least 6 units. */


    /* FIRST PATTERN ALONG Y
     * This code works to avoid cut the first line of the
     * pattern made to create the maze, these lines are
     * located at X=15, X=60 and X=105.
     *
     * if ball >= 111 && wants to be < 111
     * or 96 <= ball <= 99 && wants to be > 99
     * or
     * 66 <= ball <= 69 && wants to be < 66
     * or 51 <= ball <= 54 && wants to be > 54
     * or
     * 21 <= ball <= 24 && wants to be < 21
     * or 9 <= ball && wants to be > 9*/
    if(
            ((l_mOldXYposition[0] >= 111 && l_mActualXYposition[0] < 111)
            || (l_mOldXYposition[0] >= 96 && l_mOldXYposition[0] <= 99 && l_mActualXYposition[0] > 99))
                ||
            ((l_mOldXYposition[0] >= 66 && l_mOldXYposition[0] <= 69 && l_mActualXYposition[0] < 66)
            || (l_mOldXYposition[0] >= 51 && l_mOldXYposition[0] <= 54 && l_mActualXYposition[0] > 54))
                ||
            ((l_mOldXYposition[0] >= 21 && l_mOldXYposition[0] <= 24 && l_mActualXYposition[0] < 21)
            || (l_mOldXYposition[0] <= 9 && l_mActualXYposition[0] > 9))
      ){

        /*This if allows the ball to go "through" the first line of the pattern
         * the line has 3 holes, located at 15 <= Y <= 30, 45 <= Y <= 60 and
         * 90 <= Y <= 105, so the ball is allowed to pass if it is between
         * 21 - 24, 51 - 54 or 96 - 99*/
        if(
                (l_mActualXYposition[1] >= 21 && l_mActualXYposition[1] <= 24)
                ||
                (l_mActualXYposition[1] >= 51 && l_mActualXYposition[1] <= 54)
                ||
                (l_mActualXYposition[1] >= 96 && l_mActualXYposition[1] <= 99)
                ){
            // Do nothing
        }else{
            l_mActualXYposition[0] = l_mOldXYposition[0];
        }

    }

    /* SECOND PATTERN ALONG Y
     * This code works to avoid cut the second line of the
     * pattern made to create the maze, these lines are
     * located at X=30 and X=75.
     *
     * if 81 <= ball <= 84 && wants to be < 81
     * or 66 <= ball <= 69 && wants to be > 69
     * or
     * 36 <= ball <= 39 && wants to be < 36
     * or 21 <= ball <= 24 && wants to be > 24*/
     if(
            ((l_mOldXYposition[0] >= 81 && l_mOldXYposition[0] <= 84 && l_mActualXYposition[0] < 81)
            || (l_mOldXYposition[0] >= 66 && l_mOldXYposition[0] <= 69 && l_mActualXYposition[0] > 69))
                ||
            ((l_mOldXYposition[0] >= 36 && l_mOldXYposition[0] <= 39 && l_mActualXYposition[0] < 36)
            || (l_mOldXYposition[0] >= 21 && l_mOldXYposition[0] <= 24 && l_mActualXYposition[0] > 24))
      ){

        /*This if allows the ball to go "through" the first line of the pattern
         * the line has 3 holes, located at 15 <= Y <= 30, 60 <= Y <= 75 and
         * 105 <= Y <= 127, so the ball is allowed to pass if it is between
         * 21 - 24, 66 - 69 or 111 - 127*/
        if(
                (l_mActualXYposition[1] >= 21 && l_mActualXYposition[1] <= 24)
                ||
                (l_mActualXYposition[1] >= 66 && l_mActualXYposition[1] <= 69)
                ||
                (l_mActualXYposition[1] >= 111 && l_mActualXYposition[1] <= 127)
                ){
            // Do nothing
        }else{
            l_mActualXYposition[0] = l_mOldXYposition[0];
        }

    }

     /* THIRD PATTERN ALONG Y
      * This code works to avoid cut the third line of the
      * pattern made to create the maze, these lines are
      * located at X=45 and X=90.
      *
      * if 96 <= ball <= 99 && wants to be < 96
      * or 81 <= ball <= 84 && wants to be > 84
      * or
      * 51 <= ball <= 54 && wants to be < 51
      * or 36 <= ball <= 39 && wants to be > 39*/
      if(
             ((l_mOldXYposition[0] >= 96 && l_mOldXYposition[0] <= 99 && l_mActualXYposition[0] < 96)
             || (l_mOldXYposition[0] >= 81 && l_mOldXYposition[0] <= 84 && l_mActualXYposition[0] > 84))
                 ||
             ((l_mOldXYposition[0] >= 51 && l_mOldXYposition[0] <= 54 && l_mActualXYposition[0] < 51)
             || (l_mOldXYposition[0] >= 36 && l_mOldXYposition[0] <= 39 && l_mActualXYposition[0] > 39))
       ){

         /*This if allows the ball to go "through" the third line of the pattern
          * the line has 2 holes, located at 0 <= Y <= 15 and 105 <= Y <= 127,
          * so the ball is allowed to pass if it is between 6 - 9 or 111 - 121*/
         if(
                 (l_mActualXYposition[1] >= 0 && l_mActualXYposition[1] <= 9)
                 ||
                 (l_mActualXYposition[1] >= 111 && l_mActualXYposition[1] <= 127)
                 ){
             // Do nothing
         }else{
             l_mActualXYposition[0] = l_mOldXYposition[0];
         }
     }

      /* FIRST PATTERN ALONG X
       * This code works to avoid cut the first line of the
       * pattern made to create the maze, these lines are
       * located at Y=15, Y=60 and Y=105.
       *
       * if ball >= 111 && wants to be < 111
       * or 96 <= ball <= 99 && wants to be > 99
       * or
       * 66 <= ball <= 69 && wants to be < 66
       * or 51 <= ball <= 54 && wants to be > 54
       * or
       * 21 <= ball <= 24 && wants to be < 21
       * or 9 <= ball && wants to be > 9*/
       if(
          ((l_mOldXYposition[1] >= 111 && l_mActualXYposition[1] < 111)
          || (l_mOldXYposition[1] >= 96 && l_mOldXYposition[1] <= 99 && l_mActualXYposition[1] > 99))
              ||
          ((l_mOldXYposition[1] >= 66 && l_mOldXYposition[1] <= 69 && l_mActualXYposition[1] < 66)
          || (l_mOldXYposition[1] >= 51 && l_mOldXYposition[1] <= 54 && l_mActualXYposition[1] > 54))
              ||
          ((l_mOldXYposition[1] >= 21 && l_mOldXYposition[1] <= 24 && l_mActualXYposition[1] < 21)
          || (l_mOldXYposition[1] <= 9 && l_mActualXYposition[1] > 9))
        ){

          /*This if allows the ball to go "through" the first line of the pattern
           * the line has 3 holes, located at 15 <= X <= 30, 45 <= X <= 60 and
           * 90 <= X <= 105, so the ball is allowed to pass if it is between
           * 0 - 9, 36 - 54 or 81 - 99*/
           if(
              (l_mActualXYposition[0] >= 0 && l_mActualXYposition[0] <= 9)
              ||
              (l_mActualXYposition[0] >= 36 && l_mActualXYposition[0] <= 54)
              ||
              (l_mActualXYposition[0] >= 81 && l_mActualXYposition[0] <= 99)
           ){
              // Do nothing
           }else{
              l_mActualXYposition[1] = l_mOldXYposition[1];
           }
       }

       /* SECOND PATTERN ALONG X
        * This code works to avoid cut the second line of the
        * pattern made to create the maze, these lines are
        * located at Y=30 and Y=75.
        *
        * if 81 <= ball <= 84 && wants to be < 81
        * or 66 <= ball <= 69 && wants to be > 69
        * or
        * 36 <= ball <= 39 && wants to be < 36
        * or 21 <= ball <= 24 && wants to be > 24*/
        if(
               ((l_mOldXYposition[1] >= 81 && l_mOldXYposition[1] <= 84 && l_mActualXYposition[1] < 81)
               || (l_mOldXYposition[1] >= 66 && l_mOldXYposition[1] <= 69 && l_mActualXYposition[1] > 69))
                   ||
               ((l_mOldXYposition[1] >= 36 && l_mOldXYposition[1] <= 39 && l_mActualXYposition[1] < 36)
               || (l_mOldXYposition[1] >= 21 && l_mOldXYposition[1] <= 24 && l_mActualXYposition[1] > 24))
         ){

           /*This if allows the ball to go "through" the second line of the pattern
            * the line has 2 holes, located at 0 <= X <= 60 and 75 <= X <= 127
            * so the ball is allowed to pass if it is between 0 - 54, 81 - 127*/
           if(
                   (l_mActualXYposition[0] >= 0 && l_mActualXYposition[0] <= 54)
                   ||
                   (l_mActualXYposition[0] >= 81 && l_mActualXYposition[0] <= 127)
                   ){
               // Do nothing
           }else{
               l_mActualXYposition[1] = l_mOldXYposition[1];
           }
       }

        /* THIRD PATTERN ALONG X
         * This code works to avoid cut the third line of the
         * pattern made to create the maze, these lines are
         * located at Y=45 and Y=90.
         *
         * if 96 <= ball <= 99 && wants to be < 96
         * or 81 <= ball <= 84 && wants to be > 84
         * or
         * 51 <= ball <= 54 && wants to be < 51
         * or 36 <= ball <= 39 && wants to be > 39*/
         if(
                ((l_mOldXYposition[1] >= 96 && l_mOldXYposition[1] <= 99 && l_mActualXYposition[1] < 96)
                || (l_mOldXYposition[1] >= 81 && l_mOldXYposition[1] <= 84 && l_mActualXYposition[1] > 84))
                    ||
                ((l_mOldXYposition[1] >= 51 && l_mOldXYposition[1] <= 54 && l_mActualXYposition[1] < 51)
                || (l_mOldXYposition[1] >= 36 && l_mOldXYposition[1] <= 39 && l_mActualXYposition[1] > 39))
          ){

            /*This if allows the ball to go "through" the third line of the pattern
             * the line has 2 holes, located at 15 <= X <= 75 and 90 <= Y <= 127,
             * so the ball is allowed to pass if it is between 21 - 69 or 96 - 127*/
            if(
                    (l_mActualXYposition[0] >= 21 && l_mActualXYposition[0] <= 69)
                    ||
                    (l_mActualXYposition[0] >= 96 && l_mActualXYposition[0] <= 127)
                    ){
                // Do nothing
            }else{
                l_mActualXYposition[1] = l_mOldXYposition[1];
            }
        }



         /* This code is intended to limit the ball movement
          * when it is passing through the X = 15*n lines to
          * avoid line deleting due to ball movement */

         // Ball transitioning the X lines
         if(
                 (l_mActualXYposition[0] < 111 && l_mActualXYposition[0] > 99)
                 ||
                 (l_mActualXYposition[0] < 96 && l_mActualXYposition[0] > 84)
                 ||
                 (l_mActualXYposition[0] < 81 && l_mActualXYposition[0] > 69)
                 ||
                 (l_mActualXYposition[0] < 66 && l_mActualXYposition[0] > 54)
                 ||
                 (l_mActualXYposition[0] < 51 && l_mActualXYposition[0] > 39)
                 ||
                 (l_mActualXYposition[0] < 36 && l_mActualXYposition[0] > 24)
                 ||
                 (l_mActualXYposition[0] < 21 && l_mActualXYposition[0] > 9)
            ){

             // Ball trying to go above the Y lines by its ends
             if(
                 (l_mOldXYposition[1] <= 9 && l_mOldXYposition[1] >= 0 && l_mActualXYposition[1] > 9)
                 ||
                 (l_mOldXYposition[1] <= 24 && l_mOldXYposition[1] >= 21 && (l_mActualXYposition[1] > 24 || l_mActualXYposition[1] < 21))
                 ||
                 (l_mOldXYposition[1] <= 39 && l_mOldXYposition[1] >= 36 && (l_mActualXYposition[1] > 39 || l_mActualXYposition[1] < 36))
                 ||
                 (l_mOldXYposition[1] <= 54 && l_mOldXYposition[1] >= 51 && (l_mActualXYposition[1] > 54 || l_mActualXYposition[1] < 51))
                 ||
                 (l_mOldXYposition[1] <= 69 && l_mOldXYposition[1] >= 66 && (l_mActualXYposition[1] > 69 || l_mActualXYposition[1] < 66))
                 ||
                 (l_mOldXYposition[1] <= 84 && l_mOldXYposition[1] >= 81 && (l_mActualXYposition[1] > 84 || l_mActualXYposition[1] < 81))
                 ||
                 (l_mOldXYposition[1] <= 99 && l_mOldXYposition[1] >= 96 && (l_mActualXYposition[1] > 99 || l_mActualXYposition[1] < 96))
                 ||
                 (l_mOldXYposition[1] >= 111 && l_mActualXYposition[1] < 111)
             ){
                 l_mActualXYposition[1] = l_mOldXYposition[1];
             }else{
                 //Do nothing
             }
         }
}

uint8_t Ball::BallReachingTarget(void){
    if(l_mActualXYposition[0] > 0 && l_mActualXYposition[0] < 9 && l_mActualXYposition[1] > 111){
        return 1;
    }
    return 0;
}

void Ball::GameEnded(void){
    Graphics_clearDisplay(m_sGraphContextBall);
    Graphics_setFont(m_sGraphContextBall, &g_sFontCm28);
    int8_t WinText[8] = "YOU WIN";
    Graphics_drawString(m_sGraphContextBall, &WinText[0], 7, 3, 55, 1);
}


