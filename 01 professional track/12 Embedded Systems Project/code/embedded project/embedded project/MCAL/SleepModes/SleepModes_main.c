/*
 * SleepModes_main.c
 *
 * Created: 23/09/2022 6:51:19 pm
 *  Author: abdo
 */ 

 #include "../atmega32_uPU/atmega32_uPU_header.h"
 #include "SleepModes_config.h"
 #include "SleepModes_header.h"
 #include "SleepModes_header.h"
 #include "SleepModes_reg.h"

 #include "../../utils/standard_int.h"
 #include "../../utils/math_btt.h"


/*
 *	@fn			-:		-enterSleepMode
 *
 *	@brief		-:		-make the MCU enter a specific sleep mode
 *
 *	@params[0]	-:		-refer to @AVAILABLE_SLEEP_MODES in the header to know what params to pass into this function
 *
 *	@notes		-:		-the MCU resumes execution when wake up after end of this function
 *
 *	@return		-:		-return error state indicating the validity of the input
 */
 ES_SLeepMode_t enterSleepMode(uint8_t args_u8Mode)
 {
	/*local variable to indicate the error state*/
	ES_SLeepMode_t local_errorState_t = SLEEP_MODE_OK;

	/*check for the input validity*/
	if(args_u8Mode != SM_IDLE || args_u8Mode != SM_ADC_NOISE_REDUCTION || args_u8Mode != SM_POWER_DOWN || args_u8Mode != SM_POWER_SAVE ||
		args_u8Mode != SM_STANDBY || args_u8Mode != SM_EXTENDED_STANDBY)
	{
		local_errorState_t = SLEEP_MODE_INVALID_INPUT;
	}
	else
	{
		/*configure the sleep mode*/
		ASSIGN_RANGE(MCUCR, MCUCR_SM_3BITS, MCUCR_SM_3BITS + 2, args_u8Mode);

		/*enable the sleep bit*/
		SET_BIT(MCUCR, MCUCR_SE_1BITS);

		/*enter the sleep mode*/	
		SLEEP_FOR_INT();
	}

	/*return the error state variable*/
	return local_errorState_t;
 }