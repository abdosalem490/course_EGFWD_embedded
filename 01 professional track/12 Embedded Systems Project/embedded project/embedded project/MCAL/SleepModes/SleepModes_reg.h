/*
 * SleepModes_reg.h
 *
 * Created: 23/09/2022 6:50:26 pm
 *  Author: abdo
 */ 

 #include "../../utils/standard_int.h"

#ifndef SLEEPMODES_REG_H_
#define SLEEPMODES_REG_H_


#define MCUCR				*((volatile uint8_t* const)0x55)		/*MCU control register*/


/*
 *	bits related to MCUCR
 */
#define MCUCR_SM_3BITS		4										/*sleep modes bits*/	
#define MCUCR_SE_1BITS		7										/*sleep enable*/

#endif /* SLEEPMODES_REG_H_ */