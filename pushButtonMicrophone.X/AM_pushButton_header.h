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
                    

#ifdef	__cplusplus
}
#endif

#endif	/* AM_PUSHBUTTON_HEADER_H */