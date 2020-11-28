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
       
    /* Timer 2 Setup - For logging audio events and calculating time difference between sensors.  Period = 1ms = 0.001s */
    T2CON |= 0x0000;            //Stop Timer, Tcy clk source, PRE 1:1
    TMR2 = 0;                   // Initialize to zero
    PR2 = 15999; 
    _T2IF = 0;                  //clear interrupt flag
    _T2IE = 1;                  //enable interrupt
    T2CONbits.TON = 1;          // Restart 16-bit Timer2
    
    /*Timer 3 Setup - For Updating LCD. Period = 100ms*/
    T3CON = 0;
    PR3 = 6250;
    TMR3 = 0;
    _T3IF = 0;
    T3CON = 0x8030;             // Timer 3 on (bit 15), 1:256 prescaler for 100ms)
    _T3IE = 1;
    
    /*I2C2 setup*/
    I2C2CONbits.I2CEN = 0;      // Good practice to disable I2C2 peripheral before changing BRG
    I2C2BRG = 0x9D;             // Setting I2C2 baud rate generator to 100 kHz
    I2C2CONbits.I2CEN = 1;      // Enables the I2C2 module and configures the SDA2 and SCL2 pins 
                                // (pins 6 and 7, respectively), as serial port pins
    IFS3bits.MI2C2IF = 0;       // Clearing I2C2 interrupt flag to be safe
    
}
    


int main(void) {
    setup();
    lcd_init(CONTRAST);                 // LCD initialzation
    dly(32000);                         //~2ms required after init
    setResolution(1);
    
    
    double numSteps;
    double angle = 0;
    while(1) {
        if(PORTBbits.RB0 == 1) { // 0 degrees
       
            // reset to 0 position
            numSteps = angleToSteps(angle);
            runCCW(numSteps);
            
            // set new position
            angle = 0;
            numSteps = angleToSteps(angle);
            runCW(numSteps);
        }
        
        else if(PORTBbits.RB1 == 1) { // 72 degrees
            
            // reset to 0 position
            numSteps = angleToSteps(angle);
            runCCW(numSteps);
            
            // set new position
            angle = 72;
            numSteps = angleToSteps(angle);
            runCW(numSteps);
        }
        
        else if(PORTBbits.RB4 == 1) { // 144 degrees
            
            // reset to 0 position
            numSteps = angleToSteps(angle);
            runCCW(numSteps);
            
            // set new position
            angle = 144;
            numSteps = angleToSteps(angle);
            runCW(numSteps);
        }
        
        else if(PORTBbits.RB5 == 1) { // 216 degrees
            
            // reset to 0 position
            numSteps = angleToSteps(angle);
            runCCW(numSteps);
            
            // set new position
            angle = 216;
            numSteps = angleToSteps(angle);
            runCW(numSteps);
        }
        
        else if(PORTBbits.RB6 == 1) { // 288 degrees
            
            // reset to 0 position
            numSteps = angleToSteps(angle);
            runCCW(numSteps);
            
            // set new position
            angle = 288;
            numSteps = angleToSteps(angle);
            runCW(numSteps);
        }
        
        wait_200ms();
        PORTAbits.RA0 ^= 1;

        
        
    }
    return 0;
}
