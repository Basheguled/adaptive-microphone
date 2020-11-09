/*
 * File:   adaptive_mic_main.c
 * Author: andyk
 *
 * Created on November 6, 2020, 10:08 PM
 */


#include <p24Fxxxx.h>
#include <xc.h>
#include "adaptive_mic_top.h"

// PIC24FJ64GA002 Configuration Bit Settings
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled. 
					// Primary Oscillator refers to an external osc connected to the OSC1 and OSC2 pins)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // OSC2/CLKO/RC15 functions as port I/O (RC15)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
double rotate =0;
int numSteps=0;
void setup(void) 
{
    CLKDIVbits.RCDIV = 0;           //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M

    AD1PCFG = 0x9fff;               // all digital except 14 and 13 for programming PIC24
    TRISA = 0;                      // Initialize all Port A Bits to outputs
    TRISB = 0;                      // Initialize all Port B Bits to outputs
    LATA = 0;                       // Initialize all outputs to low
    LATB = 0;                       // Initialize all outputs to low

    TRISB |= 0x0070;                // Set pins to outputs or inputs for external components
    // Sound Detector Sensor Pins
    // RB4: IC1
    // RB5: IC2
    // RB6: IC3
    // EasyDriver Pins
    // RB10: Step
    // RB11: Direction
    // RB12: MS1
    // RB13: MS2
    // RB14: Enable
}


int main(void) 
{
        setup();
        initSensor();
        setResolution(1);
        
        int angle = 0;
        int numSteps = 0;
        runCW(numSteps);
        
        
        
        while(1)
        {
            numSteps = angleToSteps(30);
            runCW(numSteps);
            delay(2000);
            
            numSteps = angleToSteps(60);
            runCW(numSteps);
            delay(2000);
            
            numSteps = angleToSteps(90);
            runCW(numSteps);
            delay(2000);
            
            numSteps = angleToSteps(180);
            runCW(numSteps);
            delay(2000);
            
        }
            
            
//            if (rotate < 0)
//            {
//                angle = -rotate;
//                numSteps = angleToSteps(angle);
//                runCCW(numSteps);
//            }
//            else if (rotate > 0)
//            {
//                numSteps = angleToSteps(rotate);
//                runCW(numSteps);
//            }
//            else
//            {
//                numSteps = 0;
//                runCW(numSteps);
//            }
//            delay(1000);
        
            //rotate = 0;
            //numSteps = 0;
            


     
        
            return 0;
        
               
}