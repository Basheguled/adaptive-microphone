/* 
 * File:   AM_pushButton_header.h
 * Author: guledbashe
 *
 * Created on November 28, 2020, 2:47 PM
 */

#ifndef AM_PUSHBUTTON_HEADER_H
#define	AM_PUSHBUTTON_HEADER_H

#ifdef	__cplusplus
extern "C" {
#endif

// Assembly Library Function Declarations
    void delay_100us(void);
    void delay_1ms(void);                   
    void wait_1ms(void);
    
// Motor Library Function Declarations
    void delay(int delay_in_ms);            // Based on 1ms delay in assembly library
    void setResolution(int res);            
    double angleToSteps(double angle);
    void runCW(int steps);
    void runCCW(int steps);
    
// LCD Library Function Declarations
    void lcd_cmd (char command);            
    void lcd_init(int contrast);
    void lcd_setCursor(char x, char y);     
    void lcd_printChar(char Package);
    void lcd_printStr(const char *s);
    void dly(int dly);              
    void wait_50ms(void);                   // Uses Timer 1
    void wait_200ms(void);                  




#ifdef	__cplusplus
}
#endif

#endif	/* AM_PUSHBUTTON_HEADER_H */