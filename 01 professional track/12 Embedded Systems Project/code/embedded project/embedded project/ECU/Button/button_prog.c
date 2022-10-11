/*
 * button_prog.c
 *
 * Created: 28/09/2022 6:07:38 am
 *  Author: abdo
 */ 

 #include "button_config.h"
 #include "button_header.h"
 #include "button_private.h"

 #include "../../utils/math_btt.h"
 #include "../../MCAL/GPIO/GPIO_header.h"

 /*
 *	@fn			-:		-button_init_Polling
 *
 *	@params[0]	-:		-symbol representing the port name , options are in GPIO header file , refer to @PORTS_NAMES
 *
 *	@params[1]	-:		-symbol representing the pin number name , options are in GPIO  header file , refer to @PINS_NAMES
 *
 *	@params[2]	-:		-symbol representing whether external or internal resistor to be used , options are in  header file , refer to @PINS_NAMES
 *
 *	@brief		-:		-initializes a pin to be either input
 *
 *	@return		-:		=return error state indicating the validity of the input
 */
ES_BUTTON_t button_init_Polling(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber, uint8_t args_u8InternOrExternalRes)
{	
	/*local variable to indicate the validity of the input*/
	ES_BUTTON_t local_errorState  = BUTTON_OK;

	// validate the input of args_u8InternOrExternalRes
	if (args_u8InternOrExternalRes > BUTTON_EXTERNAL_PULL_UP)
	{
		local_errorState = BUTTON_INVALID_IN; 
	}
	else
	{	
		/*external pull resistor*/
		if(args_u8InternOrExternalRes == BUTTON_EXTERNAL_PULL_UP)
		{
			if(GPIO_PinInit(args_u8PortNumber, args_u8PinNumber, GPIO_MODE_IN) != GPIO_OK)
				local_errorState = BUTTON_INVALID_IN; 
		}
		else if(args_u8InternOrExternalRes == BUTTON_INTERNAL_PULL_UP)
		{
			if(GPIO_PinInit(args_u8PortNumber, args_u8PinNumber, GPIO_MODE_IN_PULLUP) != GPIO_OK)
				local_errorState = BUTTON_INVALID_IN;

		}
	}

	/*return the error state*/
	return local_errorState;
}


 /*
 *	@fn			-:		-button_init_IT
 *
 *	@params[0]	-:		-symbol representing the interrupt names, options are in GPIO header file , refer to @INT_NAMES
 *
 *	@params[1]	-:		-symbol representing the interrupt mode , options are in GPIO  header file , refer to @INT_MODES
 *
 *	@params[2]	-:		-a call back function to be called when there is external interrupt
 *
 *	@params[3]	-:		-symbol representing whether external or internal resistor to be used , options are in  header file , refer to @OPTIONS_RES_TYPES
 *
 *	@brief		-:		-initializes an external interrupt pin to a specific mode 
 *
 *	@return		-:		=return error state indicating the validity of the input
 */
ES_BUTTON_t button_init_IT(uint8_t args_u8IntNum, uint8_t args_u8IntMode, void (*args_pvCallBack) (void), uint8_t args_u8InternOrExternalRes)
{
	/*local variable to indicate the validity of the input*/
	ES_BUTTON_t local_errorState  = BUTTON_OK;

	// validate the input of args_u8InternOrExternalRes
	if (args_u8InternOrExternalRes > BUTTON_EXTERNAL_PULL_UP)
	{
		local_errorState = BUTTON_INVALID_IN;
	}
	else
	{	
		switch(args_u8IntNum)
		{
			case GPIO_INT0:
			{
				/*external pull resistor*/
				if(args_u8InternOrExternalRes == BUTTON_EXTERNAL_PULL_UP || args_u8InternOrExternalRes == BUTTON_EXTERNAL_PULL_DOWN)
				{
					if(GPIO_PinInit(GPIO_PORTD, GPIO_PIN2, GPIO_MODE_IN) != GPIO_OK || GPIO_EnableExtInterrupt(GPIO_INT0, args_u8IntMode, args_pvCallBack) != GPIO_OK)
						local_errorState = BUTTON_INVALID_IN;
				}
				else if(args_u8InternOrExternalRes == BUTTON_INTERNAL_PULL_UP)
				{
					if(GPIO_PinInit(GPIO_PORTD, GPIO_PIN2, GPIO_MODE_IN_PULLUP) != GPIO_OK || GPIO_EnableExtInterrupt(GPIO_INT0, args_u8IntMode, args_pvCallBack) != GPIO_OK)
						local_errorState = BUTTON_INVALID_IN;
				}

				break;
			}

			case GPIO_INT1:
			{
				/*external pull resistor*/
				if(args_u8InternOrExternalRes == BUTTON_EXTERNAL_PULL_UP || args_u8InternOrExternalRes == BUTTON_EXTERNAL_PULL_DOWN)
				{
					if(GPIO_PinInit(GPIO_PORTD, GPIO_PIN3, GPIO_MODE_IN) != GPIO_OK || GPIO_EnableExtInterrupt(GPIO_INT1, args_u8IntMode, args_pvCallBack) != GPIO_OK)
					local_errorState = BUTTON_INVALID_IN;
				}
				else if(args_u8InternOrExternalRes == BUTTON_INTERNAL_PULL_UP)
				{
					if(GPIO_PinInit(GPIO_PORTD, GPIO_PIN3, GPIO_MODE_IN_PULLUP) != GPIO_OK || GPIO_EnableExtInterrupt(GPIO_INT1, args_u8IntMode, args_pvCallBack) != GPIO_OK)
					local_errorState = BUTTON_INVALID_IN;
				}

				break;
			}

			case GPIO_INT2:
			{
				/*external pull resistor*/
				if(args_u8InternOrExternalRes == BUTTON_EXTERNAL_PULL_UP || args_u8InternOrExternalRes == BUTTON_EXTERNAL_PULL_DOWN)
				{
					if(GPIO_PinInit(GPIO_PORTB, GPIO_PIN2, GPIO_MODE_IN) != GPIO_OK || GPIO_EnableExtInterrupt(GPIO_INT2, args_u8IntMode, args_pvCallBack) != GPIO_OK)
					local_errorState = BUTTON_INVALID_IN;
				}
				else if(args_u8InternOrExternalRes == BUTTON_INTERNAL_PULL_UP)
				{
					if(GPIO_PinInit(GPIO_PORTB, GPIO_PIN2, GPIO_MODE_IN_PULLUP) != GPIO_OK || GPIO_EnableExtInterrupt(GPIO_INT2, args_u8IntMode, args_pvCallBack) != GPIO_OK)
					local_errorState = BUTTON_INVALID_IN;
				}

				break;
			}

			default:
				local_errorState = BUTTON_INVALID_IN;
				break;
		}


	}

	
	/*return the error state*/
	return local_errorState;		
}


 /*
 *	@fn			-:		-button_read
 *
 *	@params[0]	-:		-symbol representing the port name , options are in GPIO header file , refer to @PORTS_NAMES
 *
 *	@params[1]	-:		-symbol representing the pin number name , options are in GPIO  header file , refer to @PINS_NAMES
 *
 *	@params[2]	-:		-a variable to read in it the state of the button
 *
 *	@params[3]	-:		-symbol representing the button type , options are in  header file , refer to @OPTIONS_BUTTON_TYPES
 *
 *	@brief		-:		-initializes a pin to be either input
 *
 *	@return		-:		=return error state indicating the validity of the input
 */
ES_BUTTON_t button_read(uint8_t args_u8PortNumber, uint8_t args_u8PinNumber, uint8_t *args_u8Input, uint8_t args_u8ButtonType)
{

	if(GPIO_PinRead(args_u8PortNumber, args_u8PinNumber, args_u8Input) != GPIO_OK || args_u8ButtonType > BUTTON_TYPE_PULL_DOWN)
		return BUTTON_INVALID_IN;
	else
	{
		if(*args_u8Input == HIGH && args_u8ButtonType == BUTTON_TYPE_PULL_UP)
			*args_u8Input = BUTTON_STATE_UN_PRESSED;
		else if(*args_u8Input == LOW && args_u8ButtonType == BUTTON_TYPE_PULL_UP)
			*args_u8Input = BUTTON_STATE_PRESSED;		
		else if(*args_u8Input == HIGH && args_u8ButtonType == BUTTON_TYPE_PULL_DOWN)
			*args_u8Input = BUTTON_STATE_PRESSED;
		else if(*args_u8Input == LOW && args_u8ButtonType == BUTTON_TYPE_PULL_DOWN)
			*args_u8Input = BUTTON_STATE_UN_PRESSED;

		return BUTTON_OK;
	}
}


 /*
 *	@fn			-:		-button_deinit_IT
 *
 *	@params[0]	-:		-symbol representing the interrupt names, options are in GPIO header file , refer to @INT_NAMES
 *
 *	@params[1]	-:		-symbol representing the interrupt mode , options are in GPIO  header file , refer to @INT_MODES
 *
 *	@params[2]	-:		-a call back function to be called when there is external interrupt
 *
 *	@params[3]	-:		-symbol representing whether external or internal resistor to be used , options are in  header file , refer to @OPTIONS_RES_TYPES
 *
 *	@brief		-:		-initializes an external interrupt pin to a specific mode 
 *
 *	@return		-:		=return error state indicating the validity of the input
 */
ES_BUTTON_t button_deinit_IT(uint8_t args_u8IntNum)
{
	/*trying to disable the external interrupt*/
	if(GPIO_DisableExtInterrupt(args_u8IntNum) != GPIO_OK)
	  return BUTTON_INVALID_IN;
	else
		return BUTTON_OK;

}
