
#include "xc.h"
#include <stdint.h>
#include <math.h>
#include "adaptive_mic_top.h"

/*****************************************************************************
 * File: Library of functions for Sound Sensor Array
 * Author: Ming Wu
 * Date: 11/6/2020
 ****************************************************************************/

#define abs(x) (x<0?-x:x)
extern double rotate;

double distant_diff_12,distant_diff_23,distant_diff_13;

int count_1 = 0,count_2 = 0,count_3 = 0;
long int curTime_1 = 0;
long int curTime_2 = 0;
long int curTime_3 = 0;
volatile int rollover = 0;
double angle, angle_previous = 0;

void __attribute__((interrupt, auto_psv)) _T2Interrupt()
{      
    rollover++;
    _T2IF = 0;            
}

void __attribute__ ((interrupt, auto_psv)) _IC1Interrupt()
{
    _IC1IF = 0; 
    curTime_1 = TMR2 + (rollover * 16000);
    count_1 = 1;
    
    if (count_1 == 1 && count_2 ==1 && count_3 == 1)
    {
        Timing_calculation();
        count_1 = 0;
        count_2 = 0;
        count_3 = 0;
    }
}

void __attribute__ ((interrupt, auto_psv)) _IC2Interrupt()
{
    _IC2IF = 0;
    curTime_2 = TMR2 + (rollover * 16000);
    count_2 = 1;
    
    if (count_1 == 1 && count_2 ==1 && count_3 == 1)
    {
        Timing_calculation();
        count_1 = 0;
        count_2 = 0;
        count_3 = 0;
    }
}

void __attribute__ ((interrupt, auto_psv)) _IC3Interrupt()
{
    _IC3IF = 0;
    curTime_3 = TMR2 + (rollover * 16000);
    count_3 = 1;
    
    if (count_1 == 1 && count_2 ==1 && count_3 == 1)
    {
        Timing_calculation();
        count_1 = 0;
        count_2 = 0;
        count_3 = 0;
    }
}


void initSensor(void){
    
    //Timer2 period = 0.001s
    T2CON = 0x0000; //Stop Timer, Tcy clk source, PRE 1:1
    TMR2 = 0;     // Initialize to zero
    PR2 = 15999; 
    _T2IF = 0;             //clear interrupt flag
    _T2IE = 1;             //enable interrupt
    T2CONbits.TON = 1; // Restart 16-bit Timer2
    
    
    //set up IC         RB4 -> IC1  RB5 -> IC2  RB6 -> IC3
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR7bits.IC1R = 4;  // Use Pin RP4 = "4", for Input Capture 1 
    RPINR7bits.IC2R = 5;  // Use Pin RP5 for Input Capture 2
    RPINR8bits.IC3R = 6;  // Use Pin RP6 for Input Capture 3
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    
    IC1CON = 0; // Turn off and reset internal state of IC1
    IC1CONbits.ICTMR = 1; // Use Timer 2 for capture source
    IC1CON = 1; // Interrupt on every capture event
    IC1CONbits.ICM = 0b011; // Turn on and capture every rising edge
    _IC1IE = 1; // Enable IC1 interrupts
    
    IC2CON = 0; // Turn off and reset internal state of IC2
    IC2CONbits.ICTMR = 1; // Use Timer 2 for capture source
    IC2CON = 1; // Interrupt on every capture event
    IC2CONbits.ICM = 0b011; // Turn on and capture every rising edge
    _IC2IE = 1; // Enable IC2 interrupts
    
    IC3CON = 0; // Turn off and reset internal state of IC3
    IC3CONbits.ICTMR = 1; // Use Timer 2 for capture source
    IC3CON = 1; // Interrupt on every capture event
    IC3CONbits.ICM = 0b011; // Turn on and capture every rising edge
    _IC3IE = 1; // Enable IC3 interrupts
    
    
}

double Timing_calculation(void)
{    
    double X_test=-30;
    double Y_test=-30;
    double distant_1,distant_2,distant_3;
    double D1_square,D2_square,D3_square;
    
    long int Timediff_12,Timediff_23,Timediff_13;
    double x=0,y=0;
    long sum = 0,sum_mini = 1000000;
//    int check = 0;
    
    Timediff_12 = curTime_1 - curTime_2;
    Timediff_23 = curTime_2 - curTime_3;
    Timediff_13 = curTime_1 - curTime_3;
    
    //The unit of X,Y is 15 cm
    //based on the speed of sound, 15 cm will have 7059 unit difference in Timer 2
    //Test X from -30 to 30, Y from -30 to 30
    //start with X = -30, Y = -30
    while(Y_test <31){
        
         //finish test X range, Y++, go to test next X range
        if(X_test ==30){
            X_test = -30;
            Y_test++;
        }
        
        //calculate the distant from sound source to sensors
        D1_square = (X_test * X_test) + (Y_test * Y_test);
        distant_1 = sqrt(D1_square);
        
        D2_square = ((X_test + 1)*(X_test +1)) + (Y_test * Y_test);
        distant_2 = sqrt(D2_square);
        
        D3_square = ((X_test +1)*(X_test +1)) + ((Y_test+1)*(Y_test+1));
        distant_3 = sqrt(D3_square);
        
        //calculate the distance difference
        distant_diff_12 = distant_1 - distant_2;
        distant_diff_23 = distant_2 - distant_3;
        distant_diff_13 = distant_1 - distant_3;
        

        sum = abs((distant_diff_12 * 7059 - Timediff_12)) + abs((distant_diff_23 * 7059 - Timediff_23)) + abs((distant_diff_13 * 7059 - Timediff_13));
        
        if (sum < sum_mini)
        {
            x = X_test;
            y = Y_test;
            sum_mini = sum;
        }
        
        X_test++;
    }
    
    //
    //transfer location value to angle value
    angle = atan (y/x) *180/3.1415;
    
    if(x < 0){
        angle = 180 + angle;
    }if((x > 0) & (y < 0)){
        angle = angle + 360;
    }
    
    angle = angle;
    
    //determine the rotation of motor
//    rotate = angle - angle_previous;
    
    //only output when the change of angle greater than 5 degree
    //so the microphone working more stable 
    if (abs(angle - angle_previous) > 5)
    {
       
        rotate = angle - angle_previous;
        angle_previous = angle;
    }
    else if (abs(angle - angle_previous) < 5)
    {
        rotate = 0;
    }
    return 0;
}