/*
 * atmega32_uPU_header.h
 *
 * Created: 20/09/2022 10:10:15 pm
 *  Author: abdo
 */ 

 /*include necessary header files*/
 #include "atmega32_uPU_reg.h"

#ifndef ATMEGA32_UPU_HEADER_H_
#define ATMEGA32_UPU_HEADER_H_


/*some useful micros*/

/*status register saving micros*/
#define SAVE_SREG_IN_VAR(var)			var = SREG;								/*<save the status register in a variable , used when entering interrupt>*/
#define RESTORE_SREG_FROM_VAR(var)		SREG = (char)var;						/*<restore  the status register from a variable , used when exiting interrupt>*/


/*global interrupts micros*/
#define DISABLE_GLOBAL_INT()			asm volatile ("cli");					/*<used to disable the global interrupt , preventing any interrupt to happen>*/
#define ENABLE_GLOBAL_INT()				asm volatile ("sei");					/*<used to enable the global interrupt , allowing any interrupt to happen>*/


/*power saving micros*/
#define SLEEP_FOR_INT()					asm volatile ("sleep");					/*<puts the MCU into sleep until interrupt happens>*/


#endif /* ATMEGA32_UPU_HEADER_H_ */