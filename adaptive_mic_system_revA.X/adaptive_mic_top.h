/* 
 * File:   Adaptive Microphone Top Level Header
 * Author: Andy Keech
 * Comments: Sr. Design - Tousi Group - EE4951W
 * Revision history: Revision A:    A. Keech    11/6/2020
 *                   Revision B:      -              -
 * 
 */ 

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADAPTIVE_MIC_TOP_H
#define	ADAPTIVE_MIC_TOP_H


// Maybe Delete this xc.h line??? AK
//#include <xc.h> // include processor files - each processor file is guarded.  


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    extern double rotate;

// Assembly Function Declarations
    void delay_100us(void);
    void delay_1ms(void);
    
    
// Motor Function Declarations
    void delay(int delay_in_ms);            // Based on 1ms delay in assembly library
    void setResolution(int res);
    int angleToSteps(int angle);
    void runCW(int steps);
    void runCCW(int steps);

// Sensor Array Function Declarations
    void initSensor(void);
    double Timing_calculation(void);
    
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

