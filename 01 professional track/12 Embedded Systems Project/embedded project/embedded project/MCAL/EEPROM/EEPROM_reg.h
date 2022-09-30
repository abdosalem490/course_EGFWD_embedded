/*
 * EEPROM_reg.h
 *
 * Created: 20/09/2022 10:22:53 pm
 *  Author: abdo
 */ 

 /*include necessary header files*/
 #include "../../utils/standard_int.h"


#ifndef EEPROM_REG_H_
#define EEPROM_REG_H_

/*register definitions*/
#define EEAR				*((uint16_t volatile *)0x3E)		/*<EEPROM address register , ranges from 0 to 1023 , initial value is undefined>*/
#define EEDR				*((uint8_t volatile *) 0x3D)		/*<only one byte to be written/read - to/from the address in EEAR>*/
#define EECR				*((uint8_t volatile *) 0x3C)		/*<EEPROM control register>*/

/*register bits definition*/
#define EECR_EERE_1BIT		0
#define EECR_EEWE_1BIT		1
#define EECR_EEMWE_1BIT		2
#define EECR_EERIE_1BIT		3


#endif /* EEPROM_REG_H_ */