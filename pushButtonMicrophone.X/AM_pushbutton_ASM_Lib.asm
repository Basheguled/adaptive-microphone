#include "AM_pushButton_header.h"
; BOILERPLATE CODE
; For assembly libraries used with C programs.
    
    .include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; deine main().  However, we
; we will need a .global statement to make available ASM functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.
.global _delay_100us, _delay_1ms, _wait_1ms

    ; add underscores (_) to function names for global functions

	
_delay_100us:		    ; (100E-6)/(62.5E-9) = 1600 cycles
			    ; 2 cycles for function call
	repeat #1593	    ; 1 cycle to load and prep		
	nop		    ; 1593+1 cycles to execute NOP 1593 times
	return		    ; 3 cycles for the return
	
_delay_1ms:		    ; 62.5ns*16,000cycles = 1ms
			    ; 2 cycles for function call
	repeat #15993	    ; 1 cycle to load and prep
	nop		    ; 15993 + 1 cycles to execute NOP 15994 times
	return		    ; 3 cycles for the return 

_wait_1ms:
   ;2
    repeat #15993 ;1
    nop   ;n+1
    return  ;3
	

    
    
.end















