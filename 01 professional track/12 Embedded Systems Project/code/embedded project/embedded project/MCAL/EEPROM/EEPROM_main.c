/*
 * EEPROM_main.c
 *
 * Created: 20/09/2022 10:24:04 pm
 *  Author: abdo
 */ 

 #include "EEPROM_config.h"
 #include "EEPROM_header.h"
 #include "EEPROM_private.h"
 #include "EEPROM_reg.h"
 #include "../../utils/standard_int.h"
 #include "../../utils/math_btt.h"

 /*
  *		@fn			:		-writeCharEEPROMint_Polling
  *
  *		@notes		:		-the function will not end until it writes all the chars into the EEPROM
  *
  *		@brief		:		-takes an char array and store it in the EEPROM,
  *
  *		@params[0]	:		-a struct of type EEPROM_HandleChar_t , check the header file for its members
  *
  *		@return		:		-an Error state indicating whether it stored the array successfully or not
  */
 EEPROM_errorState_t writeCharEEPROMint_Polling(EEPROM_HandleChar_t args_EEPROMHandle_t)
 {
	/*local variable indicating the error state*/
	EEPROM_errorState_t local_ErrorState_t = EEPROM_OK;

	/*local variable to iterate through the array*/
	uint16_t local_u16Iterator = 0;

	/*check for the operands data*/
	if(args_EEPROMHandle_t.u16Address > EEPROM_MAX_ADD || args_EEPROMHandle_t.u16DataLen > EEPROM_MAX_ADD)
	{
		local_ErrorState_t = EEPROM_ADDR_OUT_OF_RANGE;
	}
	/*store the data*/
	else
	{
		/*loop all over the array*/
		for(local_u16Iterator = 0; local_u16Iterator < args_EEPROMHandle_t.u16DataLen; local_u16Iterator++)
		{
			/*Wait until EEWE becomes zero*/
			while(GET_BIT(EECR, EECR_EEWE_1BIT) == HIGH);

			/*Write new EEPROM address to EEAR*/
			EEAR = args_EEPROMHandle_t.u16Address;

			/*increment the address of EEPROM*/
			args_EEPROMHandle_t.u16Address++;

			/*Write new EEPROM data to EEDR*/
			EEDR = *args_EEPROMHandle_t.pu8Data;

			/*increment the pointer pointer by the data*/
			args_EEPROMHandle_t.pu8Data++;

			/*Write a logical one to the EEMWE bit while writing a zero to EEWE in EECR*/
			SET_BIT(EECR, EECR_EEMWE_1BIT);
			CLR_BIT(EECR, EECR_EEWE_1BIT);

			/*Within four clock cycles after setting EEMWE, write a logical one to EEWE*/
			SET_BIT(EECR, EECR_EEWE_1BIT);

			/*call the call back function after the end of the function*/
			args_EEPROMHandle_t.pvoidCallBack();
		}
		
		/*operation went successfully*/
		local_ErrorState_t = EEPROM_OK;
	}

	/*return that error state*/
	return local_ErrorState_t;
 }

 /*
  *		@fn			:		-ReadCharEEPROMint_Polling
  *
  *		@notes		:		-the function will not end until it read all the chars from the EEPROM
  *
  *		@brief		:		-takes an char array and store in it the data from the EEPROM,
  *
  *		@params[0]	:		-a struct of type EEPROM_HandleChar_t , check the header file for its members
  *
  *		@return		:		-an Error state indicating whether it stored the array successfully or not
  */
 EEPROM_errorState_t ReadCharEEPROMint_Polling(EEPROM_HandleChar_t *args_EEPROMHandle_t)
 {
	/*local variable indicating the error state*/
	EEPROM_errorState_t local_ErrorState_t = EEPROM_OK;

	/*local variable to iterate through the array*/
	uint16_t local_u16Iterator = 0;

	/*check for the operands data*/
	if(args_EEPROMHandle_t->u16Address > EEPROM_MAX_ADD || args_EEPROMHandle_t->u16DataLen > EEPROM_MAX_ADD)
	{
		local_ErrorState_t = EEPROM_ADDR_OUT_OF_RANGE;
	}
	/*gets the data*/
	else
	{
		/*loop all over the array*/
		for(local_u16Iterator = 0; local_u16Iterator < args_EEPROMHandle_t->u16DataLen; local_u16Iterator++)
		{
			/*Wait until EEWE becomes zero*/
			while(GET_BIT(EECR, EECR_EEWE_1BIT) == HIGH);

			/*Write new EEPROM address to EEAR*/
			EEAR = args_EEPROMHandle_t->u16Address;

			/*increment the address of EEPROM*/
			args_EEPROMHandle_t->u16Address++;

			/*write logical high to EERE*/
			SET_BIT(EECR, EECR_EERE_1BIT);

			/*gets EEPROM data to variable*/
			*args_EEPROMHandle_t->pu8Data = EEDR;

			/*increment the pointer pointer by the data*/
			args_EEPROMHandle_t->pu8Data++;

			/*call the call back function after the end of the function*/
			args_EEPROMHandle_t->pvoidCallBack();
		}
		
		/*operation went successfully*/
		local_ErrorState_t = EEPROM_OK;
	}

	/*return that error state*/
	return local_ErrorState_t;	

 }

 /*to be implemented later*/
 EEPROM_errorState_t writeCharEEPROMint_IT(EEPROM_HandleChar_t args_EEPROMHandle_t)
 {
	return EEPROM_OK;
 }

 /*to be implemented later*/
 EEPROM_errorState_t ReadCharEEPROMint_IT(EEPROM_HandleChar_t *args_EEPROMHandle_t)
 {
	return EEPROM_OK;
 }



