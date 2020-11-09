#include "adaptive_mic_top.h"
#include "xc.h"
/*****************************************************************************
 * File: Library of functions for SparkFun EasyDriver Stepper Motor Driver
 * Author: Andy Keech
 * Date: 11/6/2020
 ****************************************************************************/


/*****************************************************************************
*    Function:  delay()
*  Parameters:  int delay_in_ms - Integer value of desired delay in milliseconds
*      Return:  Nothing
* Description:  Calls a 1ms delay (assembly function) a number of times specified by the user                                  
*****************************************************************************/
void delay(int delay_in_ms)
{
    while(delay_in_ms)
    {
        delay_1ms();
        delay_in_ms--;
    }
    
    
}
/*****************************************************************************
*    Function:  setResolution()
*  Parameters:  int res - Integer value of the desired resolution (1-4) corresponding with 
 *              full stepping, half stepping, quarter stepping, or eighth stepping
*      Return:  Nothing
* Description:  Sets the EasyDriver stepper motor driver stepping mode (Full, half, quarter, eighth stepping)
 *              Allows micro-stepping, although testing determined full stepping at a faster speed is much smoother.                                   
*****************************************************************************/
void setResolution(int res)
{
    if(res == 1)            // Full Stepping
    {
        LATBbits.LATB12 = 0;  //MS1
        LATBbits.LATB13 = 0;  //MS2
    }
    else if(res == 2)       // Half Stepping
    {
        LATBbits.LATB12 = 1;  //MS1
        LATBbits.LATB13 = 0;  //MS2
    }
    else if(res == 3)       // Quarter Stepping
    {
        LATBbits.LATB12 = 0;  //MS1
        LATBbits.LATB13 = 1;  //MS2
    }
    else if (res == 4)       // Eighth Stepping
    {
        LATBbits.LATB12 = 1;  //MS1
        LATBbits.LATB13 = 1;  //MS2
    }
        
}
/*****************************************************************************
*    Function:  angleToSteps()
*  Parameters:  int angle - Desired angle to move the microphone to
*      Return:  stepsFromAngle - Calculated number of steps motor must move to reach
 *              the desired angle. Based on steps = desired angle / 1.8 steps per revolution
* Description:  Calls a 1ms delay (assembly function) a number of times specified by the user                                  
*****************************************************************************/
int angleToSteps(int angle)
{
    int stepsFromAngle;
    double degPerStep = 1.8;
    stepsFromAngle = angle / degPerStep;
    return stepsFromAngle;
}



void runCW(int steps)
{
    
    LATBbits.LATB11 = 0;        // Set direction pin to turn motor CW
    LATBbits.LATB14 = 0;        // Enable the driver
    static int i;
    for(i = 0; i <steps; i = i+1)     // Repeat 200 times to make 1 full revolution in full step mode
            {
                delay_1ms();
                asm ("btg  LATB, #10");
                delay_1ms();
                asm ("btg  LATB, #10");
            }
    

}
void runCCW(int steps)
{
    LATBbits.LATB11 = 1;        // Set direction pin to turn motor CCW
    LATBbits.LATB14 = 0;        // Enable the driver
    static int j;
    for(j = 0; j <steps; j = j+1)     // Repeat 200 times to make 1 full revolution in full step mode
            {
                delay_1ms();
                asm ("btg  LATB, #10");
                delay_1ms();
                asm ("btg  LATB, #10");
            }

}