#include "xc.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "AM_pushButton_header.h"


// PIC24FJ64GA002 Configuration Bit Settings
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config POSCMOD = NONE      // Primary Oscillator Select (Primary oscillator disabled. 
                                   // Primary Oscillator refers to an external osc connected to the OSC1 and OSC2 pins)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // OSC2/CLKO/RC15 functions as port I/O (RC15)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

#define CONTRAST 0b01110111     // 0b10000000 (original contrast setting)

void setup()
{
    /*General HW Setup*/
    CLKDIVbits.RCDIV = 0;       // Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9fff;           // Set all pins to digital
    TRISB = 0;                  // Set PORTB<15:0> to 0000 0000 0000 0000 
    TRISA = 0;
    TRISB |= 0x8070;            // Set PORTB<15:0> to 0000 0000 0111 0011
    
    
    // EasyDriver Pins
    // RB10: Step
    // RB11: Direction
    // RB12: MS1
    // RB13: MS2
    // RB14: Enable
    
    // Pushbutton Pins
    // RB0
    // RB1
    
    // RB4
    // RB5
    // RB6
    
    
    /*Timer 1 Setup - For delay functions in both assembly and C libraries; including 50ms delay, 100ms delay, and more. */
    T1CON = 0;                  // T1 16 bit timer setup: T1 off, prescale 1:1, internal clock
    PR1= 11499;                 // Setting PR1 for 50ms timer 1
    TMR1 = 0;                   // Reset T1 counter
    //_T2IE = 1;                //enable interrupt
    _T1IF = 0;                  // Reset T1 interrupt flag
    T1CON = 0x8020;             // Timer 1 on (bit 15), 1:64 prescaler,
    
}
    


int main(void) {
    setup();
    setResolution(1);
    
    
    double numSteps = 0;
    double tempSteps = 0;
    double angle = 0;
    while(1) {
        if(PORTBbits.RB0 == 1) { // 0 degrees       
            tempSteps = numSteps;

            // set new position
            angle = 0;
            numSteps = angleToSteps(angle);
            if(numSteps - tempSteps > 0)
                runCW(numSteps - tempSteps);
            else
                runCCW((-1)*(numSteps - tempSteps));
        }
        
        else if(PORTBbits.RB1 == 1) { // 72 degrees
            tempSteps = numSteps;
            
            // set new position
            angle = 72;
            numSteps = angleToSteps(angle);
            if(numSteps - tempSteps > 0)
                runCW(numSteps - tempSteps);
            else
                runCCW((-1)*(numSteps - tempSteps));
        }
        
        else if(PORTBbits.RB4 == 1) { // 144 degrees
            tempSteps = numSteps;
            
            // set new position
            angle = 144;
            numSteps = angleToSteps(angle);
            if(numSteps - tempSteps > 0)
                runCW(numSteps - tempSteps);
            else
                runCCW((-1)*(numSteps - tempSteps));
        }
        
        else if(PORTBbits.RB5 == 1) { // 216 degrees
            tempSteps = numSteps;
            
            // set new position
            angle = 216;
            numSteps = angleToSteps(angle);
            if(numSteps - tempSteps > 0)
                runCW(numSteps - tempSteps);
            else
                runCCW((-1)*(numSteps - tempSteps));
        }
        
        else if(PORTBbits.RB6 == 1) { // 288 degrees
            tempSteps = numSteps;
            
            // set new position
            angle = 288;
            numSteps = angleToSteps(angle);
           if(numSteps - tempSteps > 0)
                runCW(numSteps - tempSteps);
            else
                runCCW((-1)*(numSteps - tempSteps));
        }
        
        delay(500);
        PORTAbits.RA0 ^= 1;

        
        
    }
    return 0;
}
