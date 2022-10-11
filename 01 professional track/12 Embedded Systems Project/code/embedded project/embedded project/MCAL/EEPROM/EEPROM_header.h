/*
 * EEPROM_header.h
 *
 * Created: 20/09/2022 10:23:42 pm
 *  Author: abdo
 */ 

 /*include necessary header files*/
 #include "../../utils/standard_int.h"

#ifndef EEPROM_HEADER_H_
#define EEPROM_HEADER_H_

/*error state used with the function to return the state of execution*/
typedef enum EEPROM_errorState_t{
	EEPROM_OK	= 0,	
	EEPROM_ADDR_OUT_OF_RANGE = 1,
	
}EEPROM_errorState_t;

/*handle struct to handle the reading and writing to the EEPROM*/
typedef struct EEPROM_HandleChar_t{
	uint8_t* pu8Data;
	uint16_t u16Address;
	uint16_t u16DataLen;
	void (*pvoidCallBack)(void);
}EEPROM_HandleChar_t;

//the size of EEPROM is 1KB
#define EEPROM_MAX_ADD			1023

/*
 *	@public functions
 */

 // writing and reading from the internal EEPROM  using polling method
EEPROM_errorState_t writeCharEEPROMint_Polling(EEPROM_HandleChar_t args_EEPROMHandle_t);
EEPROM_errorState_t ReadCharEEPROMint_Polling(EEPROM_HandleChar_t *args_EEPROMHandle_t);

 // writing and reading from the internal EEPROM  using interrupt method
EEPROM_errorState_t writeCharEEPROMint_IT(EEPROM_HandleChar_t args_EEPROMHandle_t);
EEPROM_errorState_t ReadCharEEPROMint_IT(EEPROM_HandleChar_t *args_EEPROMHandle_t);

#endif /* EEPROM_HEADER_H_ */