#include "AM_pushButton_header.h"
#include "xc.h"
#include <assert.h>
/*****************************************************************************
 * File: Library of functions for I2C communication with LCD
 * Author: Andy Keech
 * Date: 11/5/20
 ****************************************************************************/



/*****************************************************************************
*    Function:  lcd_cmd()
*  Parameters:  char command
*      Return:  nothing
* Description:  Sends a single control or data byte to the I2C LCD display            
*****************************************************************************/
void lcd_cmd (char command)                 // Takes single byte/char command to write to I2C bus
{

    I2C2CONbits.SEN = 1;                    // Send Start bit
    while (I2C2CONbits.SEN == 1);           // Wait for I2C2 action to complete (Goes to zero after start bit sent, HW controlled)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
    
    I2C2TRN = 0b01111100;                   // Send Slave address byte with R/nW
    while (IFS3bits.MI2C2IF == 0);          // Wait for I2C2 action to complete (Wait for I2C2 flag to go to 1)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
    
    I2C2TRN = 0b00000000;                   // Control byte, RS = 0
    while (IFS3bits.MI2C2IF == 0);          // Wait for I2C2 action to complete (Wait for I2C2 flag to go to 1)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0

    I2C2TRN = command;                      // Send command/date byte (send single command to LCD)
    while (IFS3bits.MI2C2IF == 0);          // Wait for I2C2 action to complete (Wait for I2C2 flag to go to 1)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
    
    I2C2CONbits.PEN = 1;                    // Send Stop Bit
    while (I2C2CONbits.PEN == 1);           // Wait for I2C2 action to complete (Goes to zero after stop bit sent, HW controlled)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
    
}
/*****************************************************************************
*    Function:  lcd_init()
*  Parameters:  contrast- allows user to set device contrast from main program
*      Return:  nothing
* Description:  Initializes the LCD display by sending single control byte
                commands to set parameters on the slave device          
*****************************************************************************/
void  lcd_init(int contrast)
{
    wait_50ms();
    lcd_cmd(0b00111000);                    // function set, normal instruction mode
    lcd_cmd(0b00111001);                    // function set, extended instruction mode
    lcd_cmd(0b00010100);                    // interval osc
    lcd_cmd(contrast);
    lcd_cmd(0b01011101);                    // C5-C4, Ion, Bon
    lcd_cmd(0b01101100);                    // follower control
    wait_200ms();
    lcd_cmd(0b00111000);                    // function set, normal instruction mode
    lcd_cmd(0b00001110);                    // Display On, cursor on, cursor position on
    lcd_cmd(0b00000001);                    // Clear Display
    dly(150);                             // Required delay after init
    
}
/*****************************************************************************
*    Function:  lcd_setCursor()
*  Parameters:  char x, char y
*      Return:  nothing
* Description:  Sets the cursor to the desired location. (x = row, y = column)             
*****************************************************************************/
void lcd_setCursor(char x, char y)
{
    char cursor = (0x40*x) + y;
    cursor |= 0b10000000;
    lcd_cmd (cursor);
    
}
/*****************************************************************************
*    Function:  lcd_printChar()
*  Parameters:  char Package- Desired ASCII character to display
*      Return:  nothing
* Description:  Prints a character on the LCD display at the current cursor
                location.             
*****************************************************************************/
void lcd_printChar(char Package)
{
    I2C2CONbits.SEN = 1;                    // Send Start bit
    while (I2C2CONbits.SEN == 1);           // Wait for I2C2 action to complete (Goes to zero after start bit sent, HW controlled)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
    
    I2C2TRN = 0b01111100;                   // Send Slave address
    while (IFS3bits.MI2C2IF == 0);          // Wait for I2C2 action to complete (Wait for I2C2 flag to go to 1)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
    
    I2C2TRN = 0b01000000;                   // Control byte, RS = 1
    while (IFS3bits.MI2C2IF == 0);          // Wait for I2C2 action to complete (Wait for I2C2 flag to go to 1)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
    
    I2C2TRN = Package;                      // Send character package (send single character to LCD at current cursor location)
    while (IFS3bits.MI2C2IF == 0);          // Wait for I2C2 action to complete (Wait for I2C2 flag to go to 1)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
    
    I2C2CONbits.PEN = 1;                    // Send Stop Bit
    while (I2C2CONbits.PEN == 1);           // Wait for I2C2 action to complete (Goes to zero after stop bit sent, HW controlled)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
}
/*****************************************************************************
*    Function:  lcd_printStr()
*  Parameters:  char *s- Pointer that points to the address of C-String
*      Return:  nothing
* Description:  Prints a string on the LCD display at the current cursor
                location.             
*****************************************************************************/
void lcd_printStr(const char *s)
{

    int i = 0;
    
    I2C2CONbits.SEN = 1;                    // Send Start bit
    while (I2C2CONbits.SEN == 1);           // Wait for I2C2 action to complete (Goes to zero after start bit sent, HW controlled)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
    
    I2C2TRN = 0b01111100;                   // Send Slave address
    while (IFS3bits.MI2C2IF == 0);          // Wait for I2C2 action to complete (Wait for I2C2 flag to go to 1)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
    
    
    
    for(i = 0; i < 10; i = i+1)
    {
        I2C2TRN = 0b11000000;                   // Control byte, RS = 1, CO = 1, first of many writes
        while (IFS3bits.MI2C2IF == 0);          // Wait for I2C2 action to complete (Wait for I2C2 flag to go to 1)
        IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
    
        I2C2TRN = s[i];                         // Print first character in string
        while (IFS3bits.MI2C2IF == 0);          // Wait for I2C2 action to complete (Wait for I2C2 flag to go to 1)
        IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
    } 
    
    
    I2C2TRN = 0b01000000;                   // Control byte, RS = 1, Co = 0, last byte
    while (IFS3bits.MI2C2IF == 0);          // Wait for I2C2 action to complete (Wait for I2C2 flag to go to 1)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
 
    I2C2TRN = s[i];                         // Print first character in string
    while (IFS3bits.MI2C2IF == 0);          // Wait for I2C2 action to complete (Wait for I2C2 flag to go to 1)
    IFS3bits.MI2C2IF = 0; 
    
    I2C2CONbits.PEN = 1;                    // Send Stop Bit
    while (I2C2CONbits.PEN == 1);           // Wait for I2C2 action to complete (Goes to zero after stop bit sent, HW controlled)
    IFS3bits.MI2C2IF = 0;                   // I2C2 action complete, reset I2C2 flag to 0
}
/*****************************************************************************
*    Function:  dly()
*  Parameters:  int dly- Desired number of cycles to delay at 16MHz (roughly)
*      Return:  nothing
* Description:  Delays desired number of cycles
                             
*****************************************************************************/
void dly (int dly)
{
    while(dly)
    {
        dly--;
    }
}
/*****************************************************************************
*    Function:  wait_50ms()
*  Parameters:  none
*      Return:  nothing
* Description:  50ms delay using polling of timer 1.            
*****************************************************************************/
void wait_50ms()
{
    TMR1 = 0;                               // Reset timer 1 counter
    _T1IF = 0;                              // Reset timer 1 interrupt flag
    while (_T1IF == 0);                     // Wait for timer 1 interrupt flag to go high (50ms)
    _T1IF = 0;                              // Reset timer 1 interupt flag
}
/*****************************************************************************
*    Function:  wait_200ms()
*  Parameters:  none
*      Return:  nothing
* Description:  Runs 4 iterations of timer 1 (50ms) for a total delay of 200ms           
*****************************************************************************/
void wait_200ms()
{
    int count = 0;
    TMR1 = 0;
    _T1IF = 0;
    while (count < 4)                       // Run 4 iterations of timer 1 (50ms)
    {
        while (_T1IF == 0);
        _T1IF = 0;
        count ++;
    }
}
